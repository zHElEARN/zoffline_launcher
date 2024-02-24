#include "logger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

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
        Logger::instance().info("不存在toolset文件夹，创建中");
        if (!toolsetDir.mkpath("."))
        {
            Logger::instance().error("toolset文件夹无法创建");
        }
    }
    else
    {
        Logger::instance().info("toolset文件夹已存在");
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
                this->ui->label_zofflineStatus->setText("已下载");
                this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
            });
        }
        else
        {
            Logger::instance().info("Zoffline已经是最新版本，无需更新");
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
    if (!isZofflineConfigured)
    {
        Logger::instance().info("未配置Zoffline，开始配置...");
        ui->pushButton_launchZwift->setEnabled(false);
    }
}

