#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>

// 运行参数。缺字段时用协议表里的现场地址，不把旧的 127.0.0.1 再写回去。
struct AppConfig {
    QString plcIp = QStringLiteral("192.168.1.3");
    quint16 plcPort = 502;
    int slaveId = 1;

    bool mqttEnable = false;
    QString mqttHost = QStringLiteral("127.0.0.1");
    quint16 mqttPort = 1883;
    QString mqttClientId = QStringLiteral("drone-nest-hmi");

    // IP 表：风速+雨雪在 ttyS1，两路温湿度在 ttyS3。UPS 的 ttyS0 没有字节协议。
    QString weatherPort = QStringLiteral("/dev/ttyS1");
    QString climatePort = QStringLiteral("/dev/ttyS3");
    int windSlave = 1;
    int rainSlave = 2;
    int indoorSlave = 1;
    int cabinetSlave = 2;
    quint8 planeModel = 0x01;
};

AppConfig loadAppConfig(const QString &filePath, bool *createdDefault = nullptr);
bool saveAppConfig(const AppConfig &config, const QString &filePath);

#endif // APP_CONFIG_H
