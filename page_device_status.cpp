#include "page_device_status.h"

#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
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

enum IconKind {
    IconFan, IconTemp, IconDrop, IconCabinet,
    IconWind, IconRain, IconPlug, IconBolt,
    IconBattery, IconPower, IconClock, IconId
};

QPixmap statusIcon(IconKind kind)
{
    QPixmap pix(16, 16);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(QStringLiteral("#8b95a7")), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    switch (kind) {
    case IconFan:
        p.drawEllipse(QRectF(2, 2, 12, 12));
        p.drawLine(QPointF(8, 3), QPointF(8, 13));
        p.drawLine(QPointF(3, 8), QPointF(13, 8));
        break;
    case IconTemp:
        p.drawRoundedRect(QRectF(6, 1.5, 4, 9), 2, 2);
        p.drawEllipse(QRectF(4.5, 8, 7, 7));
        break;
    case IconDrop:
        p.drawEllipse(QRectF(4, 6, 8, 8));
        p.drawLine(QPointF(8, 2), QPointF(5, 8));
        p.drawLine(QPointF(8, 2), QPointF(11, 8));
        break;
    case IconCabinet:
        p.drawRoundedRect(QRectF(3, 2, 10, 12), 1.5, 1.5);
        p.drawLine(QPointF(8, 2), QPointF(8, 14));
        break;
    case IconWind:
        p.drawArc(QRectF(2, 3, 12, 6), 20 * 16, 140 * 16);
        p.drawArc(QRectF(2, 8, 12, 6), 20 * 16, 140 * 16);
        break;
    case IconRain:
        p.drawLine(QPointF(5, 2), QPointF(3, 7));
        p.drawLine(QPointF(9, 2), QPointF(7, 7));
        p.drawLine(QPointF(13, 2), QPointF(11, 7));
        p.drawLine(QPointF(4, 10), QPointF(2.5, 14));
        p.drawLine(QPointF(8, 10), QPointF(6.5, 14));
        p.drawLine(QPointF(12, 10), QPointF(10.5, 14));
        break;
    case IconPlug:
        p.drawRoundedRect(QRectF(3, 6, 10, 8), 1.5, 1.5);
        p.drawLine(QPointF(6, 6), QPointF(6, 2.5));
        p.drawLine(QPointF(10, 6), QPointF(10, 2.5));
        break;
    case IconBolt:
        p.drawLine(QPointF(9, 1.5), QPointF(4, 9));
        p.drawLine(QPointF(4, 9), QPointF(8, 9));
        p.drawLine(QPointF(8, 9), QPointF(6, 14.5));
        p.drawLine(QPointF(6, 14.5), QPointF(12, 7));
        p.drawLine(QPointF(12, 7), QPointF(8, 7));
        p.drawLine(QPointF(8, 7), QPointF(9, 1.5));
        break;
    case IconBattery:
        p.drawRoundedRect(QRectF(2, 4, 10, 8), 1.2, 1.2);
        p.drawLine(QPointF(12, 6.5), QPointF(12, 9.5));
        p.drawLine(QPointF(4, 8), QPointF(8, 8));
        break;
    case IconPower:
        p.drawArc(QRectF(4, 3, 8, 8), 40 * 16, 280 * 16);
        p.drawLine(QPointF(8, 2), QPointF(8, 7));
        break;
    case IconClock:
        p.drawEllipse(QRectF(2, 2, 12, 12));
        p.drawLine(QPointF(8, 8), QPointF(8, 5));
        p.drawLine(QPointF(8, 8), QPointF(11, 9));
        break;
    case IconId:
        p.drawRoundedRect(QRectF(2, 3, 12, 10), 1.5, 1.5);
        p.drawLine(QPointF(5, 7), QPointF(11, 7));
        p.drawLine(QPointF(5, 10), QPointF(9, 10));
        break;
    }
    return pix;
}

QLabel *iconLabel(IconKind kind)
{
    auto *label = new QLabel;
    label->setPixmap(statusIcon(kind));
    label->setFixedSize(16, 16);
    return label;
}

