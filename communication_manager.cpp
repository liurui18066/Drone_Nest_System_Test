#include "communication_manager.h"

#include "mqtt_comm.h"
#include "plc_comm.h"

#include <QMetaObject>

CommunicationManager::CommunicationManager(QObject *parent)
    : QObject(parent)
{
}

CommunicationManager::~CommunicationManager()
{
    if (m_mqtt) {
        m_mqtt->disconnectFromBroker();
    }
    if (m_weatherThread && m_weatherThread->isRunning()) {
        QMetaObject::invokeMethod(m_weather, "stop", Qt::BlockingQueuedConnection);
        m_weatherThread->quit();
        m_weatherThread->wait(3000);
    }
    stopPlc();
}

void CommunicationManager::startAll(const AppConfig &config)
{
    m_config = config;
    stopPlc();
    if (m_weatherThread) {
        if (m_weatherThread->isRunning()) {
            QMetaObject::invokeMethod(m_weather, "stop", Qt::BlockingQueuedConnection);
            m_weatherThread->quit();
            m_weatherThread->wait(3000);
        }
        delete m_weatherThread;
        m_weatherThread = nullptr;
        m_weather = nullptr;
    }
    if (m_mqtt) {
        m_mqtt->disconnectFromBroker();
        m_mqtt->deleteLater();
        m_mqtt = nullptr;
    }
    startPlc();
    // 当前只做 PLC 收发和单步调试。MQTT、风速雨雪、温湿度串口先不启动。
    emit logMessage(Protocol::LogInfo, QStringLiteral("系统"),
                    QStringLiteral("当前只调试 PLC。MQTT 和气象/温湿度串口已暂停，连接后不写 42700 网络状态。"));
}

bool CommunicationManager::isPlcConnected() const
{
    return m_plc && m_plc->isConnected();
}

void CommunicationManager::stopPlc()
{
    if (!m_plcThread) {
        return;
    }
    if (m_plcThread->isRunning()) {
        QMetaObject::invokeMethod(m_plc, "disconnectFromPlc", Qt::BlockingQueuedConnection);
        m_plcThread->quit();
        m_plcThread->wait(3000);
    }
    delete m_plcThread;
    m_plcThread = nullptr;
    m_plc = nullptr;
}

void CommunicationManager::startPlc()
{
    m_plcThread = new QThread(this);
    m_plc = new PlcComm(m_config.plcIp, m_config.plcPort, m_config.slaveId, m_config.planeModel);
    m_plc->moveToThread(m_plcThread);
    connect(m_plcThread, &QThread::started, m_plc, &PlcComm::start);
    connect(m_plcThread, &QThread::finished, m_plc, &QObject::deleteLater);
    bindPlc();
    m_plcThread->start();
}

void CommunicationManager::bindPlc()
{
    connect(m_plc, &PlcComm::connectionStateChanged, this, &CommunicationManager::plcConnectionChanged);
    connect(m_plc, &PlcComm::healthUpdated, this, &CommunicationManager::healthUpdated);
    connect(m_plc, &PlcComm::servoUpdated, this, &CommunicationManager::servoUpdated);
    connect(m_plc, &PlcComm::commandFinished, this, &CommunicationManager::commandFinished);
    connect(m_plc, &PlcComm::logMessage, this, &CommunicationManager::logMessage);
}

void CommunicationManager::startWeather()
{
    m_weatherThread = new QThread(this);
    m_weather = new WeatherComm(m_config);
    m_weather->moveToThread(m_weatherThread);
    connect(m_weatherThread, &QThread::started, m_weather, &WeatherComm::start);
    connect(m_weatherThread, &QThread::finished, m_weather, &QObject::deleteLater);
    connect(m_weather, &WeatherComm::logMessage, this, &CommunicationManager::logMessage);
    connect(m_weather, &WeatherComm::readingUpdated, this, [this](const WeatherComm::Reading &reading) {
        emit weatherUpdated(reading);
        if (reading.windRainLink != m_windRainUp) {
            m_windRainUp = reading.windRainLink;
            pushNetwork(Protocol::kNetWindRain, m_windRainUp);
        }
        if (reading.climateLink != m_climateUp) {
            m_climateUp = reading.climateLink;
            pushNetwork(Protocol::kNetTempHum, m_climateUp);
        }
    });
    m_weatherThread->start();
}

