#include "logger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
}

void MainWindow::initialize()
{
    Logger::instance().setOutputWidget(ui->textBrowser_log);

    // 检测工具文件夹是否存在
    QString appDir = QApplication::applicationDirPath();
    QString toolsetDirPath = QDir::cleanPath(appDir + QDir::separator() + "toolset");
    QDir toolsetDir(toolsetDirPath);
    if (!toolsetDir.exists())
    {
        Logger::instance().info("不存在toolset文件夹，创建");
        if (!toolsetDir.mkpath(".")) Logger::instance().error("toolset文件夹无法创建");
    }
    else Logger::instance().info("toolset文件夹已存在");
    QString configureClientPath = QDir::cleanPath(toolsetDirPath + QDir::separator() + "configure_client.bat");
    QFile configureClientFile(configureClientPath);
    if (!configureClientFile.exists())
    {
        Logger::instance().info("客户端配置文件不存在，创建");

        configureClientFile.close();
        QFile configureFile(":/configure_client.bat");
        configureFile.copy(configureClientPath);
        configureFile.close();
    }

    // 检测是否安装了Zwift
    if (isZwiftInstalled = Utils::getZwiftInstallLocation(zwiftInstallFolderPath); isZwiftInstalled)
    {
        Logger::instance().info("检测到已安装Zwift");
        Logger::instance().info(QString("Zwift安装路径：%1").arg(zwiftInstallFolderPath));
        ui->label_zwiftStatus->setText("已安装");
        ui->label_zwiftStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");

        zwiftInstalledVersion = Utils::getInstalledZwiftVersion(zwiftInstallFolderPath);
        if (zwiftInstalledVersion.isEmpty())
        {
            Logger::instance().warn("无法获取Zwift版本");
            ui->label_zwiftStatus->setText("无版本");
            ui->label_zwiftStatus->setStyleSheet("QLabel { color: rgb(170, 85, 0); }");
        }
        else
        {
            Logger::instance().info(QString("Zwift版本：%1").arg(zwiftInstalledVersion));
        }
    }
    else
    {
        Logger::instance().warn("未安装Zwift");
    }

    // 获取zoffline信息
    Logger::instance().info("正在获取Zoffline信息");
    Utils::getLatestZofflineInfo([this](const QString &name, const QString &url) {
        Logger::instance().info(QString("Zoffline最新: %1").arg(name));
        Logger::instance().info(QString("Zoffline链接: %1").arg(url));
        this->latestZofflineFileName = name;

        QString appDir = QApplication::applicationDirPath();
        QString toolsetDirPath = appDir + QDir::separator() + "toolset" + QDir::separator();

        if (bool isZofflineInstalled = QFile::exists(toolsetDirPath + name); !isZofflineInstalled)
        {
            Logger::instance().info("未下载Zoffline，开始下载");
            this->ui->label_zofflineStatus->setText("下载中");
            Utils::downloadFile(toolsetDirPath, name, url, [this]() {
                Logger::instance().info("Zoffline下载成功");
                this->ui->pushButton_launchZwift->setEnabled(true);
                this->ui->label_zofflineStatus->setText("已下载");
                this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
            });
        }
        else
        {
            Logger::instance().info("Zoffline已经是最新版本，无需更新");
            this->ui->pushButton_launchZwift->setEnabled(true);
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
        }

        zofflineInstalledVersion = Utils::parseZofflineVersion(name);
        Logger::instance().info(QString("Zoffline版本：%1").arg(zofflineInstalledVersion));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_connectMethod_currentIndexChanged(int index)
{
    if (index == 2)
    {
        ui->comboBox_customServer->setEnabled(true);
    }
    else
    {
        ui->comboBox_customServer->setEnabled(false);
    }
}


void MainWindow::on_pushButton_launchZwift_clicked()
{
    if (isZwiftInstalled == false)
    {
        Logger::instance().warn("未安装Zwift，无法启动");
        return;
    }
    if (isZofflineInstalled)
    {
        Logger::instance().warn("未安装Zoffline，无法启动");
        return;
    }
    ui->pushButton_launchZwift->setEnabled(false);

    int comparison = Utils::compareVersion(zwiftInstalledVersion, zofflineInstalledVersion);
    if (comparison == 1)
    {
        Logger::instance().warn("Zwift版本高于Zoffline版本");
    }
    else if (comparison == -1)
    {
        Logger::instance().info("Zwift版本低于Zoffline版本");
    }
    else
    {
        Logger::instance().info("Zwift匹配版本Zoffline版本");
    }

    QString appDir = QApplication::applicationDirPath();
    QString configureClientPath = QDir::cleanPath(appDir + QDir::separator() + "toolset" + QDir::separator() + "configure_client.bat");

    QProcess configureClientProcess;
    QString command = QString("\"%1\" \"%2\"").arg(configureClientPath, zwiftInstallFolderPath);
    configureClientProcess.start(command);
    configureClientProcess.waitForFinished();

    QString output = QString::fromLocal8Bit(configureClientProcess.readAllStandardOutput());
    Logger::instance().info("配置客户端输出：" + output);

    QString zofflinePath = QDir::cleanPath(appDir + QDir::separator() + "toolset" + QDir::separator() + latestZofflineFileName);
    QString zwiftPath = QDir::cleanPath(zwiftInstallFolderPath + QDir::separator() + "ZwiftLauncher.exe");

    // QDesktopServices::openUrl(QUrl("file:///" + zofflinePath, QUrl::TolerantMode));
    QDesktopServices::openUrl(QUrl("file:///" + zwiftPath, QUrl::TolerantMode));

    connect(&zofflineProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        Logger::instance().info(zofflineProcess.readAllStandardOutput());
    });
    connect(&zofflineProcess, &QProcess::readyReadStandardError, this, [this]() {
        Logger::instance().info(zofflineProcess.readAllStandardError());
    });
    zofflineProcess.start(zofflinePath);
    // zwiftProcess.start(zwiftPath);
}