void setCardCheck(QLabel *label, bool ok)
{
    label->setText(ok ? QStringLiteral("✓") : QString());
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(ok
                             ? QStringLiteral("background:#2f9e44; color:white; border-radius:9px; font-size:11px; font-weight:700;")
                             : QStringLiteral("background:#dfe3ea; color:transparent; border-radius:9px;"));
}

void setActivityDot(QLabel *dot, bool on)
{
    dot->setStyleSheet(QStringLiteral("background:%1; border-radius:5px;")
                           .arg(on ? QStringLiteral("#2f9e44") : QStringLiteral("#c5cad3")));
}

QLabel *valueLabel(const QString &text)
{
    auto *label = new QLabel(text);
    label->setStyleSheet(QStringLiteral("color:#1c2434; font-size:13px;"));
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return label;
}

QWidget *infoRow(IconKind kind, const QString &name, QWidget *right)
{
    auto *row = new QWidget;
    auto *layout = new QHBoxLayout(row);
    layout->setContentsMargins(0, 2, 0, 2);
    layout->setSpacing(8);
    layout->addWidget(iconLabel(kind));
    auto *title = new QLabel(name);
    title->setStyleSheet(QStringLiteral("color:#8b95a7; font-size:13px;"));
    layout->addWidget(title);
    layout->addStretch();
    layout->addWidget(right);
    return row;
}

class FanKnob : public QWidget
{
public:
    explicit FanKnob(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFixedSize(44, 22);
    }

    void setOn(bool on)
    {
        m_on = on;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(m_on ? QColor(QStringLiteral("#b7ebc6")) : QColor(QStringLiteral("#d5dbe3")));
        p.drawRoundedRect(rect(), 11, 11);
        const int knob = 16;
        const int x = m_on ? width() - knob - 3 : 3;
        p.setBrush(Qt::white);
        p.drawEllipse(x, 3, knob, knob);
    }

private:
    bool m_on = false;
};

QFrame *makeCard(const QString &title, QLabel *check, QVBoxLayout **body)
{
    auto *card = new QFrame;
    card->setObjectName(QStringLiteral("envCard"));
    card->setStyleSheet(QStringLiteral(
        "QFrame#envCard { background:#ffffff; border:1px solid #e6e8ef; border-radius:12px; }"));
    card->setMinimumWidth(220);
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(8);

    auto *head = new QHBoxLayout;
    head->setSpacing(8);
    auto *name = new QLabel(title);
    name->setStyleSheet(QStringLiteral("color:#1c2434; font-size:15px; font-weight:600;"));
    check->setFixedSize(18, 18);
    setCardCheck(check, false);
    head->addWidget(name);
    head->addStretch();
    head->addWidget(check);
    layout->addLayout(head);

    *body = new QVBoxLayout;
    (*body)->setSpacing(4);
    layout->addLayout(*body);
    layout->addStretch();
    return card;
}

