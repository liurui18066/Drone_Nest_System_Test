#include "plc_comm.h"

#include <QModbusDataUnit>
#include <QModbusReply>
#include <QVariant>
#include <QVector>

namespace {

int seqSlot(quint8 cmd)
{
    if (cmd == Protocol::kCmdHealth) return 0;
    if (cmd == Protocol::kCmdServo) return 1;
    if (cmd == Protocol::kCmdBattery) return 2;
    return -1;
}

QByteArray registersToFrame(const QModbusDataUnit &unit)
{
    QByteArray frame;
    frame.reserve(int(unit.valueCount()) * 2);
    for (uint i = 0; i < unit.valueCount(); ++i) {
        const quint16 reg = unit.value(i);
        frame.append(static_cast<char>(reg >> 8));
        frame.append(static_cast<char>(reg & 0xFF));
    }
    return frame;
}

QVector<quint16> frameToRegisters(const QByteArray &frame)
{
    QVector<quint16> registers;
    for (int i = 0; i + 1 < frame.size(); i += 2) {
        const quint16 reg = (static_cast<quint8>(frame.at(i)) << 8)
                            | static_cast<quint8>(frame.at(i + 1));
        registers.append(reg);
    }
    return registers;
}

} // namespace

PlcComm::PlcComm(const QString &host, int port, int slaveId, quint8 planeModel, QObject *parent)
    : QObject(parent)
    , m_host(host)
    , m_port(port)
    , m_slaveId(slaveId)
    , m_planeModel(planeModel)
    , m_connected(0)
{
    m_lastSeq[0] = m_lastSeq[1] = m_lastSeq[2] = 0;
    m_hasSeq[0] = m_hasSeq[1] = m_hasSeq[2] = false;
    m_sameSeqCount[0] = m_sameSeqCount[1] = m_sameSeqCount[2] = 0;
    m_allZeroNoted[0] = m_allZeroNoted[1] = m_allZeroNoted[2] = false;

    m_plcCtrlFrame = Protocol::blankFrame(Protocol::kCmdPlcCtrl);
    m_networkFrame = Protocol::blankFrame(Protocol::kCmdNetwork);
    for (int i = 0; i < Protocol::kNetCount; ++i) {
        Protocol::setControlPair(m_networkFrame, i, Protocol::kLinkDown);
    }

    m_modbusClient = new QModbusTcpClient(this);
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant(m_host));
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant(m_port));
    m_modbusClient->setTimeout(800);
    m_modbusClient->setNumberOfRetries(1);
    connect(m_modbusClient, &QModbusClient::stateChanged, this, &PlcComm::onModbusStateChanged);
    connect(m_modbusClient, &QModbusClient::errorOccurred, this, &PlcComm::onErrorOccurred);

    m_readTimer = new QTimer(this);
    m_readTimer->setInterval(3000);
    connect(m_readTimer, &QTimer::timeout, this, &PlcComm::periodicRead);

    m_commandPollTimer = new QTimer(this);
    m_commandPollTimer->setInterval(300);
    connect(m_commandPollTimer, &QTimer::timeout, this, &PlcComm::pollPendingCommand);

    m_commandTimeoutTimer = new QTimer(this);
    m_commandTimeoutTimer->setSingleShot(true);
    m_commandTimeoutTimer->setInterval(120000);
    connect(m_commandTimeoutTimer, &QTimer::timeout, this, &PlcComm::onCommandTimeout);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(5000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &PlcComm::attemptReconnection);
}

PlcComm::~PlcComm()
{
    m_manualDisconnect = true;
    if (m_modbusClient) {
        m_modbusClient->disconnectDevice();
    }
}

void PlcComm::log(int level, const QString &text) const
{
    emit const_cast<PlcComm *>(this)->logMessage(level, QStringLiteral("PLC"), text);
}

void PlcComm::start()
{
    log(Protocol::LogInfo,
        QStringLiteral("准备连接 %1:%2 从站=%3。上报地址 42000/42100/42200 每 3 秒只读；"
                       "命令写 42300–42700，回复看字节28。PDU 地址按协议表原值发送。")
            .arg(m_host).arg(m_port).arg(m_slaveId));
    connectToPlc();
}

