#ifndef MIRRORMANAGER_H
#define MIRRORMANAGER_H

#include <QDialog>

#include "configmanager/configmanager.h"
#include "ui_mirrormanager.h"

namespace Ui {
class MirrorManager;
}

class MirrorManager : public QDialog
{
    Q_OBJECT

public:
    explicit MirrorManager(QWidget *parent = nullptr);
    ~MirrorManager();

private slots:
    void on_pushButton_done_clicked();

private:
    Ui::MirrorManager *ui;

public:
    void loadMirror();
};

#endif // MIRRORMANAGER_H
