#include "mirrormanager.h"

MirrorManager::MirrorManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MirrorManager)
{
    ui->setupUi(this);
}

MirrorManager::~MirrorManager()
{
    delete ui;
}

void MirrorManager::on_pushButton_done_clicked()
{
    QString mirror;
    int index = ui->comboBox->currentIndex();
    if (index == 0) {
        mirror = "github";
    } else if (index == 1) {
        mirror = "ghproxy";
    }
    ConfigManager::instance().setMirror(mirror);
    ConfigManager::instance().save();

    accept();
}

void MirrorManager::loadMirror()
{
    ConfigManager::instance().load();
    QString mirror = ConfigManager::instance().getMirror();

    if (mirror == "github") {
        ui->comboBox->setCurrentIndex(0);
    } else if (mirror == "ghproxy") {
        ui->comboBox->setCurrentIndex(1);
    }
}

