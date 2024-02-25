#include "serverlistmanager.h"

ServerListManager::ServerListManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerListManager)
{
    ui->setupUi(this);
}

ServerListManager::~ServerListManager()
{
    delete ui;
}
