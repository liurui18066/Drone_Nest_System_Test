#include "page_debug_control.h"

#include "communication_manager.h"
#include "protocol.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QVBoxLayout>

PageDebugControl::PageDebugControl(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    auto *grid = new QGridLayout;
    grid->setSpacing(12);

    auto add = [this, grid](const QString &text, int row, int column, quint8 cmd, quint8 opcode) {
        auto *button = new QPushButton(text);
        button->setMinimumSize(180, 52);
        grid->addWidget(button, row, column);
        m_buttons.append(button);
        connect(button, &QPushButton::clicked, this, [this, cmd, opcode]() {
            sendMotion(cmd, opcode);
        });
    };

    // 操作码写在字节 4。01–04 来自流程表和告警码，主表正文把 01/02 简写成开/关。
    add(QStringLiteral("有飞机开门"), 0, 0, Protocol::kCmdDoor, Protocol::kDoorOpenWithPlane);
    add(QStringLiteral("有飞机关门"), 0, 1, Protocol::kCmdDoor, Protocol::kDoorCloseWithPlane);
    add(QStringLiteral("无飞机开门"), 0, 2, Protocol::kCmdDoor, Protocol::kDoorOpenNoPlane);
    add(QStringLiteral("无飞机关门"), 1, 0, Protocol::kCmdDoor, Protocol::kDoorCloseNoPlane);
    add(QStringLiteral("张开前后归正"), 1, 1, Protocol::kCmdGripFr, Protocol::kGripOpen);
    add(QStringLiteral("夹紧前后归正"), 1, 2, Protocol::kCmdGripFr, Protocol::kGripClose);
    add(QStringLiteral("张开充电归正"), 2, 0, Protocol::kCmdGripCh, Protocol::kGripOpen);
    add(QStringLiteral("夹紧充电归正"), 2, 1, Protocol::kCmdGripCh, Protocol::kGripClose);

    auto *reset = new QPushButton(QStringLiteral("PLC复位"));
    reset->setMinimumSize(180, 52);
    grid->addWidget(reset, 2, 2);
    m_buttons.append(reset);
    connect(reset, &QPushButton::clicked, this, [this]() {
        if (!m_comm) return;
        if (!m_comm->isPlcConnected()) {
            QMessageBox::warning(this, QStringLiteral("PLC"), QStringLiteral("PLC 未连接"));
            return;
        }
        if (m_waiting) return;
        setBusy(true, QStringLiteral("已发送 PLC 复位，等待字节28"));
        m_waitingCmd = Protocol::kCmdPlcCtrl;
        m_comm->sendPlcControl(Protocol::kCtrlReset, Protocol::kOpOn);
    });

    m_result = new QLabel(QStringLiteral("命令结果会显示在这里，详细帧在下方日志。机械动作不会自动重发。"));
    m_result->setWordWrap(true);
    layout->addLayout(grid);
    layout->addWidget(m_result);
    layout->addStretch();
}

void PageDebugControl::setCommunicationManager(CommunicationManager *manager)
{
    m_comm = manager;
    if (!m_comm) return;
    connect(m_comm, &CommunicationManager::commandFinished, this, &PageDebugControl::onCommandFinished);
}

void PageDebugControl::sendMotion(quint8 cmd, quint8 opcode)
{
    if (!m_comm) return;
    if (!m_comm->isPlcConnected()) {
        QMessageBox::warning(this, QStringLiteral("PLC"), QStringLiteral("PLC 未连接"));
        return;
    }
    if (m_waiting) {
        return;
    }
    QString action;
    if (cmd == Protocol::kCmdDoor) action = Protocol::doorOpcodeText(opcode);
    else action = Protocol::commandName(cmd) + QStringLiteral(" ") + Protocol::gripOpcodeText(opcode);
    setBusy(true, QStringLiteral("已发送 %1，等待字节28 的回复").arg(action));
    m_waitingCmd = cmd;
    if (cmd == Protocol::kCmdDoor) m_comm->sendDoor(opcode);
    else if (cmd == Protocol::kCmdGripFr) m_comm->sendGripFr(opcode);
    else if (cmd == Protocol::kCmdGripCh) m_comm->sendGripCh(opcode);
}

void PageDebugControl::setBusy(bool busy, const QString &text)
{
    m_waiting = busy;
    for (QPushButton *button : m_buttons) {
        button->setEnabled(!busy);
    }
    m_result->setText(text);
}

void PageDebugControl::onCommandFinished(quint8 cmd, quint8 replyStatus, quint8 alarm, quint8 fault, const QString &summary)
{
    Q_UNUSED(alarm)
    Q_UNUSED(fault)
    if (!m_waiting || cmd != m_waitingCmd) {
        return;
    }
    setBusy(false, QStringLiteral("%1：%2").arg(Protocol::commandName(cmd), summary));
    if (replyStatus != Protocol::kStatusOk) {
        QMessageBox::warning(this, Protocol::commandName(cmd), summary);
    }
}
