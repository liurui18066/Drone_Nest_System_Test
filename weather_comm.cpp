#include "weather_comm.h"

#include "protocol.h"

namespace {

QByteArray rtuRequest(quint8 slave, quint8 func, quint16 address, quint16 count)
{
    QByteArray pdu;
    pdu.append(static_cast<char>(slave));
    pdu.append(static_cast<char>(func));
    pdu.append(static_cast<char>(address >> 8));
    pdu.append(static_cast<char>(address & 0xFF));
    pdu.append(static_cast<char>(count >> 8));
    pdu.append(static_cast<char>(count & 0xFF));
    return pdu;
}

} // namespace

WeatherComm::WeatherComm(const AppConfig &config, QObject *parent)
    : QObject(parent)
    , m_config(config)
{
}

WeatherComm::~WeatherComm()
{
    stop();
}

void WeatherComm::start()
{
    if (m_running) {
        return;
    }
    m_running = true;
    m_weather.port = new QSerialPort(this);
    m_climate.port = new QSerialPort(this);
    connect(m_weather.port, &QSerialPort::readyRead, this, &WeatherComm::onWeatherReady);
    connect(m_climate.port, &QSerialPort::readyRead, this, &WeatherComm::onClimateReady);

    openPort(m_weather.port, m_config.weatherPort);
    openPort(m_climate.port, m_config.climatePort);

    m_weatherTimer = new QTimer(this);
    m_climateTimer = new QTimer(this);
    m_weatherTimer->setInterval(400);
    m_climateTimer->setInterval(400);
    connect(m_weatherTimer, &QTimer::timeout, this, &WeatherComm::pollWeather);
    connect(m_climateTimer, &QTimer::timeout, this, &WeatherComm::pollClimate);
    if (m_weather.port->isOpen()) {
        m_weatherTimer->start();
    }
    if (m_climate.port->isOpen()) {
        m_climateTimer->start();
    }
    emit logMessage(Protocol::LogInfo, QStringLiteral("串口"),
                    QStringLiteral("风速站号=%1 雨雪站号=%2 端口=%3；设备内温湿度站号=%4 电柜温湿度站号=%5 端口=%6。"
                                   "功能码沿用原仪表：风速/雨雪 03，温湿度 04。UPS %7 无字节协议，链路保持断联。")
                        .arg(m_config.windSlave).arg(m_config.rainSlave).arg(m_config.weatherPort)
                        .arg(m_config.indoorSlave).arg(m_config.cabinetSlave).arg(m_config.climatePort)
                        .arg(QStringLiteral("/dev/ttyS0")));
}

void WeatherComm::stop()
{
    m_running = false;
    if (m_weatherTimer) m_weatherTimer->stop();
    if (m_climateTimer) m_climateTimer->stop();
    if (m_weather.port && m_weather.port->isOpen()) m_weather.port->close();
    if (m_climate.port && m_climate.port->isOpen()) m_climate.port->close();
}

void WeatherComm::openPort(QSerialPort *port, const QString &name)
{
    port->setPortName(name);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    if (!port->open(QIODevice::ReadWrite)) {
        emit logMessage(Protocol::LogError, QStringLiteral("串口"),
                        QStringLiteral("打开 %1 失败：%2。确认设备名、用户在 dialout 组，且该口没有 serial-getty。")
                            .arg(name, port->errorString()));
        return;
    }
    emit logMessage(Protocol::LogInfo, QStringLiteral("串口"),
                    QStringLiteral("已打开 %1，9600 8N1").arg(name));
}

QByteArray WeatherComm::crc16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data.at(i));
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    QByteArray out;
    out.append(static_cast<char>(crc & 0xFF));
    out.append(static_cast<char>((crc >> 8) & 0xFF));
    return out;
}

void WeatherComm::writeRequest(QSerialPort *port, const QByteArray &pdu, const QString &what)
{
    if (!port || !port->isOpen()) {
        return;
    }
    QByteArray frame = pdu;
    frame.append(crc16(pdu));
    const qint64 n = port->write(frame);
    if (n != frame.size()) {
        emit logMessage(Protocol::LogError, QStringLiteral("串口"),
                        QStringLiteral("TX %1 失败：%2").arg(what, port->errorString()));
        return;
    }
    emit logMessage(Protocol::LogTrace, QStringLiteral("串口"),
                    QStringLiteral("TX %1 %2 帧=%3").arg(port->portName(), what, Protocol::hexBytes(frame)));
}