void CommunicationManager::startMqtt()
{
    if (!m_config.mqttEnable) {
        emit logMessage(Protocol::LogInfo, QStringLiteral("MQTT"),
                        QStringLiteral("config.xml 中 mqtt/enable=0，不连接 Broker。0x08 字节4按断联下发。"));
        pushNetwork(Protocol::kNetMqtt, false);
        return;
    }
    m_mqtt = new MqttComm(m_config.mqttHost, m_config.mqttPort, m_config.mqttClientId, this);
    connect(m_mqtt, &MqttComm::connectionStateChanged, this, [this](bool connected) {
        emit mqttConnectionChanged(connected);
        emit logMessage(connected ? Protocol::LogInfo : Protocol::LogWarn, QStringLiteral("MQTT"),
                        connected ? QStringLiteral("已连接 Broker") : QStringLiteral("与 Broker 断开"));
        pushNetwork(Protocol::kNetMqtt, connected);
    });
    connect(m_mqtt, &MqttComm::errorOccurred, this, [this](const QString &error) {
        emit logMessage(Protocol::LogError, QStringLiteral("MQTT"), error);
    });
    connect(m_mqtt, &MqttComm::messageReceived, this, [this](const QString &topic, const QByteArray &payload) {
        emit logMessage(Protocol::LogInfo, QStringLiteral("MQTT"),
                        QStringLiteral("收到 topic=%1 长度=%2。协议表没有定义主题含义，内容不转成 PLC 命令。")
                            .arg(topic).arg(payload.size()));
    });
    m_mqtt->start();
}

void CommunicationManager::pushNetwork(int itemIndex, bool up)
{
    if (!m_plc) {
        return;
    }
    const quint8 state = up ? Protocol::kLinkUp : Protocol::kLinkDown;
    QMetaObject::invokeMethod(m_plc, "sendNetworkLink", Qt::QueuedConnection,
                              Q_ARG(int, itemIndex), Q_ARG(quint8, state));
}

void CommunicationManager::sendDoor(quint8 opcode)
{
    if (!m_plc) return;
    QMetaObject::invokeMethod(m_plc, "sendDoor", Qt::QueuedConnection, Q_ARG(quint8, opcode));
}

void CommunicationManager::sendGripFr(quint8 opcode)
{
    if (!m_plc) return;
    QMetaObject::invokeMethod(m_plc, "sendGripFr", Qt::QueuedConnection, Q_ARG(quint8, opcode));
}

void CommunicationManager::sendGripCh(quint8 opcode)
{
    if (!m_plc) return;
    QMetaObject::invokeMethod(m_plc, "sendGripCh", Qt::QueuedConnection, Q_ARG(quint8, opcode));
}

void CommunicationManager::sendPlcControl(int itemIndex, quint8 opcode)
{
    if (!m_plc) return;
    QMetaObject::invokeMethod(m_plc, "sendPlcControl", Qt::QueuedConnection,
                              Q_ARG(int, itemIndex), Q_ARG(quint8, opcode));
}

void CommunicationManager::reconnectPlc(const QString &host, quint16 port)
{
    m_config.plcIp = host;
    m_config.plcPort = port;
    stopPlc();
    startPlc();
    emit logMessage(Protocol::LogInfo, QStringLiteral("系统"),
                    QStringLiteral("按新地址重连 PLC %1:%2 从站=%3").arg(host).arg(port).arg(m_config.slaveId));
}

void CommunicationManager::disconnectPlc()
{
    if (!m_plc) {
        emit logMessage(Protocol::LogWarn, QStringLiteral("PLC"), QStringLiteral("通信对象不存在，无法断开"));
        return;
    }
    QMetaObject::invokeMethod(m_plc, "disconnectFromPlc", Qt::QueuedConnection);
}
