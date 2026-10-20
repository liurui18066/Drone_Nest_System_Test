#ifndef PAGE_DEVICE_STATUS_H
#define PAGE_DEVICE_STATUS_H

#include "protocol.h"
#include "weather_comm.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class IndicatorLight : public QLabel
{
public:
    explicit IndicatorLight(QWidget *parent = nullptr);
    void setState(bool on);
};

class PageDeviceStatus : public QWidget
{
    Q_OBJECT
public:
    explicit PageDeviceStatus(QWidget *parent = nullptr);

    void setPlcIp(const QString &ip);
    void setPlcPort(quint16 port);
    QString plcIp() const;
    quint16 plcPort() const;
    void setMqttClientId(const QString &clientId);

signals:
    void connectPlcRequested(const QString &ip, quint16 port);
    void closePlcRequested();

public slots:
    void setPlcConnected(bool connected);
    void setMqttConnected(bool connected);
    void updateHealth(const Protocol::HealthStatus &status);
    void updateServo(const Protocol::ServoStatus &status);
    void updateWeather(const WeatherComm::Reading &reading);

private:
    IndicatorLight *m_plcLight = nullptr;
    IndicatorLight *m_mqttLight = nullptr;
    IndicatorLight *m_windLight = nullptr;
    IndicatorLight *m_climateLight = nullptr;
    IndicatorLight *m_upsLight = nullptr;
    IndicatorLight *m_doorOpen = nullptr;
    IndicatorLight *m_doorClose = nullptr;
    IndicatorLight *m_frOpen = nullptr;
    IndicatorLight *m_frClose = nullptr;
    IndicatorLight *m_chOpen = nullptr;
    IndicatorLight *m_chClose = nullptr;
    IndicatorLight *m_paddleA = nullptr;
    IndicatorLight *m_paddleB = nullptr;
    IndicatorLight *m_estop = nullptr;
    IndicatorLight *m_smoke = nullptr;
    IndicatorLight *m_plane = nullptr;
    IndicatorLight *m_fan = nullptr;
    IndicatorLight *m_cabinetFan = nullptr;
    IndicatorLight *m_ac = nullptr;
    IndicatorLight *m_charger = nullptr;
    IndicatorLight *m_qr = nullptr;
    QLabel *m_mode = nullptr;
    QLabel *m_fault = nullptr;
    QLineEdit *m_plcIp = nullptr;
    QLineEdit *m_plcPort = nullptr;
    QPushButton *m_connect = nullptr;
    QPushButton *m_close = nullptr;
    QWidget *m_fanPill = nullptr;
    QLabel *m_fanOff = nullptr;
    QLabel *m_fanOn = nullptr;
    QLabel *m_deviceTemp = nullptr;
    QLabel *m_deviceHum = nullptr;
    QLabel *m_cabinetTemp = nullptr;
    QLabel *m_cabinetHum = nullptr;
    QLabel *m_windValue = nullptr;
    QLabel *m_rainValue = nullptr;
    QLabel *m_upsSupply = nullptr;
    QLabel *m_upsRectifier = nullptr;
    QLabel *m_upsCharge = nullptr;
    QLabel *m_upsRunning = nullptr;
    QLabel *m_upsBackup = nullptr;
    QLabel *m_mqttId = nullptr;
    QLabel *m_mqttHeartbeat = nullptr;
    QLabel *m_mqttSensor = nullptr;
    QLabel *m_publishDot = nullptr;
    QLabel *m_receiveDot = nullptr;
    QLineEdit *m_torque = nullptr;
    QLineEdit *m_position = nullptr;
    QLineEdit *m_servoTime = nullptr;
};

#endif // PAGE_DEVICE_STATUS_H
