#ifndef PAGE_DEBUG_CONTROL_H
#define PAGE_DEBUG_CONTROL_H

#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QWidget>

class CommunicationManager;

class PageDebugControl : public QWidget
{
    Q_OBJECT
public:
    explicit PageDebugControl(QWidget *parent = nullptr);
    void setCommunicationManager(CommunicationManager *manager);

public slots:
    void onCommandFinished(quint8 cmd, quint8 replyStatus, quint8 alarm, quint8 fault, const QString &summary);

private:
    void sendMotion(quint8 cmd, quint8 opcode);
    void setBusy(bool busy, const QString &text);

    CommunicationManager *m_comm = nullptr;
    QLabel *m_result = nullptr;
    QList<QPushButton *> m_buttons;
    bool m_waiting = false;
    quint8 m_waitingCmd = 0;
};
#endif // PAGE_DEBUG_CONTROL_H
