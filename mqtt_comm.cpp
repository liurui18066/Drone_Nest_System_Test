#include "mqtt_comm.h"
#include <QDebug>
#include <QMetaEnum>

MqttComm::MqttComm(const QString &host, quint16 port, const QString &clientId, QObject *parent)
    : QObject(parent)
    , m_host(host)
    , m_port(port)
    , m_clientId(clientId)
    , m_isReconnecting(false)
{
    m_mqttClient = new QMqttClient(this);
    if (!m_clientId.isEmpty())
        m_mqttClient->setClientId(m_clientId);
    m_mqttClient->setHostname(m_host);
    m_mqttClient->setPort(m_port);


    m_mqttClient->setCleanSession(true);
    m_mqttClient->setKeepAlive(30);
    m_mqttClient->setProtocolVersion(QMqttClient::MQTT_3_1_1);
    connect(m_mqttClient, &QMqttClient::connected, this, &MqttComm::onConnected);
    connect(m_mqttClient, &QMqttClient::disconnected, this, &MqttComm::onDisconnected);
    connect(m_mqttClient, &QMqttClient::messageReceived, this, &MqttComm::onMessageReceived);
    connect(m_mqttClient, &QMqttClient::errorChanged, this, &MqttComm::onErrorOccurred);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(5000);
    m_reconnectTimer->setSingleShot(false);
    connect(m_reconnectTimer, &QTimer::timeout, this, &MqttComm::attemptReconnection);
}

MqttComm::~MqttComm()
{
    disconnectFromBroker();
}

void MqttComm::start()
{
    qDebug() << "MqttComm::start() - initiating connection";
    connectToBroker();
}

void MqttComm::connectToBroker()
{
    if (m_mqttClient->state() == QMqttClient::Connected ||
        m_mqttClient->state() == QMqttClient::Connecting) {
        qDebug() << "MQTT already connected or connecting.";
        return;
    }
    qDebug() << "Connecting to MQTT broker at" << m_host << ":" << m_port;
    m_mqttClient->connectToHost();
}

void MqttComm::disconnectFromBroker()
{
    m_reconnectTimer->stop();
    if (m_mqttClient->state() == QMqttClient::Connected) {
        m_mqttClient->disconnectFromHost();
    }
    m_isReconnecting = false;
}

bool MqttComm::isConnected() const
{
    return (m_mqttClient->state() == QMqttClient::Connected);
}

bool MqttComm::publishMessage(const QString &topic, const QByteArray &payload, quint8 qos)
{
    if (!isConnected()) {
        qDebug() << "MQTT not connected, cannot publish";
        return false;
    }
    auto result = m_mqttClient->publish(topic, payload, qos);
    if (result == -1) {
        qDebug() << "Failed to publish to topic:" << topic;
        return false;
    }
    qDebug() << "Published to" << topic << "size:" << payload.size();
    return true;
}

void MqttComm::subscribeTopic(const QString &topic, quint8 qos)
{
    if (!isConnected()) {
        qDebug() << "Subscription deferred until connected";
        return;
    }
    auto sub = m_mqttClient->subscribe(topic, qos);
    if (!sub) {
        qDebug() << "Failed to subscribe to topic:" << topic;
    } else {
        qDebug() << "Subscribed to topic:" << topic;
    }
}

void MqttComm::onConnected()
{
    qDebug() << "MQTT connected successfully";
    m_reconnectTimer->stop();
    m_isReconnecting = false;
    emit connectionStateChanged(true);
}

void MqttComm::onDisconnected()
{
    qDebug() << "MQTT disconnected";
    emit connectionStateChanged(false);
    if (!m_isReconnecting && m_mqttClient->state() != QMqttClient::Connecting) {
        m_reconnectTimer->start();
        m_isReconnecting = true;
    }
}

void MqttComm::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "MQTT message received on topic:" << topic.name() << "size:" << message.size();
    emit messageReceived(topic.name(), message);
}

void MqttComm::onErrorOccurred(QMqttClient::ClientError error)
{
    // 使用 QMetaEnum 动态获取错误名称，避免硬编码枚举值
    QString errorStr = QString("Error code: %1").arg(static_cast<int>(error));
    const QMetaObject *metaObj = &QMqttClient::staticMetaObject;
    int enumIndex = metaObj->indexOfEnumerator("ClientError");
    if (enumIndex != -1) {
        QMetaEnum metaEnum = metaObj->enumerator(enumIndex);
        const char *key = metaEnum.valueToKey(static_cast<int>(error));
        if (key) {
            errorStr = QString("%1 (code: %2)").arg(key).arg(error);
        }
    }
    qDebug() << "MQTT error:" << errorStr;
    emit errorOccurred(errorStr);
}

void MqttComm::attemptReconnection()
{
    qDebug() << "Attempting to reconnect MQTT...";
    connectToBroker();
}
