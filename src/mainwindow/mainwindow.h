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

    void on_pushButton_launchZwift_clicked();

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
    QString latestZofflineFileName;
    QString zofflineInstalledVersion;
    QString zwiftInstallFolderPath;
    QString zwiftInstalledVersion;

    QProcess zofflineProcess;
    QProcess zwiftProcess;

};
#endif // MAINWINDOW_H
