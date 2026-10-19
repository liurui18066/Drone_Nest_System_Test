#include "page_device_status.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QStringList>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>

namespace {

QWidget *lightRow(const QString &text, IndicatorLight *light)
{
    QWidget *row = new QWidget;
    auto *layout = new QHBoxLayout(row);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new QLabel(text));
    layout->addWidget(light);
    layout->addStretch();
    return row;
}

} // namespace

IndicatorLight::IndicatorLight(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(18, 18);
    setState(false);
}

void IndicatorLight::setState(bool on)
{
    // 绿色表示条件成立。急停、烟雾在界面上反过来用：亮起表示异常。
    setStyleSheet(QStringLiteral("background-color:%1; border-radius:9px;")
                      .arg(on ? QStringLiteral("#2e7d32") : QStringLiteral("#9e9e9e")));
}

PageDeviceStatus::PageDeviceStatus(QWidget *parent)
    : QWidget(parent)
{
    m_plcLight = new IndicatorLight;
    m_mqttLight = new IndicatorLight;
    m_windLight = new IndicatorLight;
    m_climateLight = new IndicatorLight;
    m_doorOpen = new IndicatorLight;
    m_doorClose = new IndicatorLight;
    m_frOpen = new IndicatorLight;
    m_frClose = new IndicatorLight;
    m_chOpen = new IndicatorLight;
    m_chClose = new IndicatorLight;
    m_paddleA = new IndicatorLight;
    m_paddleB = new IndicatorLight;
    m_estop = new IndicatorLight;
    m_smoke = new IndicatorLight;
    m_plane = new IndicatorLight;
    m_fan = new IndicatorLight;
    m_cabinetFan = new IndicatorLight;
    m_ac = new IndicatorLight;
    m_charger = new IndicatorLight;
    m_qr = new IndicatorLight;

    m_plcIp = new QLineEdit(QStringLiteral("192.168.1.3"));
    m_plcPort = new QLineEdit(QStringLiteral("502"));
    m_plcPort->setMaximumWidth(80);
    m_connect = new QPushButton(QStringLiteral("连接"));
    m_close = new QPushButton(QStringLiteral("断开"));
    m_close->setEnabled(false);
    m_mode = new QLabel(QStringLiteral("PLC状态：等待数据"));
    m_fault = new QLabel(QStringLiteral("故障/告警：等待数据"));
    m_fault->setWordWrap(true);

    auto *netBox = new QGroupBox(QStringLiteral("链路"));
    auto *netLayout = new QVBoxLayout(netBox);
    netLayout->addWidget(lightRow(QStringLiteral("PLC"), m_plcLight));
    netLayout->addWidget(lightRow(QStringLiteral("MQTT"), m_mqttLight));
    netLayout->addWidget(lightRow(QStringLiteral("风速雨雪"), m_windLight));
    netLayout->addWidget(lightRow(QStringLiteral("温湿度"), m_climateLight));
    auto *addr = new QHBoxLayout;
    addr->addWidget(m_plcIp);
    addr->addWidget(m_plcPort);
    netLayout->addLayout(addr);
    auto *buttons = new QHBoxLayout;
    buttons->addWidget(m_connect);
    buttons->addWidget(m_close);
    netLayout->addLayout(buttons);

    auto *mechBox = new QGroupBox(QStringLiteral("机构（字节20）"));
    auto *mechLayout = new QVBoxLayout(mechBox);
    mechLayout->addWidget(lightRow(QStringLiteral("顶门开启"), m_doorOpen));
    mechLayout->addWidget(lightRow(QStringLiteral("顶门关闭"), m_doorClose));
    mechLayout->addWidget(lightRow(QStringLiteral("前后张开"), m_frOpen));
    mechLayout->addWidget(lightRow(QStringLiteral("前后夹紧"), m_frClose));
    mechLayout->addWidget(lightRow(QStringLiteral("充电张开"), m_chOpen));
    mechLayout->addWidget(lightRow(QStringLiteral("充电夹紧"), m_chClose));
    mechLayout->addWidget(lightRow(QStringLiteral("拨杆在A点"), m_paddleA));
    mechLayout->addWidget(lightRow(QStringLiteral("拨杆在B点"), m_paddleB));

    auto *inBox = new QGroupBox(QStringLiteral("输入（字节29）与输出（字节32）"));
    auto *inLayout = new QVBoxLayout(inBox);
    inLayout->addWidget(lightRow(QStringLiteral("急停按下"), m_estop));
    inLayout->addWidget(lightRow(QStringLiteral("烟雾报警"), m_smoke));
    inLayout->addWidget(lightRow(QStringLiteral("飞机在平台"), m_plane));
    inLayout->addWidget(lightRow(QStringLiteral("设备风扇"), m_fan));
    inLayout->addWidget(lightRow(QStringLiteral("电控柜风扇"), m_cabinetFan));
    inLayout->addWidget(lightRow(QStringLiteral("空调"), m_ac));
    inLayout->addWidget(lightRow(QStringLiteral("充电器有电"), m_charger));
    inLayout->addWidget(lightRow(QStringLiteral("二维码灯"), m_qr));
    inLayout->addWidget(m_mode);

    m_wind = new QLineEdit;
    m_rain = new QLineEdit;
    m_indoor = new QLineEdit;
    m_cabinet = new QLineEdit;
    m_torque = new QLineEdit;
    m_position = new QLineEdit;
    m_servoTime = new QLineEdit;
    const QList<QLineEdit *> fields = {m_wind, m_rain, m_indoor, m_cabinet, m_torque, m_position, m_servoTime};
    for (QLineEdit *field : fields) {
        field->setReadOnly(true);
    }

    auto *weatherBox = new QGroupBox(QStringLiteral("气象（串口，不在 PLC 帧内）"));
    auto *weatherForm = new QFormLayout(weatherBox);
    weatherForm->addRow(QStringLiteral("风速"), m_wind);
    weatherForm->addRow(QStringLiteral("雨雪"), m_rain);
    weatherForm->addRow(QStringLiteral("设备内温湿度"), m_indoor);
    weatherForm->addRow(QStringLiteral("电柜温湿度"), m_cabinet);

    auto *servoBox = new QGroupBox(QStringLiteral("伺服（字节6起，力矩为表中的百分比原值）"));
    auto *servoForm = new QFormLayout(servoBox);
    servoForm->addRow(QStringLiteral("PLC时间"), m_servoTime);
    servoForm->addRow(QStringLiteral("力矩 顶门/升降/前后/充电"), m_torque);
    servoForm->addRow(QStringLiteral("坐标 顶门/升降/前后/充电"), m_position);

    auto *columns = new QHBoxLayout;
    columns->addWidget(netBox);
    columns->addWidget(mechBox);
    columns->addWidget(inBox);

    auto *content = new QWidget;
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->addLayout(columns);
    contentLayout->addWidget(m_fault);
    contentLayout->addWidget(weatherBox);
    contentLayout->addWidget(servoBox);
    contentLayout->addStretch();

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setWidget(content);
    auto *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scroll);

    connect(m_connect, &QPushButton::clicked, this, [this]() {
        quint16 port = m_plcPort->text().toUShort();
        if (port == 0) {
            QMessageBox::warning(this, QStringLiteral("端口"), QStringLiteral("端口不是有效数字"));
            return;
        }
        emit connectPlcRequested(m_plcIp->text().trimmed(), port);
    });
    connect(m_close, &QPushButton::clicked, this, &PageDeviceStatus::closePlcRequested);
}

