#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QWidget>
#include "ui_mafenetre.h"
#include "ODALID.h"
#include "stdlib.h"
#include <QtGui>

namespace Ui {
class MaFenetre;
}

class MaFenetre : public QWidget
{
    Q_OBJECT

public:
    explicit MaFenetre(QWidget *parent = 0);
    ~MaFenetre();
    void readCard();
    void poll();

private:
    Ui::MaFenetre *ui;
    int16_t status;
    uint8_t uid[12];
    ReaderName reader;
    unsigned char key_ff[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t key_index;

private slots:
    void on_connect_clicked();
    void on_quit_clicked();
    void on_saveParameters_clicked();
    void on_userFirstNameEditor_textEdited(const QString &arg1);
    void on_userNameEditor_textEdited(const QString &arg1);
    void on_readCard_clicked();
};

#endif // MAFENETRE_H
