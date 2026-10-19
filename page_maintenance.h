#ifndef PAGE_MAINTENANCE_H
#define PAGE_MAINTENANCE_H

#include <QCheckBox>
#include <QPropertyAnimation>
#include <QWidget>

class SwitchButton : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(qreal position READ position WRITE setPosition)
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    qreal position() const { return m_position; }
    void setPosition(qreal position);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal m_position = 0;
    QColor m_onColor;
    QColor m_offColor;
    QColor m_sliderColor;
    QPropertyAnimation *m_positionAnimation = nullptr;
};

class CommunicationManager;

class PageMaintenance : public QWidget
{
    Q_OBJECT
public:
    explicit PageMaintenance(QWidget *parent = nullptr);
    void setCommunicationManager(CommunicationManager *manager);

private:
    void bindSwitch(SwitchButton *button, int itemIndex, const QString &name);

    CommunicationManager *m_comm = nullptr;
    SwitchButton *m_deviceFan = nullptr;
    SwitchButton *m_cabinetFan = nullptr;
    SwitchButton *m_ac = nullptr;
    SwitchButton *m_charger = nullptr;
    SwitchButton *m_qr = nullptr;
};
#endif // PAGE_MAINTENANCE_H
