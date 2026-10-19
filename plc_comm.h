#ifndef PLC_COMM_H
#define PLC_COMM_H

#include "protocol.h"

#include <QObject>
#include <QModbusTcpClient>
#include <QQueue>
#include <QTimer>
#include <QAtomicInt>

class PlcComm : public QObject
{
    Q_OBJECT
public:
    explicit PlcComm(const QString &host, int port, int slaveId, quint8 planeModel,
                     QObject *parent = nullptr);
    ~PlcComm();

    bool isConnected() const { return m_connected.loadRelaxed() != 0; }

public slots:
    void start();
    void disconnectFromPlc();
    void sendDoor(quint8 opcode);
    void sendGripFr(quint8 opcode);
    void sendGripCh(quint8 opcode);
    void sendPlcControl(int itemIndex, quint8 opcode);
    void sendNetworkLink(int itemIndex, quint8 linkState);

signals:
    void connectionStateChanged(bool connected);
    void healthUpdated(Protocol::HealthStatus status);
    void servoUpdated(Protocol::ServoStatus status);
    void commandFinished(quint8 cmd, quint8 replyStatus, quint8 alarm, quint8 fault, QString summary);
    void logMessage(int level, const QString &category, const QString &text);

private slots:
    void onModbusStateChanged(QModbusDevice::State state);
    void onErrorOccurred(QModbusDevice::Error error);
    void periodicRead();
    void pollPendingCommand();
    void onCommandTimeout();
    void attemptReconnection();
    void pump();

private:
    enum JobType { JobRead, JobWrite };

    struct Job {
        JobType type = JobRead;
        int address = 0;
        quint8 cmd = 0;
        QByteArray frame;
    };

    void connectToPlc();
    void enqueue(const Job &job, bool front);
    bool queueContainsRead(int address) const;
    void finishJob();
    void handleRead(const Job &job, const QByteArray &frame);
    void handleCommandFrame(quint8 cmd, int address, const QByteArray &frame);
    void noteSequence(quint8 cmd, quint8 seq, int address, const QByteArray &frame);
    void beginCommand(quint8 cmd, const QByteArray &frame);
    void log(int level, const QString &text) const;
    QByteArray controlFrame(quint8 cmd) const;

    QModbusTcpClient *m_modbusClient;
    QString m_host;
    int m_port;
    int m_slaveId;
    quint8 m_planeModel;
    QAtomicInt m_connected;

    QTimer *m_readTimer;
    QTimer *m_commandPollTimer;
    QTimer *m_commandTimeoutTimer;
    QTimer *m_reconnectTimer;
    bool m_manualDisconnect = false;
    bool m_reconnectArmed = false;
    bool m_inflight = false;

    QQueue<Job> m_queue;
    Job m_current;

    QByteArray m_plcCtrlFrame;
    QByteArray m_networkFrame;

    bool m_commandPending = false;
    quint8 m_pendingCmd = 0;
    int m_pendingAddress = 0;
    bool m_seenExecuting = false;

    quint8 m_lastSeq[3];
    bool m_hasSeq[3];
    int m_sameSeqCount[3];
    bool m_allZeroNoted[3];
};
#endif // PLC_COMM_H
