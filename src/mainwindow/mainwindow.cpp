#include "../logger/logger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    appPath = QApplication::applicationDirPath();
    toolsetPath = QDir::cleanPath(appPath + QDir::separator() + "toolset");
    configPath = QDir::cleanPath(appPath + QDir::separator() + "config");
    configureClientPath = toolsetPath + QDir::separator() + "configure_client.bat";

    isZwiftInstalled = Utils::getZwiftInstallLocation(zwiftPath);
    zwiftPath = QDir::cleanPath(zwiftPath);
    zwiftLauncherPath = zwiftPath + QDir::separator() + "ZwiftLauncher.exe";

    Logger::instance().setOutputWidget(ui->textBrowser_mainLog);

    initialize();

    ConfigManager::instance().initialize("config.json", configPath);

    updateServerList();

    serverListManagerDialog = new ServerListManager(this);
    mirrorManager = new MirrorManager(this);
}

void MainWindow::initialize()
{
    // 检测工具文件夹是否存在
    QDir toolsetDir(toolsetPath);
    if (!toolsetDir.exists()) {
        if (!toolsetDir.mkpath(".")) QMessageBox::critical(nullptr, "错误", "无法创建toolset文件夹");
    }

    // 检测配置文件文件夹是否存在
    QDir configDir(configPath);
    if (!configDir.exists()) {
        if (!configDir.mkpath(".")) QMessageBox::critical(nullptr, "错误", "无法创建config文件夹");
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
        ui->label_zwiftVersion->setText(zwiftVersion);
        ui->label_zwiftVersion->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
    }

    // 获取zoffline信息
    Logger::instance().info("正在获取Zoffline信息");
    Utils::getLatestZofflineInfo([this](const QString &name, const QString &url, qint64 size) {
        Logger::instance().info("Zoffline最新: ", name);
        Logger::instance().info("Zoffline链接: ", url);
        Logger::instance().info("Zoffline大小: ", QString::number(size));

        zofflineFileName = name;
        zofflineURL = url;
        zofflinePath = toolsetPath + QDir::separator() + zofflineFileName;

        isZofflineInstalled = QFile::exists(zofflinePath);

        auto activateZofflineStatus = [this]() {
            this->ui->pushButton_launch->setEnabled(true);
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
        };

        if (isZofflineInstalled) {
            QFileInfo fileInfo(zofflinePath);
            qint64 fileSize = fileInfo.size();

            Logger::instance().info("本地Zoffline大小: ", QString::number(fileSize));
            if (fileSize != size) {
                isZofflineInstalled = false;
                Logger::instance().warn("Zoffline大小不匹配");
            }
        }

        if (isZofflineInstalled) {
            Logger::instance().info("Zoffline已经是最新版本，无需更新");
            ui->pushButton_downloadZoffline->setEnabled(false);
            activateZofflineStatus();
        } else {
            this->ui->label_zofflineStatus->setText("未安装");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(255, 0, 0); }");
        }
        zofflineVersion = Utils::parseZofflineVersion(name);
        Logger::instance().info("Zoffline版本: ", zofflineVersion);
        ui->label_zofflineVersion->setText(zofflineVersion);
        ui->label_zofflineVersion->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
    });
}

