#include "serverlistmanager.h"

ServerListManager::ServerListManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerListManager)
{
    ui->setupUi(this);
}

void ServerListManager::loadServers()
{
    ConfigManager::instance().load();

    QJsonObject servers = ConfigManager::instance().getServers();
    ui->tableWidget->setRowCount(servers.size());

    int row = 0;
    for (auto it = servers.begin(); it != servers.end(); ++it) {
        QTableWidgetItem *aliasItem = new QTableWidgetItem(it.key());
        QTableWidgetItem *addressItem = new QTableWidgetItem(it.value().toString());
        ui->tableWidget->setItem(row, 0, aliasItem);
        ui->tableWidget->setItem(row, 1, addressItem);
        row++;
    }
}

ServerListManager::~ServerListManager()
{
    delete ui;
}

void ServerListManager::on_pushButton_add_clicked()
{
    bool ok;
    QString alias = QInputDialog::getText(this, "添加服务器", "服务器别名:", QLineEdit::Normal, "", &ok);
    if (ok && !alias.isEmpty()) {
        QString address = QInputDialog::getText(this, "添加服务器", "服务器地址:", QLineEdit::Normal, "", &ok);
        if (ok && !address.isEmpty()) {
            ConfigManager::instance().addServer(alias, address);
            ConfigManager::instance().save();
            loadServers();
        }
    }
}


void ServerListManager::on_pushButton_delete_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow != -1) {
        QString alias = ui->tableWidget->item(currentRow, 0)->text();
        ConfigManager::instance().removeServer(alias);
        ConfigManager::instance().save();
        loadServers();
    }
}


void ServerListManager::on_pushButton_done_clicked()
{
    accept();
}