void PlcComm::connectToPlc()
{
    m_manualDisconnect = false;
    if (m_modbusClient->state() != QModbusDevice::UnconnectedState) {
        log(Protocol::LogWarn, QStringLiteral("已在连接或已连接，忽略重复连接"));
        return;
    }
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant(m_host));
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant(m_port));
    if (!m_modbusClient->connectDevice()) {
        log(Protocol::LogError,
            QStringLiteral("connectDevice 失败：%1").arg(m_modbusClient->errorString()));
    }
}

void PlcComm::disconnectFromPlc()
{
    m_manualDisconnect = true;
    m_reconnectArmed = false;
    m_readTimer->stop();
    m_commandPollTimer->stop();
    m_commandTimeoutTimer->stop();
    m_reconnectTimer->stop();
    m_queue.clear();
    m_inflight = false;
    m_commandPending = false;
    if (m_modbusClient->state() != QModbusDevice::UnconnectedState) {
        m_modbusClient->disconnectDevice();
    } else {
        m_connected.storeRelaxed(0);
        emit connectionStateChanged(false);
        log(Protocol::LogInfo, QStringLiteral("已处于断开状态"));
    }
}

void PlcComm::onModbusStateChanged(QModbusDevice::State state)
{
    const bool connected = (state == QModbusDevice::ConnectedState);
    m_connected.storeRelaxed(connected ? 1 : 0);
    emit connectionStateChanged(connected);
    if (connected) {
        m_reconnectTimer->stop();
        m_reconnectArmed = false;
        m_manualDisconnect = false;
        m_readTimer->start();
        log(Protocol::LogInfo, QStringLiteral("Modbus 已连接，开始 3 秒轮询健康/伺服/电池"));
        periodicRead();
        // 单步调试阶段不把 MQTT/串口链路写成 0x08。网络帧仍保留，恢复链路上报时再入队。
    } else {
        m_readTimer->stop();
        m_commandPollTimer->stop();
        if (!m_manualDisconnect && !m_reconnectArmed) {
            m_reconnectArmed = true;
            m_reconnectTimer->start();
            log(Protocol::LogWarn, QStringLiteral("连接断开，5 秒后重试"));
        } else if (m_manualDisconnect) {
            log(Protocol::LogInfo, QStringLiteral("手动断开，不再自动重连"));
        }
    }
}

void PlcComm::onErrorOccurred(QModbusDevice::Error error)
{
    if (error == QModbusDevice::NoError) {
        return;
    }
    log(Protocol::LogError,
        QStringLiteral("Modbus 设备错误 code=%1：%2")
            .arg(int(error)).arg(m_modbusClient->errorString()));
    if (!m_manualDisconnect && m_modbusClient->state() == QModbusDevice::UnconnectedState
        && !m_reconnectArmed) {
        m_reconnectArmed = true;
        m_reconnectTimer->start();
    }
}

void PlcComm::attemptReconnection()
{
    log(Protocol::LogWarn, QStringLiteral("正在重连 PLC"));
    connectToPlc();
}

void PlcComm::enqueue(const Job &job, bool front)
{
    if (m_queue.size() > 30) {
        log(Protocol::LogError, QStringLiteral("收发队列超过 30，丢弃新请求。PLC 应答过慢或连接异常"));
        return;
    }
    if (front) {
        m_queue.prepend(job);
    } else {
        m_queue.enqueue(job);
    }
    QTimer::singleShot(0, this, &PlcComm::pump);
}

bool PlcComm::queueContainsRead(int address) const
{
    if (m_inflight && m_current.type == JobRead && m_current.address == address) {
        return true;
    }
    for (const Job &job : m_queue) {
        if (job.type == JobRead && job.address == address) {
            return true;
        }
    }
    return false;
}

void PlcComm::periodicRead()
{
    const quint8 cmds[] = {Protocol::kCmdHealth, Protocol::kCmdServo, Protocol::kCmdBattery};
    for (quint8 cmd : cmds) {
        const int address = Protocol::addressOf(cmd);
        if (!queueContainsRead(address)) {
            Job job;
            job.type = JobRead;
            job.address = address;
            job.cmd = cmd;
            enqueue(job, false);
        }
    }
}

