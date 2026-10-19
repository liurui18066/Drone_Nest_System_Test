#include "page_maintenance.h"

#include "communication_manager.h"
#include "protocol.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

SwitchButton::SwitchButton(QWidget *parent)
    : QCheckBox(parent)
    , m_onColor(QStringLiteral("#4CAF50"))
    , m_offColor(QStringLiteral("#FF4D4F"))
    , m_sliderColor(Qt::white)
{
    setCheckable(true);
    setFixedSize(60, 30);
    m_positionAnimation = new QPropertyAnimation(this, "position", this);
    m_positionAnimation->setDuration(160);
    connect(this, &QCheckBox::toggled, this, [this](bool checked) {
        m_positionAnimation->stop();
        m_positionAnimation->setEndValue(checked ? 1.0 : 0.0);
        m_positionAnimation->start();
    });
}

void SwitchButton::setPosition(qreal position)
{
    m_position = position;
    update();
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(isChecked() ? m_onColor : m_offColor);
    painter.drawRoundedRect(rect(), height() / 2.0, height() / 2.0);
    painter.setPen(Qt::white);
    painter.drawText(rect().adjusted(0, 0, -8, 0), Qt::AlignRight | Qt::AlignVCenter,
                     isChecked() ? QStringLiteral("ON") : QStringLiteral("OFF"));
    const int slider = height() - 8;
    const int x = 4 + int((width() - slider - 8) * m_position);
    painter.setBrush(m_sliderColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QRect(x, 4, slider, slider));
}

void SwitchButton::mousePressEvent(QMouseEvent *event) { QCheckBox::mousePressEvent(event); }
void SwitchButton::mouseReleaseEvent(QMouseEvent *event) { QCheckBox::mouseReleaseEvent(event); }

PageMaintenance::PageMaintenance(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    auto *grid = new QGridLayout;
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(16);

    m_deviceFan = new SwitchButton;
    m_cabinetFan = new SwitchButton;
    m_ac = new SwitchButton;
    m_charger = new SwitchButton;
    m_qr = new SwitchButton;

    struct Item { QString name; SwitchButton *button; int index; };
    const Item items[] = {
        {QStringLiteral("设备风扇"), m_deviceFan, Protocol::kCtrlDeviceFan},
        {QStringLiteral("电控柜风扇"), m_cabinetFan, Protocol::kCtrlCabinetFan},
        {QStringLiteral("设备空调"), m_ac, Protocol::kCtrlAirCondition},
        {QStringLiteral("充电器电源"), m_charger, Protocol::kCtrlChargerPower},
        {QStringLiteral("二维码灯"), m_qr, Protocol::kCtrlQrLight},
    };
    for (int i = 0; i < 5; ++i) {
        grid->addWidget(new QLabel(items[i].name), 0, i * 2);
        grid->addWidget(items[i].button, 0, i * 2 + 1);
        bindSwitch(items[i].button, items[i].index, items[i].name);
    }

    auto *hint = new QLabel(QStringLiteral("开=0x01，关=0x02，写在协议表对应的高字节。0x00 不会当成关闭。"
                                          "设备运营、飞机开关机、自检、日志、配置、重启在协议表里没有命令。"));
    hint->setWordWrap(true);

    auto *row = new QHBoxLayout;
    const QStringList names = {QStringLiteral("设备自检"), QStringLiteral("日志查询"),
                               QStringLiteral("设备配置"), QStringLiteral("重启/关机")};
    for (const QString &name : names) {
        auto *button = new QPushButton(name);
        button->setMinimumSize(140, 48);
        row->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, name]() {
            QMessageBox::information(this, name, QStringLiteral("协议表没有这条命令，没有向 PLC 发送。"));
        });
    }

    layout->addLayout(grid);
    layout->addWidget(hint);
    layout->addLayout(row);
    layout->addStretch();
}

void PageMaintenance::setCommunicationManager(CommunicationManager *manager)
{
    m_comm = manager;
}

void PageMaintenance::bindSwitch(SwitchButton *button, int itemIndex, const QString &name)
{
    connect(button, &QCheckBox::toggled, this, [this, button, itemIndex, name](bool checked) {
        if (!m_comm) {
            return;
        }
        if (!m_comm->isPlcConnected()) {
            button->blockSignals(true);
            button->setChecked(!checked);
            button->setPosition(button->isChecked() ? 1.0 : 0.0);
            button->blockSignals(false);
            QMessageBox::warning(this, name, QStringLiteral("PLC 未连接，开关已恢复。"));
            return;
        }
        m_comm->sendPlcControl(itemIndex, checked ? Protocol::kOpOn : Protocol::kOpOff);
    });
}
