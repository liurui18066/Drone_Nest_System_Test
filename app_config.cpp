#include "app_config.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace {

QString elementText(QXmlStreamReader &xml)
{
    return xml.readElementText().trimmed();
}

} // namespace

AppConfig loadAppConfig(const QString &filePath, bool *createdDefault)
{
    AppConfig config;
    if (createdDefault) {
        *createdDefault = false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (createdDefault) {
            *createdDefault = true;
        }
        saveAppConfig(config, filePath);
        return config;
    }

    QXmlStreamReader xml(&file);
    QString section;
    while (!xml.atEnd()) {
        xml.readNext();
        if (!xml.isStartElement()) {
            continue;
        }
        const QString name = xml.name().toString();
        if (name == QLatin1String("config") || name == QLatin1String("plc")
            || name == QLatin1String("mqtt") || name == QLatin1String("serial")) {
            if (name != QLatin1String("config")) {
                section = name;
            }
            continue;
        }
        const QString text = elementText(xml);
        if (section == QLatin1String("plc")) {
            if (name == QLatin1String("ip")) config.plcIp = text;
            else if (name == QLatin1String("port")) config.plcPort = text.toUShort();
            else if (name == QLatin1String("slaveId")) config.slaveId = text.toInt();
        } else if (section == QLatin1String("mqtt")) {
            if (name == QLatin1String("enable")) config.mqttEnable = (text == QLatin1String("1"));
            else if (name == QLatin1String("host")) config.mqttHost = text;
            else if (name == QLatin1String("port")) config.mqttPort = text.toUShort();
            else if (name == QLatin1String("clientId")) config.mqttClientId = text;
        } else if (section == QLatin1String("serial")) {
            if (name == QLatin1String("weatherPort")) config.weatherPort = text;
            else if (name == QLatin1String("climatePort")) config.climatePort = text;
            else if (name == QLatin1String("windSlave")) config.windSlave = text.toInt();
            else if (name == QLatin1String("rainSlave")) config.rainSlave = text.toInt();
            else if (name == QLatin1String("indoorSlave")) config.indoorSlave = text.toInt();
            else if (name == QLatin1String("cabinetSlave")) config.cabinetSlave = text.toInt();
            else if (name == QLatin1String("planeModel")) {
                config.planeModel = static_cast<quint8>(text.toUInt(nullptr, 0));
            }
        }
    }
    if (config.slaveId <= 0 || config.slaveId > 247) {
        config.slaveId = 1;
    }
    if (config.planeModel == 0) {
        config.planeModel = 0x01;
    }
    return config;
}

bool saveAppConfig(const AppConfig &config, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement(QStringLiteral("config"));

    xml.writeStartElement(QStringLiteral("plc"));
    xml.writeTextElement(QStringLiteral("ip"), config.plcIp);
    xml.writeTextElement(QStringLiteral("port"), QString::number(config.plcPort));
    xml.writeTextElement(QStringLiteral("slaveId"), QString::number(config.slaveId));
    xml.writeEndElement();

    xml.writeStartElement(QStringLiteral("mqtt"));
    xml.writeTextElement(QStringLiteral("enable"), config.mqttEnable ? QStringLiteral("1") : QStringLiteral("0"));
    xml.writeTextElement(QStringLiteral("host"), config.mqttHost);
    xml.writeTextElement(QStringLiteral("port"), QString::number(config.mqttPort));
    xml.writeTextElement(QStringLiteral("clientId"), config.mqttClientId);
    xml.writeEndElement();

    xml.writeStartElement(QStringLiteral("serial"));
    xml.writeTextElement(QStringLiteral("weatherPort"), config.weatherPort);
    xml.writeTextElement(QStringLiteral("climatePort"), config.climatePort);
    xml.writeTextElement(QStringLiteral("windSlave"), QString::number(config.windSlave));
    xml.writeTextElement(QStringLiteral("rainSlave"), QString::number(config.rainSlave));
    xml.writeTextElement(QStringLiteral("indoorSlave"), QString::number(config.indoorSlave));
    xml.writeTextElement(QStringLiteral("cabinetSlave"), QString::number(config.cabinetSlave));
    xml.writeTextElement(QStringLiteral("planeModel"),
                         QStringLiteral("0x%1").arg(config.planeModel, 2, 16, QLatin1Char('0')));
    xml.writeEndElement();

    xml.writeEndElement();
    xml.writeEndDocument();
    return true;
}