void PlcComm::pollPendingCommand()
{
    if (!m_commandPending || queueContainsRead(m_pendingAddress)) {
        return;
    }
    Job job;
    job.type = JobRead;
    job.address = m_pendingAddress;
    job.cmd = m_pendingCmd;
    enqueue(job, true);
}

void PlcComm::pump()
{
    if (m_inflight || m_queue.isEmpty()) {
        return;
    }
    if (m_modbusClient->state() != QModbusDevice::ConnectedState) {
        return;
    }

    m_current = m_queue.dequeue();
    m_inflight = true;

    if (m_current.type == JobWrite) {
        const QVector<quint16> registers = frameToRegisters(m_current.frame);
        QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, m_current.address, registers.size());
        for (int i = 0; i < registers.size(); ++i) {
            unit.setValue(i, registers.at(i));
        }
        log(Protocol::LogInfo,
            QStringLiteral("TX 地址=%1 %2")
                .arg(m_current.address)
                .arg(Protocol::describeTxFrame(m_current.cmd, m_current.frame)));
        QModbusReply *reply = m_modbusClient->sendWriteRequest(unit, m_slaveId);
        if (!reply) {
            m_inflight = false;
            log(Protocol::LogError,
                QStringLiteral("写请求没有发出，地址=%1：%2")
                    .arg(m_current.address).arg(m_modbusClient->errorString()));
            QTimer::singleShot(0, this, &PlcComm::pump);
            return;
        }
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            const bool ok = (reply->error() == QModbusDevice::NoError);
            if (!ok) {
                log(Protocol::LogError,
                    QStringLiteral("写失败 地址=%1 命令=%2：%3")
                        .arg(m_current.address)
                        .arg(Protocol::commandName(m_current.cmd))
                        .arg(reply->errorString()));
            } else {
                log(Protocol::LogTrace,
                    QStringLiteral("写成功 地址=%1 %2").arg(m_current.address).arg(Protocol::commandName(m_current.cmd)));
            }
            const quint8 writtenCmd = m_current.cmd;
            reply->deleteLater();
            finishJob();
            if (!ok && m_commandPending && writtenCmd == m_pendingCmd) {
                m_commandPending = false;
                m_commandPollTimer->stop();
                m_commandTimeoutTimer->stop();
                emit commandFinished(writtenCmd, 0x00, 0, 0,
                                     QStringLiteral("写寄存器失败，命令没有交给 PLC"));
                return;
            }
            if (ok && m_commandPending && writtenCmd == m_pendingCmd) {
                pollPendingCommand();
            }
        });
        return;
    }

    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, m_current.address, Protocol::kRegisterCount);
    QModbusReply *reply = m_modbusClient->sendReadRequest(unit, m_slaveId);
    if (!reply) {
        m_inflight = false;
        log(Protocol::LogError,
            QStringLiteral("读请求没有发出，地址=%1：%2")
                .arg(m_current.address).arg(m_modbusClient->errorString()));
        QTimer::singleShot(0, this, &PlcComm::pump);
        return;
    }
    connect(reply, &QModbusReply::finished, this, [this, reply]() {
        const Job job = m_current;
        if (reply->error() == QModbusDevice::NoError) {
            handleRead(job, registersToFrame(reply->result()));
        } else {
            log(Protocol::LogError,
                QStringLiteral("读失败 地址=%1 %2：%3")
                    .arg(job.address)
                    .arg(Protocol::commandName(job.cmd))
                    .arg(reply->errorString()));
        }
        reply->deleteLater();
        finishJob();
    });
}

void PlcComm::finishJob()
{
    m_inflight = false;
    QTimer::singleShot(0, this, &PlcComm::pump);
}