void MainWindow::updateServerList()
{
    ui->comboBox_customServer->clear();
    QJsonObject servers = ConfigManager::instance().getServers();
    for (auto it = servers.begin(); it != servers.end(); ++it) {
        QString serverInfo = it.key() + " [" + it.value().toString() + "]";
        QMap<QString, QString> serverData;
        serverData.insert(it.key(), it.value().toString());
        ui->comboBox_customServer->addItem(serverInfo, QVariant::fromValue(serverData));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (this->isLaunched) {
        QMessageBox::warning(this, "警告", "请先停止后再退出程序", QMessageBox::Ok);
        event->ignore();
    } else {
        event->accept();
    }
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


void MainWindow::on_pushButton_launch_clicked()
{
    if (!isZwiftInstalled) {
        QMessageBox::warning(nullptr, "警告", "未安装Zwift，无法启动");
        return;
    }

    int connectMethodIndex = ui->comboBox_connectMethod->currentIndex();
    bool isLocalServer = connectMethodIndex == 0;
    bool isCustomServer = connectMethodIndex == 1;

    if (isLocalServer && !isZofflineInstalled) {
        QMessageBox::warning(nullptr, "警告", "未安装Zoffline，无法启动");
        return;
    }

    ui->pushButton_launch->setEnabled(false);
    ui->comboBox_connectMethod->setEditable(false);

    if (isLocalServer) {
        // 版本检查，目前仅检查
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
        Logger::instance().info("配置客户端日志：" + output);

        Logger::instance().info("写入hosts", "服务器地址: 127.0.0.1 [本地]");
        hosts.load();
        hosts.backup();
        hosts.addHosts("127.0.0.1", zwiftHostnames);
        hosts.save();

        auto handleZofflineOutput = [this]() {
            QByteArray standardOutput = zofflineProcess.readAllStandardOutput();
            QString standardOutputStr = QString::fromLocal8Bit(standardOutput);

            QByteArray errorOutput = zofflineProcess.readAllStandardError();
            QString errorOutputStr = QString::fromLocal8Bit(errorOutput);

            // 合并标准输出和错误输出
            QString outputStr = standardOutputStr + errorOutputStr;

            this->ui->textBrowser_zofflineLog->append(outputStr);

            if (outputStr.contains("is running.")) {
                Logger::instance().info("Zoffline已启动");
                QDesktopServices::openUrl(QUrl("file:///" + zwiftLauncherPath, QUrl::TolerantMode));
            }
        };

        connect(&zofflineProcess, &QProcess::readyReadStandardOutput, this, [handleZofflineOutput]() {
            handleZofflineOutput();
        });
        connect(&zofflineProcess, &QProcess::readyReadStandardError, this, [handleZofflineOutput]() {
            handleZofflineOutput();
        });

        // 启动Zoffline
        zofflineProcess.start(zofflinePath);
    } else if (isCustomServer) {
        // 自定义服务器
        QMap<QString, QString> selectedServer = ui->comboBox_customServer->currentData().value<QMap<QString, QString>>();

        QString alias = selectedServer.firstKey();
        QString address = selectedServer.first();

        Logger::instance().info("写入hosts","服务器地址:", address, QString("[%1]").arg(alias));
        hosts.load();
        hosts.backup();
        hosts.addHosts(address, zwiftHostnames);
        hosts.save();
        QDesktopServices::openUrl(QUrl("file:///" + zwiftLauncherPath, QUrl::TolerantMode));
    }

    ui->pushButton_stop->setEnabled(true);
    isLaunched = true;
    ui->label_launcherStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
    ui->label_launcherStatus->setText("已启动");
}


void MainWindow::on_pushButton_stop_clicked()
{
    if (Utils::isZwiftProcessExists()) {
        QMessageBox terminateTip;
        terminateTip.setText("检测到Zwift正在运行，是否停止？（会终止Zwift）");
        terminateTip.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        terminateTip.setDefaultButton(QMessageBox::Cancel);
        int option = terminateTip.exec();
        if (option ==  QMessageBox::Cancel) {
            return;
        }
    }

    if (ui->comboBox_connectMethod->currentIndex() == 0) {
        // 本地服务器
        hosts.restore();
    } else if (ui->comboBox_connectMethod->currentIndex() == 1) {
        // 官方服务器
    } else {
        // 自定义服务器
        hosts.restore();
    }

    zofflineProcess.kill();
    Utils::terminate(zofflineFileName);

    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_launch->setEnabled(true);
    isLaunched = false;
    ui->label_launcherStatus->setStyleSheet("QLabel { color: rgb(255, 0, 0); }");
    ui->label_launcherStatus->setText("未启动");
}


void MainWindow::on_action_manage_triggered()
{
    serverListManagerDialog->loadServers();
    serverListManagerDialog->exec();
    updateServerList();
}


void MainWindow::on_action_mirror_triggered()
{
    mirrorManager->loadMirror();
    mirrorManager->exec();
}


void MainWindow::on_pushButton_downloadZoffline_clicked()
{
    if (!isZofflineInstalled) {
        Logger::instance().info("未下载Zoffline，开始下载");
        this->ui->pushButton_downloadZoffline->setEnabled(false);
        this->ui->label_zofflineStatus->setText("下载中");
        this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(170, 85, 0); }");

        QString mirror = ConfigManager::instance().getMirror();
        QString url;
        if (mirror == "github") {
            url = zofflineURL;
        } else if (mirror == "ghproxy") {
            url = "https://mirror.ghproxy.com/" + zofflineURL;
        } else if (mirror == "zhe_learn ecs") {
            url = "https://zoffline-sync.zhelearn.com/downloads/" + zofflineFileName;
        }

        Utils::downloadFile(toolsetPath, zofflineFileName, url, [this]() {
            Logger::instance().info("Zoffline下载成功");
            this->ui->pushButton_launch->setEnabled(true);
            this->ui->label_zofflineStatus->setText("已下载");
            this->ui->label_zofflineStatus->setStyleSheet("QLabel { color: rgb(0, 255, 0); }");
            isZofflineInstalled = true;
            ui->pushButton_downloadZoffline->setEnabled(false);
        });
    }
}

