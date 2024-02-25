#ifndef SERVERLISTMANAGER_H
#define SERVERLISTMANAGER_H

#include <QDialog>
#include <QWidget>

#include "ui_serverlistmanager.h"

namespace Ui {
    class ServerListManager;
}

class ServerListManager : public QDialog
{
    Q_OBJECT

public:
    ServerListManager(QWidget *parent = nullptr);
    ~ServerListManager();

private:
    Ui::ServerListManager *ui;
};

#endif // SERVERLISTMANAGER_H
