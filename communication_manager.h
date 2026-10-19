#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "app_config.h"
#include "protocol.h"
#include "weather_comm.h"

#include <QObject>
#include <QThread>

class PlcComm;
class MqttComm;

class CommunicationManager : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationManager(QObject *parent = nullptr);
    ~CommunicationManager();

    void startAll(const AppConfig &config);
    bool isPlcConnected() const;

signals:
    void plcConnectionChanged(bool connected);
    void mqttConnectionChanged(bool connected);
    void healthUpdated(Protocol::HealthStatus status);
    void servoUpdated(Protocol::ServoStatus status);
    void weatherUpdated(WeatherComm::Reading reading);
    void commandFinished(quint8 cmd, quint8 replyStatus, quint8 alarm, quint8 fault, QString summary);
    void logMessage(int level, const QString &category, const QString &text);

public slots:
    void sendDoor(quint8 opcode);
    void sendGripFr(quint8 opcode);
    void sendGripCh(quint8 opcode);
    void sendPlcControl(int itemIndex, quint8 opcode);
    void reconnectPlc(const QString &host, quint16 port);
    void disconnectPlc();

private:
    void stopPlc();
    void startPlc();
    void startWeather();
    void startMqtt();
    void bindPlc();
    void pushNetwork(int itemIndex, bool up);

    AppConfig m_config;
    QThread *m_plcThread = nullptr;
    PlcComm *m_plc = nullptr;
    QThread *m_weatherThread = nullptr;
    WeatherComm *m_weather = nullptr;
    MqttComm *m_mqtt = nullptr;
    bool m_windRainUp = false;
    bool m_climateUp = false;
};
#endif // COMMUNICATION_MANAGER_H
