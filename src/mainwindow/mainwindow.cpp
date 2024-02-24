#include "../logger/logger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Logger::instance().setOutputWidget(ui->textBrowser_mainLog);

    appPath = QApplication::applicationDirPath();
    toolsetPath = QDir::cleanPath(appPath + QDir::separator() + "toolset");
    configureClientPath = toolsetPath + QDir::separator() + "configure_client.bat";

    isZwiftInstalled = Utils::getZwiftInstallLocation(zwiftPath);
    zwiftPath = QDir::cleanPath(zwiftPath);
    zwiftLauncherPath = zwiftPath + QDir::separator() + "ZwiftLauncher.exe";

    initialize();
}

void MainWindow::initialize()
{
    // 检测工具文件夹是否存在
    QDir toolsetDir(toolsetPath);
    if (!toolsetDir.exists()) {
        if (!toolsetDir.mkpath(".")) QMessageBox::critical(nullptr, "错误", "无法创建toolset文件夹");
    }

    // 判断配置客户端脚本文件是否存在
    if (!QFile::exists(configureClientPath)) {
        QFile configureClientFile(":/configure_client.bat");
        configureClientFile.copy(configureClientPath);
        configureClientFile.close();
    }

    // 检测是否安装了Zwift
    if (isZwiftInstalled) {
        Logger::instance().info("Zwift安装路径: ", zwiftPath);
        ui->label_zwiftStatus->setText("已安装");
        ui->label_zwiftStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");

        zwiftVersion = Utils::getInstalledZwiftVersion(zwiftPath);
        Logger::instance().info("Zwift版本: ", zwiftVersion);
    }

    // 获取zoffline信息
    Logger::instance().info("正在获取Zoffline信息");
    Utils::getLatestZofflineInfo([this](const QString &name, const QString &url) {
        Logger::instance().info("Zoffline最新: ", name);
        Logger::instance().info("Zoffline链接: ", url);

        zofflineFileName = name;
        zofflinePath = toolsetPath + QDir::separator() + zofflineFileName;

        isZofflineInstalled = QFile::exists(zofflinePath);

        auto activateZofflineStatus = [this]() {
            this->ui->pushButton_launch->setEnabled(true);
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
        };

        if (!isZofflineInstalled) {
            Logger::instance().info("未下载Zoffline，开始下载");
            this->ui->label_zofflineStatus->setText("下载中");
            Utils::downloadFile(toolsetPath, name, url, [this, activateZofflineStatus]() {
                Logger::instance().info("Zoffline下载成功");
                activateZofflineStatus();
            });
        } else {
            Logger::instance().info("Zoffline已经是最新版本，无需更新");
            activateZofflineStatus();
        }
        zofflineVersion = Utils::parseZofflineVersion(name);
        Logger::instance().info("Zoffline版本: ", zofflineVersion);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_connectMethod_currentIndexChanged(int index)
{
    if (index == 2) ui->comboBox_customServer->setEnabled(true);
    else ui->comboBox_customServer->setEnabled(false);
}


void MainWindow::on_pushButton_launchZwift_clicked()
{
    if (isZwiftInstalled == false) {
        QMessageBox::warning(nullptr, "警告", "未安装Zwift，无法启动");
        return;
    }
    if (isZofflineInstalled) {
        QMessageBox::warning(nullptr, "警告", "未安装Zoffline，无法启动");
        return;
    }
    ui->pushButton_launch->setEnabled(false);

    int comparison = Utils::compareVersion(zwiftVersion, zofflineVersion);
    if (comparison == 1) Logger::instance().warn("Zwift版本高于Zoffline版本");
    else if (comparison == -1) Logger::instance().info("Zwift版本低于Zoffline版本");
    else Logger::instance().info("Zwift匹配Zoffline版本");

    // zoffline证书配置
    QProcess configureClientProcess;
    QString command = QString("\"%1\" \"%2\"").arg(configureClientPath, zwiftPath);
    configureClientProcess.start(command);
    configureClientProcess.waitForFinished();

    QString output = QString::fromLocal8Bit(configureClientProcess.readAllStandardOutput());
    Logger::instance().info("配置客户端输出：" + output);

    Logger::instance().info("写入hosts");
    Utils::writeHosts();

    connect(&zofflineProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        Logger::instance().info(zofflineProcess.readAllStandardOutput());
    });
    connect(&zofflineProcess, &QProcess::readyReadStandardError, this, [this]() {
        Logger::instance().info(zofflineProcess.readAllStandardError());
    });

    // 启动Zoffline
    zofflineProcess.start(zofflinePath);

    // 启动Zwift
    QDesktopServices::openUrl(QUrl("file:///" + zwiftLauncherPath, QUrl::TolerantMode));
}