void PlcComm::handleRead(const Job &job, const QByteArray &frame)
{
    if (frame.size() != Protocol::kFrameBytes) {
        log(Protocol::LogError,
            QStringLiteral("地址=%1 读回 %2 字节，协议要求 32 字节。原始=%3")
                .arg(job.address).arg(frame.size()).arg(Protocol::hexBytes(frame)));
        return;
    }

    const bool allZero = (frame == QByteArray(Protocol::kFrameBytes, 0));
    const int slot = seqSlot(job.cmd);
    if (allZero) {
        if (slot >= 0 && !m_allZeroNoted[slot]) {
            m_allZeroNoted[slot] = true;
            log(Protocol::LogWarn,
                QStringLiteral("地址=%1 %2 读回全 0。请核对 PDU 地址是 42000 还是 41999、从站号=%3、PLC 是否在写该区")
                    .arg(job.address).arg(Protocol::commandName(job.cmd)).arg(m_slaveId));
        }
        return;
    }
    if (slot >= 0) {
        m_allZeroNoted[slot] = false;
    }

    const quint8 device = static_cast<quint8>(frame.at(0));
    const quint8 cmd = static_cast<quint8>(frame.at(1));
    const quint8 status = static_cast<quint8>(frame.at(2));
    const quint8 seq = static_cast<quint8>(frame.at(3));
    // 命令回复示例里字节 1、字节 2 常被留成 0，只填字节 28。0 不当成设备类型错误。
    if (device != Protocol::kDeviceType && device != 0) {
        log(Protocol::LogWarn,
            QStringLiteral("地址=%1 字节1设备类型=0x%2，协议为 0x01。帧=%3")
                .arg(job.address).arg(int(device), 2, 16, QLatin1Char('0')).arg(Protocol::hexBytes(frame)));
    }
    if (cmd != 0 && cmd != job.cmd) {
        log(Protocol::LogError,
            QStringLiteral("地址=%1 期望指令 %2，帧内指令=%3。帧=%4")
                .arg(job.address)
                .arg(Protocol::commandName(job.cmd))
                .arg(Protocol::commandName(cmd))
                .arg(Protocol::hexBytes(frame)));
    }

    if (job.cmd == Protocol::kCmdHealth || job.cmd == Protocol::kCmdServo || job.cmd == Protocol::kCmdBattery) {
        if (status != Protocol::kStatusOk) {
            log(Protocol::LogWarn,
                QStringLiteral("地址=%1 %2 字节3=0x%3，周期上报应为 0x80。帧=%4")
                    .arg(job.address)
                    .arg(Protocol::commandName(job.cmd))
                    .arg(int(status), 2, 16, QLatin1Char('0'))
                    .arg(Protocol::hexBytes(frame)));
        }
        noteSequence(job.cmd, seq, job.address, frame);
    }

    if (job.cmd == Protocol::kCmdHealth) {
        Protocol::HealthStatus health;
        QString error;
        if (!Protocol::parseHealth(frame, &health, &error)) {
            log(Protocol::LogError, error);
            return;
        }
        emit healthUpdated(health);
        return;
    }
    if (job.cmd == Protocol::kCmdServo) {
        Protocol::ServoStatus servo;
        QString error;
        if (!Protocol::parseServo(frame, &servo, &error)) {
            log(Protocol::LogError, error);
            return;
        }
        emit servoUpdated(servo);
        return;
    }
    if (job.cmd == Protocol::kCmdBattery) {
        // 协议表写明电池字节在 Word 里，这里只做丢包判断，不把猜测的位送到界面。
        return;
    }

    handleCommandFrame(job.cmd, job.address, frame);
}

