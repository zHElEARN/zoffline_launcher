#include "logmanager.h"
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
    LogManager::instance().setOutputWidget(ui->textBrowser_log);

    // 检测工具文件夹是否存在
    QString appDir = QApplication::applicationDirPath();
    QString toolsetDirPath = QDir::cleanPath(appDir + QDir::separator() + "toolset");
    QDir toolsetDir(toolsetDirPath);
    if (!toolsetDir.exists())
    {
        LogManager::instance().info("不存在toolset文件夹，创建中");
        if (!toolsetDir.mkpath("."))
        {
            LogManager::instance().error("toolset文件夹无法创建");
        }
    }
    else
    {
        LogManager::instance().info("toolset文件夹已存在");
    }

    // 检测是否安装了Zwift
    if (Utils::isZwiftInstalled())
    {
        LogManager::instance().info("检测到已安装Zwift");
        ui->label_zwiftStatus->setText("已安装");
        ui->label_zwiftStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
    }
    else
    {
        LogManager::instance().warn("未安装Zwift");
    }

    // 获取zoffline信息
    LogManager::instance().info("正在获取Zoffline信息");
    Utils::getLatestZofflineInfo([this](const QString &name, const QString &url) {
        LogManager::instance().info(QString("最新Zoffline: %1，下载链接: %2").arg(name, url));
        this->zofflineName = name;

        QString appDir = QApplication::applicationDirPath();
        QString toolsetDirPath = appDir + QDir::separator() + "toolset" + QDir::separator();

        if (!QFile::exists(toolsetDirPath + name))
        {
            LogManager::instance().info("未下载Zoffline，开始下载");
            this->ui->label_zofflineStatus->setText("下载中");
            Utils::downloadFile(toolsetDirPath, name, url, [this]() {
                LogManager::instance().info("Zoffline下载成功");
                this->ui->label_zofflineStatus->setText("已下载");
                this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
            });
        }
        else
        {
            LogManager::instance().info("Zoffline已经是最新版本，无需更新");
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