void WeatherComm::pollWeather()
{
    if (m_weather.phase == 0) {
        writeRequest(m_weather.port,
                     rtuRequest(static_cast<quint8>(m_config.windSlave), 0x03, 0, 1),
                     QStringLiteral("风速"));
        m_weather.phase = 1;
    } else {
        writeRequest(m_weather.port,
                     rtuRequest(static_cast<quint8>(m_config.rainSlave), 0x03, 0, 1),
                     QStringLiteral("雨雪"));
        m_weather.phase = 0;
    }
    refreshLinks();
}

void WeatherComm::pollClimate()
{
    const int slave = (m_climate.phase == 0) ? m_config.indoorSlave : m_config.cabinetSlave;
    const QString what = (m_climate.phase == 0) ? QStringLiteral("设备内温湿度") : QStringLiteral("电柜温湿度");
    writeRequest(m_climate.port, rtuRequest(static_cast<quint8>(slave), 0x04, 0, 2), what);
    m_climate.phase = (m_climate.phase == 0) ? 1 : 0;
    refreshLinks();
}

void WeatherComm::onWeatherReady()
{
    m_weather.buffer.append(m_weather.port->readAll());
    consume(m_weather.port, m_weather.buffer, true);
}

void WeatherComm::onClimateReady()
{
    m_climate.buffer.append(m_climate.port->readAll());
    consume(m_climate.port, m_climate.buffer, false);
}

void WeatherComm::consume(QSerialPort *port, QByteArray &buffer, bool weatherBus)
{
    const int slaveA = weatherBus ? m_config.windSlave : m_config.indoorSlave;
    const int slaveB = weatherBus ? m_config.rainSlave : m_config.cabinetSlave;
    while (!buffer.isEmpty()) {
        const int station = static_cast<quint8>(buffer.at(0));
        if (station != slaveA && station != slaveB) {
            emit logMessage(Protocol::LogWarn, QStringLiteral("串口"),
                            QStringLiteral("RX %1 丢弃非本总线站号 0x%2")
                                .arg(port->portName()).arg(station, 2, 16, QLatin1Char('0')));
            buffer.remove(0, 1);
            continue;
        }
        if (buffer.size() < 5) {
            return;
        }
        const quint8 func = static_cast<quint8>(buffer.at(1));
        if (func != 0x03 && func != 0x04) {
            emit logMessage(Protocol::LogWarn, QStringLiteral("串口"),
                            QStringLiteral("RX %1 站号=%2 功能码=0x%3 无法识别")
                                .arg(port->portName()).arg(station).arg(int(func), 2, 16, QLatin1Char('0')));
            buffer.remove(0, 1);
            continue;
        }
        const int byteCount = static_cast<quint8>(buffer.at(2));
        const int frameLength = 3 + byteCount + 2;
        if (buffer.size() < frameLength) {
            return;
        }
        const QByteArray frame = buffer.left(frameLength);
        buffer.remove(0, frameLength);
        const QByteArray body = frame.left(frame.size() - 2);
        const QByteArray expect = crc16(body);
        if (expect != frame.right(2)) {
            emit logMessage(Protocol::LogError, QStringLiteral("串口"),
                            QStringLiteral("RX %1 CRC 错误 帧=%2 期望CRC=%3")
                                .arg(port->portName(), Protocol::hexBytes(frame), Protocol::hexBytes(expect)));
            continue;
        }
        parseFrame(frame, weatherBus);
    }
}