void PlcComm::noteSequence(quint8 cmd, quint8 seq, int address, const QByteArray &frame)
{
    const int slot = seqSlot(cmd);
    if (slot < 0) {
        return;
    }
    if (!m_hasSeq[slot]) {
        m_hasSeq[slot] = true;
        m_lastSeq[slot] = seq;
        m_sameSeqCount[slot] = 1;
        log(Protocol::LogInfo,
            QStringLiteral("RX 地址=%1 %2 首次序号=%3")
                .arg(address).arg(Protocol::commandName(cmd)).arg(int(seq)));
        if (cmd == Protocol::kCmdHealth) {
            Protocol::HealthStatus health;
            if (Protocol::parseHealth(frame, &health, nullptr)) {
                log(Protocol::LogInfo, Protocol::describeHealth(health));
            }
        } else if (cmd == Protocol::kCmdServo) {
            Protocol::ServoStatus servo;
            if (Protocol::parseServo(frame, &servo, nullptr)) {
                log(Protocol::LogInfo, Protocol::describeServo(servo));
            }
        }
        return;
    }

    if (seq == m_lastSeq[slot]) {
        ++m_sameSeqCount[slot];
        // 协议要求连续 2 次及以上序号不变就记录。第 2 次必记，之后每 10 次再记，避免刷屏。
        if (m_sameSeqCount[slot] == 2 || (m_sameSeqCount[slot] % 10) == 0) {
            log(Protocol::LogWarn,
                QStringLiteral("丢包 客户端=%1 组别=%2 地址=%3 包序号=%4 连续不变次数=%5 "
                               "说明=PLC 未写入新数据")
                    .arg(QString::fromUtf8(Protocol::kClientName))
                    .arg(Protocol::commandName(cmd))
                    .arg(address)
                    .arg(int(seq))
                    .arg(m_sameSeqCount[slot]));
        }
        return;
    }

    if (m_sameSeqCount[slot] >= 2) {
        log(Protocol::LogInfo,
            QStringLiteral("地址=%1 %2 包序号恢复 %3 -> %4")
                .arg(address).arg(Protocol::commandName(cmd)).arg(int(m_lastSeq[slot])).arg(int(seq)));
    }
    m_lastSeq[slot] = seq;
    m_sameSeqCount[slot] = 1;
    log(Protocol::LogTrace,
        QStringLiteral("RX 地址=%1 %2 序号=%3 帧=%4")
            .arg(address).arg(Protocol::commandName(cmd)).arg(int(seq)).arg(Protocol::hexBytes(frame)));
    if (cmd == Protocol::kCmdHealth) {
        Protocol::HealthStatus health;
        if (Protocol::parseHealth(frame, &health, nullptr)) {
            log(Protocol::LogTrace, Protocol::describeHealth(health));
        }
    }
}

void PlcComm::handleCommandFrame(quint8 cmd, int address, const QByteArray &frame)
{
    if (!m_commandPending || cmd != m_pendingCmd) {
        log(Protocol::LogTrace,
            QStringLiteral("RX 地址=%1 %2（当前没有等待这条命令）帧=%3")
                .arg(address).arg(Protocol::commandName(cmd)).arg(Protocol::hexBytes(frame)));
        return;
    }

    const Protocol::CommandReply reply = Protocol::parseCommandReply(frame);
    if (!reply.recognized) {
        log(Protocol::LogTrace,
            QStringLiteral("RX 地址=%1 %2 %3 帧=%4")
                .arg(address).arg(Protocol::commandName(cmd), reply.summary, Protocol::hexBytes(frame)));
        return;
    }

    if (reply.status == Protocol::kStatusExecuting) {
        if (!m_seenExecuting) {
            m_seenExecuting = true;
            log(Protocol::LogInfo,
                QStringLiteral("RX 地址=%1 %2 %3")
                    .arg(address).arg(Protocol::commandName(cmd), reply.summary));
        }
        return;
    }

    m_commandPending = false;
    m_commandPollTimer->stop();
    m_commandTimeoutTimer->stop();
    const int level = (reply.status == Protocol::kStatusOk) ? Protocol::LogInfo : Protocol::LogError;
    log(level,
        QStringLiteral("RX 地址=%1 %2 %3 帧=%4")
            .arg(address)
            .arg(Protocol::commandName(cmd))
            .arg(reply.summary)
            .arg(Protocol::hexBytes(frame)));
    emit commandFinished(cmd, reply.status, reply.alarm, reply.fault, reply.summary);
}

void PlcComm::beginCommand(quint8 cmd, const QByteArray &frame)
{
    if (m_commandPending) {
        log(Protocol::LogWarn,
            QStringLiteral("上一条 %1 尚未结束，又发送 %2。PLC 可能以告警 0x03 拒绝")
                .arg(Protocol::commandName(m_pendingCmd), Protocol::commandName(cmd)));
    }
    m_commandPending = true;
    m_pendingCmd = cmd;
    m_pendingAddress = Protocol::addressOf(cmd);
    m_seenExecuting = false;
    m_commandPollTimer->start();
    m_commandTimeoutTimer->start();

    Job job;
    job.type = JobWrite;
    job.address = m_pendingAddress;
    job.cmd = cmd;
    job.frame = frame;
    enqueue(job, true);
}

