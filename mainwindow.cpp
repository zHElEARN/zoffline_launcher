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

    if (Utils::isZwiftInstalled())
    {
        ui->label_zwiftStatus->setText("已安装");
        ui->label_zwiftStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
    }

    Utils::getLatestZofflineInfo([this](const QString &name, const QString &url) {
        this->zofflineName = name;

        QString appDir = QApplication::applicationDirPath();
        QString toolsetDirPath = appDir + QDir::separator() + "toolset" + QDir::separator();

        if (!QFile::exists(toolsetDirPath + name))
        {
            qDebug() << "zoffline不存在，下载";
            this->ui->label_zofflineStatus->setText("下载中");
            Utils::downloadFile(toolsetDirPath, name, url, [this]() {
                qDebug() << "zoffline下载成功";
                this->ui->label_zofflineStatus->setText("已下载");
                this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
            });
        }
        else
        {
            LogManager::instance().addLog("zoffline存在，无需更新");
            qDebug() << "zoffline存在，无需更新";
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

