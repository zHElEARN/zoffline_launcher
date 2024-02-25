#ifndef SERVERLISTMANAGER_H
#define SERVERLISTMANAGER_H

#include <QDialog>
#include <QWidget>
#include <QInputDialog>

#include "configmanager/configmanager.h"

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

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_done_clicked();

private:
    Ui::ServerListManager *ui;

private:
    void loadServers();
};

#endif // SERVERLISTMANAGER_H
