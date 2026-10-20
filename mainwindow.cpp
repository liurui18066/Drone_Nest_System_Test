#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "app_config.h"
#include "communication_manager.h"
#include "page_debug_control.h"
#include "page_device_status.h"
#include "page_maintenance.h"
#include "protocol.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QPushButton>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("无人机机巢上位机"));
    resize(1180, 760);

    m_tabs = new QTabWidget(this);
    m_statusPage = new PageDeviceStatus;
    m_debugPage = new PageDebugControl;
    m_maintenancePage = new PageMaintenance;
    m_tabs->addTab(m_statusPage, QStringLiteral("设备状态"));
    m_tabs->addTab(m_debugPage, QStringLiteral("单步调试"));
    m_tabs->addTab(m_maintenancePage, QStringLiteral("运维管理"));

    m_log = new QPlainTextEdit;
    m_log->setReadOnly(true);
    m_log->setMaximumBlockCount(8000);
    m_log->setMinimumHeight(180);
    m_log->setStyleSheet(QStringLiteral(
        "QPlainTextEdit { background:#1e1e1e; color:#d4d4d4; font-family:monospace; }"));
    auto *clearLog = new QPushButton(QStringLiteral("清空日志"));
    connect(clearLog, &QPushButton::clicked, m_log, &QPlainTextEdit::clear);
    auto *logPanel = new QWidget;
    auto *logLayout = new QVBoxLayout(logPanel);
    logLayout->setContentsMargins(0, 0, 0, 0);
    logLayout->addWidget(clearLog);
    logLayout->addWidget(m_log);

    auto *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_tabs);
    splitter->addWidget(logPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);
    setCentralWidget(splitter);

    bool created = false;
    AppConfig config = loadAppConfig(configPath(), &created);
    m_statusPage->setPlcIp(config.plcIp);
    m_statusPage->setPlcPort(config.plcPort);
    m_statusPage->setMqttClientId(config.mqttClientId);

    QString selfCheckError;
    if (!Protocol::selfCheck(&selfCheckError)) {
        appendLog(Protocol::LogError, QStringLiteral("系统"), selfCheckError);
    } else {
        appendLog(Protocol::LogInfo, QStringLiteral("系统"),
                  QStringLiteral("协议自检通过。帧长 32 字节；舱门操作码在字节4，机型在字节6；"
                                 "回复状态在字节28，告警在字节30，故障在字节32。"
                                 "配置文件：%1%2")
                      .arg(configPath(), created ? QStringLiteral("（已按默认值创建）") : QString()));
    }
    appendLog(Protocol::LogInfo, QStringLiteral("系统"),
              QStringLiteral("日志级别：灰色=收发原文，白色=过程，黄色=丢包或异常，红色=失败。"
                             "排查时先看红色和黄色，再对照同一地址的 TX/RX 原文。"));

    m_comm = new CommunicationManager(this);
    m_debugPage->setCommunicationManager(m_comm);
    m_maintenancePage->setCommunicationManager(m_comm);
    connect(m_comm, &CommunicationManager::logMessage, this, &MainWindow::appendLog);
    connect(m_comm, &CommunicationManager::plcConnectionChanged, m_statusPage, &PageDeviceStatus::setPlcConnected);
    connect(m_comm, &CommunicationManager::mqttConnectionChanged, m_statusPage, &PageDeviceStatus::setMqttConnected);
    connect(m_comm, &CommunicationManager::healthUpdated, m_statusPage, &PageDeviceStatus::updateHealth);
    connect(m_comm, &CommunicationManager::servoUpdated, m_statusPage, &PageDeviceStatus::updateServo);
    connect(m_comm, &CommunicationManager::weatherUpdated, m_statusPage, &PageDeviceStatus::updateWeather);
    connect(m_comm, &CommunicationManager::plcConnectionChanged, this, [this](bool connected) {
        appendLog(connected ? Protocol::LogInfo : Protocol::LogWarn, QStringLiteral("PLC"),
                  connected ? QStringLiteral("界面收到：PLC 已连接") : QStringLiteral("界面收到：PLC 已断开"));
    });
    connect(m_statusPage, &PageDeviceStatus::connectPlcRequested, this, [this](const QString &ip, quint16 port) {
        AppConfig config = loadAppConfig(configPath());
        config.plcIp = ip;
        config.plcPort = port;
        if (!saveAppConfig(config, configPath())) {
            appendLog(Protocol::LogError, QStringLiteral("系统"), QStringLiteral("写入 config.xml 失败"));
        }
        appendLog(Protocol::LogInfo, QStringLiteral("系统"),
                  QStringLiteral("用户请求连接 PLC %1:%2").arg(ip).arg(port));
        m_comm->reconnectPlc(ip, port);
    });
    connect(m_statusPage, &PageDeviceStatus::closePlcRequested, this, [this]() {
        appendLog(Protocol::LogInfo, QStringLiteral("系统"), QStringLiteral("用户请求断开 PLC"));
        m_comm->disconnectPlc();
    });

    m_comm->startAll(config);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::configPath() const
{
    const QString besideApp = QCoreApplication::applicationDirPath() + QStringLiteral("/config.xml");
    if (QFileInfo::exists(besideApp)) {
        return besideApp;
    }
    return QFileInfo(QStringLiteral("config.xml")).absoluteFilePath();
}

void MainWindow::appendLog(int level, const QString &category, const QString &text)
{
    QColor color(QStringLiteral("#d4d4d4"));
    if (level == Protocol::LogTrace) color = QColor(QStringLiteral("#9e9e9e"));
    else if (level == Protocol::LogWarn) color = QColor(QStringLiteral("#ffb74d"));
    else if (level == Protocol::LogError) color = QColor(QStringLiteral("#ef5350"));

    const QString line = QStringLiteral("[%1] [%2] %3")
                             .arg(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss.zzz")),
                                  category, text);
    QTextCursor cursor = m_log->textCursor();
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    format.setForeground(color);
    cursor.insertText(line + QLatin1Char('\n'), format);
    m_log->setTextCursor(cursor);
    m_log->ensureCursorVisible();
}