QString dash()
{
    return QStringLiteral("—");
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
    m_upsLight = new IndicatorLight;
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

    auto *netBox = new QGroupBox(QStringLiteral("PLC 连接"));
    auto *netLayout = new QVBoxLayout(netBox);
    netLayout->addWidget(lightRow(QStringLiteral("PLC"), m_plcLight));
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

    m_fanOff = new QLabel(QStringLiteral("关闭"));
    m_fanOn = new QLabel(QStringLiteral("打开"));
    m_fanOff->setStyleSheet(QStringLiteral("color:#5c6570; font-size:12px;"));
    m_fanOn->setStyleSheet(QStringLiteral("color:#8b95a7; font-size:12px;"));
    auto *knob = new FanKnob;
    m_fanPill = knob;
    auto *fanSwitch = new QWidget;
    auto *fanLayout = new QHBoxLayout(fanSwitch);
    fanLayout->setContentsMargins(0, 0, 0, 0);
    fanLayout->setSpacing(6);
    fanLayout->addWidget(m_fanOff);
    fanLayout->addWidget(knob);
    fanLayout->addWidget(m_fanOn);

    m_deviceTemp = valueLabel(dash());
    m_deviceHum = valueLabel(dash());
    m_cabinetTemp = valueLabel(dash());
    m_cabinetHum = valueLabel(dash());
    m_windValue = valueLabel(dash());
    m_rainValue = valueLabel(dash());
    m_upsSupply = valueLabel(dash());
    m_upsRectifier = valueLabel(dash());
    m_upsCharge = valueLabel(dash());
    m_upsRunning = valueLabel(dash());
    m_upsBackup = valueLabel(dash());
    m_mqttId = valueLabel(dash());
    m_mqttHeartbeat = valueLabel(dash());
    m_mqttSensor = valueLabel(dash());
    m_publishDot = new QLabel;
    m_receiveDot = new QLabel;
    m_publishDot->setFixedSize(10, 10);
    m_receiveDot->setFixedSize(10, 10);
    setActivityDot(m_publishDot, false);
    setActivityDot(m_receiveDot, false);

    auto *publishRow = new QWidget;
    auto *publishLayout = new QHBoxLayout(publishRow);
    publishLayout->setContentsMargins(0, 0, 0, 0);
    publishLayout->setSpacing(14);
    auto addDot = [publishLayout](const QString &name, QLabel *dot) {
        auto *wrap = new QWidget;
        auto *row = new QHBoxLayout(wrap);
        row->setContentsMargins(0, 0, 0, 0);
        row->setSpacing(6);
        auto *text = new QLabel(name);
        text->setStyleSheet(QStringLiteral("color:#8b95a7; font-size:13px;"));
        row->addWidget(dot);
        row->addWidget(text);
        publishLayout->addWidget(wrap);
    };
    addDot(QStringLiteral("发布"), m_publishDot);
    addDot(QStringLiteral("接收"), m_receiveDot);

    QVBoxLayout *climateBody = nullptr;
    QVBoxLayout *windBody = nullptr;
    QVBoxLayout *upsBody = nullptr;
    QVBoxLayout *mqttBody = nullptr;
    auto *climateCard = makeCard(QStringLiteral("温湿度"), m_climateLight, &climateBody);
    auto *windCard = makeCard(QStringLiteral("风速雨雪"), m_windLight, &windBody);
    auto *upsCard = makeCard(QStringLiteral("UPS"), m_upsLight, &upsBody);
    auto *mqttCard = makeCard(QStringLiteral("MQTT"), m_mqttLight, &mqttBody);

    climateBody->addWidget(infoRow(IconFan, QStringLiteral("温控风扇"), fanSwitch));
    climateBody->addWidget(infoRow(IconTemp, QStringLiteral("设备温度"), m_deviceTemp));
    climateBody->addWidget(infoRow(IconDrop, QStringLiteral("设备湿度"), m_deviceHum));
    climateBody->addWidget(infoRow(IconTemp, QStringLiteral("电柜温度"), m_cabinetTemp));
    climateBody->addWidget(infoRow(IconDrop, QStringLiteral("电柜湿度"), m_cabinetHum));

    windBody->addWidget(infoRow(IconWind, QStringLiteral("风速"), m_windValue));
    windBody->addWidget(infoRow(IconRain, QStringLiteral("雨雪"), m_rainValue));

    upsBody->addWidget(infoRow(IconPlug, QStringLiteral("供电方式"), m_upsSupply));
    upsBody->addWidget(infoRow(IconBolt, QStringLiteral("整流侧供电状态"), m_upsRectifier));
    upsBody->addWidget(infoRow(IconBattery, QStringLiteral("电池充电状态"), m_upsCharge));
    upsBody->addWidget(infoRow(IconPower, QStringLiteral("电池开机状态"), m_upsRunning));
    upsBody->addWidget(infoRow(IconClock, QStringLiteral("电池后备时间"), m_upsBackup));

    mqttBody->addWidget(infoRow(IconId, QStringLiteral("设备ID"), m_mqttId));
    mqttBody->addWidget(infoRow(IconClock, QStringLiteral("status (heartbeat)"), m_mqttHeartbeat));
    mqttBody->addWidget(infoRow(IconClock, QStringLiteral("status (sensor)"), m_mqttSensor));
    mqttBody->addWidget(publishRow);

    m_torque = new QLineEdit;
    m_position = new QLineEdit;
    m_servoTime = new QLineEdit;
    const QList<QLineEdit *> fields = {m_torque, m_position, m_servoTime};
    for (QLineEdit *field : fields) {
        field->setReadOnly(true);
    }

    auto *servoBox = new QGroupBox(QStringLiteral("伺服（字节6起，力矩为表中的百分比原值）"));
    auto *servoForm = new QFormLayout(servoBox);
    servoForm->addRow(QStringLiteral("PLC时间"), m_servoTime);
    servoForm->addRow(QStringLiteral("力矩 顶门/升降/前后/充电"), m_torque);
    servoForm->addRow(QStringLiteral("坐标 顶门/升降/前后/充电"), m_position);

    auto *columns = new QHBoxLayout;
    columns->addWidget(netBox);
    columns->addWidget(mechBox);
    columns->addWidget(inBox);

    auto *cards = new QHBoxLayout;
    cards->setSpacing(12);
    cards->addWidget(climateCard, 1);
    cards->addWidget(windCard, 1);
    cards->addWidget(upsCard, 1);
    cards->addWidget(mqttCard, 1);

    auto *content = new QWidget;
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->addLayout(columns);
    contentLayout->addWidget(m_fault);
    contentLayout->addLayout(cards);
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

void PageDeviceStatus::setMqttClientId(const QString &clientId)
{
    m_mqttId->setText(clientId.isEmpty() ? dash() : clientId);
}

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
    setCardCheck(m_mqttLight, connected);
    setActivityDot(m_publishDot, connected);
    setActivityDot(m_receiveDot, connected);
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
    if (auto *knob = dynamic_cast<FanKnob *>(m_fanPill)) {
        knob->setOn(status.deviceFan);
    }
    m_fanOff->setStyleSheet(status.deviceFan
                                ? QStringLiteral("color:#8b95a7; font-size:12px;")
                                : QStringLiteral("color:#5c6570; font-size:12px; font-weight:600;"));
    m_fanOn->setStyleSheet(status.deviceFan
                               ? QStringLiteral("color:#1f7a45; font-size:12px; font-weight:600;")
                               : QStringLiteral("color:#8b95a7; font-size:12px;"));
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
    setCardCheck(m_windLight, reading.windRainLink);
    setCardCheck(m_climateLight, reading.climateLink);
    m_windValue->setText(reading.windValid
                             ? QString::number(reading.windSpeed, 'f', 1) + QStringLiteral(" m/s")
                             : dash());
    if (!reading.rainValid) {
        m_rainValue->setText(dash());
    } else if (reading.rain == 1) {
        m_rainValue->setText(QStringLiteral("有"));
    } else if (reading.rain == 0) {
        m_rainValue->setText(QStringLiteral("无"));
    } else {
        m_rainValue->setText(QStringLiteral("未知"));
    }
    m_deviceTemp->setText(reading.indoorValid
                              ? QString::number(reading.indoorTemp, 'f', 1) + QStringLiteral(" ℃")
                              : dash());
    m_deviceHum->setText(reading.indoorValid
                             ? QString::number(reading.indoorHum, 'f', 1) + QStringLiteral("%")
                             : dash());
    m_cabinetTemp->setText(reading.cabinetValid
                               ? QString::number(reading.cabinetTemp, 'f', 1) + QStringLiteral(" ℃")
                               : dash());
    m_cabinetHum->setText(reading.cabinetValid
                              ? QString::number(reading.cabinetHum, 'f', 1) + QStringLiteral("%")
                              : dash());
}