void PageDeviceStatus::setPlcIp(const QString &ip) { m_plcIp->setText(ip); }
void PageDeviceStatus::setPlcPort(quint16 port) { m_plcPort->setText(QString::number(port)); }
QString PageDeviceStatus::plcIp() const { return m_plcIp->text().trimmed(); }
quint16 PageDeviceStatus::plcPort() const { return m_plcPort->text().toUShort(); }

void PageDeviceStatus::setPlcConnected(bool connected)
{
    m_plcLight->setState(connected);
    m_close->setEnabled(connected);
    if (!connected) {
        m_mode->setText(QStringLiteral("PLC状态：未连接"));
    }
}

void PageDeviceStatus::setMqttConnected(bool connected)
{
    m_mqttLight->setState(connected);
}

void PageDeviceStatus::updateHealth(const Protocol::HealthStatus &status)
{
    m_doorOpen->setState(status.doorOpen);
    m_doorClose->setState(status.doorClose);
    m_frOpen->setState(status.gripFrOpen);
    m_frClose->setState(status.gripFrClose);
    m_chOpen->setState(status.gripChOpen);
    m_chClose->setState(status.gripChClose);
    m_paddleA->setState(status.paddleAtA);
    m_paddleB->setState(status.paddleAtB);
    m_estop->setState(status.estop);
    m_smoke->setState(status.smoke);
    m_plane->setState(status.planePresent);
    m_fan->setState(status.deviceFan);
    m_cabinetFan->setState(status.cabinetFan);
    m_ac->setState(status.airCondition);
    m_charger->setState(status.chargerPower);
    m_qr->setState(status.qrLight);
    m_mode->setText(QStringLiteral("PLC状态：%1（%2）  复位完成：%3")
                        .arg(Protocol::plcStateText(status.plcState))
                        .arg(status.automatic ? QStringLiteral("自动位") : QStringLiteral("手动位"))
                        .arg(status.resetDone ? QStringLiteral("是") : QStringLiteral("否")));

    QStringList faults;
    auto addAxis = [&faults](const QString &name, quint16 code) {
        if (code != 0) {
            faults << QStringLiteral("%1 %2").arg(name, Protocol::driverFaultText(code));
        }
    };
    addAxis(QStringLiteral("顶门轴"), status.topDoorFault);
    addAxis(QStringLiteral("升降轴"), status.liftFault);
    addAxis(QStringLiteral("前后轴"), status.gripFrFault);
    addAxis(QStringLiteral("充电轴"), status.gripChFault);
    if (status.paddleFault != 0) {
        faults << QStringLiteral("拨桨叶 %1").arg(Protocol::driverFaultText(status.paddleFault));
    }
    if (status.plcFault != 0) {
        faults << QStringLiteral("PLC %1").arg(Protocol::processFaultText(status.plcFault));
    }
    const quint8 alarm = static_cast<quint8>(status.alarmRaw & 0xFF);
    if (status.alarmRaw != 0) {
        faults << QStringLiteral("告警 %1").arg(Protocol::alarmText(alarm));
    }
    m_fault->setText(faults.isEmpty() ? QStringLiteral("故障/告警：无")
                                      : QStringLiteral("故障/告警：%1").arg(faults.join(QStringLiteral("；"))));
}

