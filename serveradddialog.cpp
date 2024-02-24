#include "serveradddialog.h"
#include "ui_serveradddialog.h"

#include <QMessageBox>
#include <QAbstractButton>

ServerAddDialog::ServerAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerAddDialog)
{
    ui->setupUi(this);
}

ServerAddDialog::~ServerAddDialog()
{
    delete ui;
}

void ServerAddDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::Cancel) == button)
    {
        done(Rejected);
    }

    QString serverAlias = ui->lineEdit_serverAlias->text();
    QString serverAddr = ui->lineEdit_serverAddr->text();

    if (serverAlias.isEmpty() or serverAddr.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入内容！");
        return;
    }


}

