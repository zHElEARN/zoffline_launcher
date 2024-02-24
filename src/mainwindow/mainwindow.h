#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>

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

private:
    void initialize();

private:
    Ui::MainWindow *ui;

private:
    QString appPath;
    QString toolsetPath;
    QString configureClientPath;

    bool isZwiftInstalled = false;
    bool isZofflineInstalled = false;
    bool isZofflineConfigured = false;

    // 文件名 如zoffline_1.0.126283.exe
    QString zofflineFileName;
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
    QProcess zwiftProcess;

};
#endif // MAINWINDOW_H