void PageDeviceStatus::updateServo(const Protocol::ServoStatus &status)
{
    m_servoTime->setText(QStringLiteral("%1:%2:%3")
                             .arg(status.hour, 2, 10, QLatin1Char('0'))
                             .arg(status.minute, 2, 10, QLatin1Char('0'))
                             .arg(status.second, 2, 10, QLatin1Char('0')));
    m_torque->setText(QStringLiteral("%1 / %2 / %3 / %4")
                          .arg(status.torqueTopDoor).arg(status.torqueLift)
                          .arg(status.torqueGripFr).arg(status.torqueGripCh));
    m_position->setText(QStringLiteral("%1 / %2 / %3 / %4")
                            .arg(status.posTopDoor).arg(status.posLift)
                            .arg(status.posGripFr).arg(status.posGripCh));
}

void PageDeviceStatus::updateWeather(const WeatherComm::Reading &reading)
{
    m_windLight->setState(reading.windRainLink);
    m_climateLight->setState(reading.climateLink);
    m_wind->setText(reading.windValid ? QString::number(reading.windSpeed, 'f', 1) + QStringLiteral(" m/s")
                                      : QStringLiteral("-"));
    if (!reading.rainValid) {
        m_rain->setText(QStringLiteral("-"));
    } else if (reading.rain == 1) {
        m_rain->setText(QStringLiteral("有雨雪"));
    } else if (reading.rain == 0) {
        m_rain->setText(QStringLiteral("无雨雪"));
    } else {
        m_rain->setText(QStringLiteral("未知"));
    }
    m_indoor->setText(reading.indoorValid
                          ? QStringLiteral("%1 ℃  %2 %RH").arg(reading.indoorTemp, 0, 'f', 1).arg(reading.indoorHum, 0, 'f', 1)
                          : QStringLiteral("-"));
    m_cabinet->setText(reading.cabinetValid
                           ? QStringLiteral("%1 ℃  %2 %RH").arg(reading.cabinetTemp, 0, 'f', 1).arg(reading.cabinetHum, 0, 'f', 1)
                           : QStringLiteral("-"));
}
