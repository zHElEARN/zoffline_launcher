#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>
#include <QVariant>
#include <QCloseEvent>

#include "hosts/hosts.h"

#include "serverlistmanager/serverlistmanager.h"
#include "mirrormanager/mirrormanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_connectMethod_currentIndexChanged(int index);

    void on_pushButton_launch_clicked();
    void on_pushButton_stop_clicked();

    void on_action_manage_triggered();
    void on_action_mirror_triggered();

    void on_pushButton_downloadZoffline_clicked();

private:
    void initialize();
    void updateServerList();

private:
    Ui::MainWindow *ui;
    ServerListManager *serverListManagerDialog;
    MirrorManager *mirrorManager;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QStringList zwiftHostnames = {"us-or-rly101.zwift.com", "secure.zwift.com", "cdn.zwift.com", "launcher.zwift.com"};

    // 可执行程序所在的路径
    QString appPath;
    // toolset文件夹路径 如./toolset
    QString toolsetPath;
    // 配置文件文件夹路径 如./config
    QString configPath;
    // 配置客户端文件路径 如./toolset/configure_client.bat
    QString configureClientPath;

    bool isZwiftInstalled = false;
    bool isZofflineInstalled = false;
    bool isZofflineConfigured = false;
    bool isLaunched = false;

    // 文件名 如zoffline_1.0.126283.exe
    QString zofflineFileName;
    // 下载链接 如https://github.com/zoffline/zwift-offline/releases/download/zoffline_1.0.130349/zoffline_1.0.130349.exe
    QString zofflineURL;
    // 版本号 如1.0.126283
    QString zofflineVersion;
    // 文件完整路径
    QString zofflinePath;
    QProcess zofflineProcess;

    // 安装路径 如C:\Program Files (x86)\Zwift
    QString zwiftPath;
    // Zwift启动器路径 如C:\Program Files (x86)\Zwift\ZwiftLauncher.exe
    QString zwiftLauncherPath;
    // 版本号 如1.0.126283
    QString zwiftVersion;

    Hosts hosts;
};
#endif // MAINWINDOW_H
