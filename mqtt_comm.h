#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include <QObject>
#include <QtMqtt/qmqttclient.h>
#include <QTimer>

class MqttComm : public QObject
{
    Q_OBJECT
public:
    explicit MqttComm(const QString &host = "127.0.0.1",
                      quint16 port = 1883,
                      const QString &clientId = "",
                      QObject *parent = nullptr);
    ~MqttComm();
    bool isConnected() const;
public slots:
    void start();
    void disconnectFromBroker();
    bool publishMessage(const QString &topic, const QByteArray &payload, quint8 qos = 0);
    void subscribeTopic(const QString &topic, quint8 qos = 0);

signals:
    void connectionStateChanged(bool connected);
    void messageReceived(const QString &topic, const QByteArray &payload);
    void errorOccurred(const QString &errorStr);

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onErrorOccurred(QMqttClient::ClientError error);
    void attemptReconnection();

private:
    void connectToBroker();


    QMqttClient *m_mqttClient;
    QString m_host;
    quint16 m_port;
    QString m_clientId;

    QTimer *m_reconnectTimer;
    bool m_isReconnecting;
};

#endif // MQTT_COMM_H