void WeatherComm::parseFrame(const QByteArray &frame, bool weatherBus)
{
    const int station = static_cast<quint8>(frame.at(0));
    const quint8 func = static_cast<quint8>(frame.at(1));
    emit logMessage(Protocol::LogTrace, QStringLiteral("串口"),
                    QStringLiteral("RX 站号=%1 功能码=0x%2 帧=%3")
                        .arg(station).arg(int(func), 2, 16, QLatin1Char('0')).arg(Protocol::hexBytes(frame)));

    if (weatherBus && station == m_config.windSlave && func == 0x03 && frame.size() >= 7) {
        const quint16 raw = (static_cast<quint8>(frame.at(3)) << 8) | static_cast<quint8>(frame.at(4));
        m_reading.windValid = true;
        m_reading.windSpeed = raw / 10.0;
        m_weather.windSeen = true;
        m_weather.windOk.restart();
    } else if (weatherBus && station == m_config.rainSlave && func == 0x03 && frame.size() >= 7) {
        const quint16 raw = (static_cast<quint8>(frame.at(3)) << 8) | static_cast<quint8>(frame.at(4));
        m_reading.rainValid = true;
        m_reading.rain = (raw == 0x0001) ? 1 : (raw == 0x0000 ? 0 : -1);
        m_weather.rainSeen = true;
        m_weather.rainOk.restart();
        if (m_reading.rain < 0) {
            emit logMessage(Protocol::LogWarn, QStringLiteral("串口"),
                            QStringLiteral("雨雪原始值=%1，既不是 0 也不是 1").arg(raw));
        }
    } else if (!weatherBus && func == 0x04 && frame.size() >= 9) {
        const quint16 tempRaw = (static_cast<quint8>(frame.at(3)) << 8) | static_cast<quint8>(frame.at(4));
        const quint16 humRaw = (static_cast<quint8>(frame.at(5)) << 8) | static_cast<quint8>(frame.at(6));
        const double temp = (tempRaw < 10000) ? tempRaw * 0.1 : -(tempRaw - 10000) * 0.1;
        const double hum = humRaw * 0.1;
        if (station == m_config.indoorSlave) {
            m_reading.indoorValid = true;
            m_reading.indoorTemp = temp;
            m_reading.indoorHum = hum;
            m_climate.indoorSeen = true;
            m_climate.indoorOk.restart();
        } else if (station == m_config.cabinetSlave) {
            m_reading.cabinetValid = true;
            m_reading.cabinetTemp = temp;
            m_reading.cabinetHum = hum;
            m_climate.cabinetSeen = true;
            m_climate.cabinetOk.restart();
        }
    } else {
        emit logMessage(Protocol::LogWarn, QStringLiteral("串口"),
                        QStringLiteral("帧长度或功能码与请求不匹配 站号=%1 帧=%2")
                            .arg(station).arg(Protocol::hexBytes(frame)));
    }
    refreshLinks();
    emit readingUpdated(m_reading);
}

void WeatherComm::refreshLinks()
{
    const bool windOk = m_weather.windSeen && m_weather.windOk.isValid() && m_weather.windOk.elapsed() < 3000;
    const bool rainOk = m_weather.rainSeen && m_weather.rainOk.isValid() && m_weather.rainOk.elapsed() < 3000;
    const bool indoorOk = m_climate.indoorSeen && m_climate.indoorOk.isValid() && m_climate.indoorOk.elapsed() < 3000;
    const bool cabinetOk = m_climate.cabinetSeen && m_climate.cabinetOk.isValid() && m_climate.cabinetOk.elapsed() < 3000;
    const bool windRain = windOk && rainOk;
    const bool climate = indoorOk && cabinetOk;
    if (windRain != m_reading.windRainLink) {
        emit logMessage(windRain ? Protocol::LogInfo : Protocol::LogWarn, QStringLiteral("串口"),
                        windRain ? QStringLiteral("风速和雨雪都在 3 秒内有有效帧")
                                 : QStringLiteral("风速或雨雪超过 3 秒没有有效帧，链路按断联上报"));
    }
    if (climate != m_reading.climateLink) {
        emit logMessage(climate ? Protocol::LogInfo : Protocol::LogWarn, QStringLiteral("串口"),
                        climate ? QStringLiteral("两路温湿度都在 3 秒内有有效帧")
                                : QStringLiteral("温湿度超过 3 秒没有有效帧，链路按断联上报"));
    }
    m_reading.windRainLink = windRain;
    m_reading.climateLink = climate;
}