void PlcComm::onCommandTimeout()
{
    if (!m_commandPending) {
        return;
    }
    const quint8 cmd = m_pendingCmd;
    const int address = m_pendingAddress;
    m_commandPending = false;
    m_commandPollTimer->stop();
    log(Protocol::LogError,
        QStringLiteral("命令超时 地址=%1 %2。120 秒内字节28/字节3都没有出现成功、失败或拒绝。"
                       "若字节28一直是 0 而 PLC 无动作，操作码位置可能仍按流程表放在字节5。")
            .arg(address).arg(Protocol::commandName(cmd)));
    emit commandFinished(cmd, 0x00, 0, 0, QStringLiteral("超时，未收到回复状态"));
}

QByteArray PlcComm::controlFrame(quint8 cmd) const
{
    if (cmd == Protocol::kCmdNetwork) {
        return m_networkFrame;
    }
    return m_plcCtrlFrame;
}

void PlcComm::sendDoor(quint8 opcode)
{
    beginCommand(Protocol::kCmdDoor, Protocol::buildMotionFrame(Protocol::kCmdDoor, opcode, m_planeModel));
}

void PlcComm::sendGripFr(quint8 opcode)
{
    beginCommand(Protocol::kCmdGripFr, Protocol::buildMotionFrame(Protocol::kCmdGripFr, opcode, m_planeModel));
}

void PlcComm::sendGripCh(quint8 opcode)
{
    beginCommand(Protocol::kCmdGripCh, Protocol::buildMotionFrame(Protocol::kCmdGripCh, opcode, m_planeModel));
}

void PlcComm::sendPlcControl(int itemIndex, quint8 opcode)
{
    if (itemIndex < 0 || itemIndex >= Protocol::kCtrlCount) {
        log(Protocol::LogError, QStringLiteral("PLC 控制项序号越界：%1").arg(itemIndex));
        return;
    }
    Protocol::setControlPair(m_plcCtrlFrame, itemIndex, opcode);
    log(Protocol::LogInfo,
        QStringLiteral("PLC控制 %1=%2。其余控制项保持上次写入的值，未操作过的项仍是 0")
            .arg(Protocol::controlItemName(itemIndex))
            .arg(opcode == Protocol::kOpOn ? QStringLiteral("0x01") : QStringLiteral("0x%1").arg(int(opcode), 2, 16, QLatin1Char('0'))));
    beginCommand(Protocol::kCmdPlcCtrl, m_plcCtrlFrame);
}

void PlcComm::sendNetworkLink(int itemIndex, quint8 linkState)
{
    if (itemIndex < 0 || itemIndex >= Protocol::kNetCount) {
        log(Protocol::LogError, QStringLiteral("网络状态项序号越界：%1").arg(itemIndex));
        return;
    }
    Protocol::setControlPair(m_networkFrame, itemIndex, linkState);
    // 0x08 在协议表里只有下发，没有回复行。不占用舱门/归正的等待，避免把正在执行的动作冲掉。
    if (!m_connected.loadRelaxed()) {
        log(Protocol::LogWarn,
            QStringLiteral("PLC 未连接，网络状态先记下，连接后会整帧下发：%1=%2")
                .arg(Protocol::networkItemName(itemIndex))
                .arg(linkState == Protocol::kLinkUp ? QStringLiteral("正常") : QStringLiteral("断联")));
        return;
    }
    Job job;
    job.type = JobWrite;
    job.address = Protocol::kAddrNetwork;
    job.cmd = Protocol::kCmdNetwork;
    job.frame = m_networkFrame;
    log(Protocol::LogInfo,
        QStringLiteral("网络状态更新 %1=%2")
            .arg(Protocol::networkItemName(itemIndex))
            .arg(linkState == Protocol::kLinkUp ? QStringLiteral("正常") : QStringLiteral("断联")));
    enqueue(job, false);
}
