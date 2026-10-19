#include "mainwindow.h"
#include "protocol.h"
#include "weather_comm.h"

#include <QApplication>
#include <QFont>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font(QStringLiteral("Noto Sans CJK SC"));
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    qRegisterMetaType<Protocol::HealthStatus>("Protocol::HealthStatus");
    qRegisterMetaType<Protocol::ServoStatus>("Protocol::ServoStatus");
    qRegisterMetaType<Protocol::CommandReply>("Protocol::CommandReply");
    qRegisterMetaType<WeatherComm::Reading>("WeatherComm::Reading");

    MainWindow window;
    window.show();
    return app.exec();
}
