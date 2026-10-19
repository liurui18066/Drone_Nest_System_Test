#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CommunicationManager;
class PageDeviceStatus;
class PageDebugControl;
class PageMaintenance;
class QTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void appendLog(int level, const QString &category, const QString &text);
    QString configPath() const;

    Ui::MainWindow *ui;
    QTabWidget *m_tabs = nullptr;
    PageDeviceStatus *m_statusPage = nullptr;
    PageDebugControl *m_debugPage = nullptr;
    PageMaintenance *m_maintenancePage = nullptr;
    QPlainTextEdit *m_log = nullptr;
    CommunicationManager *m_comm = nullptr;
};
#endif // MAINWINDOW_H
