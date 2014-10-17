#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QWidget>

namespace Ui {
class MaFenetre;
}

class MaFenetre : public QWidget
{
    Q_OBJECT

public:
    explicit MaFenetre(QWidget *parent = 0);
    ~MaFenetre();

private:
    Ui::MaFenetre *ui;

private slots:
    void on_Connect_clicked();
    void on_saisie_button_clicked();
    void on_Quit_clicked();
};

#endif // MAFENETRE_H
