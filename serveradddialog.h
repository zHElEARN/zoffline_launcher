#ifndef SERVERADDDIALOG_H
#define SERVERADDDIALOG_H

#include <QDialog>

namespace Ui {
class ServerAddDialog;
}

class ServerAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerAddDialog(QWidget *parent = nullptr);
    ~ServerAddDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ServerAddDialog *ui;
};

#endif // SERVERADDDIALOG_H
