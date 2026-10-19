#ifndef WEATHER_COMM_H
#define WEATHER_COMM_H

#include "app_config.h"

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QElapsedTimer>

// 气象站不在 PLC 的 32 字节协议里。端口和站号来自协议表「IP地址」：
// ttyS1 上站号 1 风速、站号 2 雨雪；ttyS3 上站号 1 设备内温湿度、站号 2 电柜温湿度。
// 功能码和比例沿用原先仪表实现（风速/雨雪 0x03，温湿度 0x04，数值除以 10），表里没有这部分。
class WeatherComm : public QObject
{
    Q_OBJECT
public:
    struct Reading {
        bool windValid = false;
        double windSpeed = 0;
        bool rainValid = false;
        int rain = -1;
        bool indoorValid = false;
        double indoorTemp = 0;
        double indoorHum = 0;
        bool cabinetValid = false;
        double cabinetTemp = 0;
        double cabinetHum = 0;
        bool windRainLink = false;
        bool climateLink = false;
    };

    explicit WeatherComm(const AppConfig &config, QObject *parent = nullptr);
    ~WeatherComm();

public slots:
    void start();
    void stop();

signals:
    void readingUpdated(WeatherComm::Reading reading);
    void logMessage(int level, const QString &category, const QString &text);

private slots:
    void pollWeather();
    void pollClimate();
    void onWeatherReady();
    void onClimateReady();

private:
    struct PortState {
        QSerialPort *port = nullptr;
        QByteArray buffer;
        QElapsedTimer windOk;
        QElapsedTimer rainOk;
        QElapsedTimer indoorOk;
        QElapsedTimer cabinetOk;
        bool windSeen = false;
        bool rainSeen = false;
        bool indoorSeen = false;
        bool cabinetSeen = false;
        int phase = 0;
    };

    void openPort(QSerialPort *port, const QString &name);
    void writeRequest(QSerialPort *port, const QByteArray &pdu, const QString &what);
    void consume(QSerialPort *port, QByteArray &buffer, bool weatherBus);
    void parseFrame(const QByteArray &frame, bool weatherBus);
    void refreshLinks();
    static QByteArray crc16(const QByteArray &data);

    AppConfig m_config;
    PortState m_weather;
    PortState m_climate;
    QTimer *m_weatherTimer = nullptr;
    QTimer *m_climateTimer = nullptr;
    Reading m_reading;
    bool m_running = false;
};

Q_DECLARE_METATYPE(WeatherComm::Reading)
#endif // WEATHER_COMM_H
