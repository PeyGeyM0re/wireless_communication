//#pragma comment(lib, "ODALID.lib") // a utiliser si on utilise MSVC
#include "mafenetre.h"
#include "ui_mafenetre.h"
#include "ODALID.h"
#include <QtGui>

MaFenetre::MaFenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaFenetre)
{
    ui->setupUi(this);
}

MaFenetre::~MaFenetre()
{
    delete ui;
}

ReaderName MonLecteur;

char pszHost[] = "192.168.1.4";

void MaFenetre::on_Connect_clicked()
{
    int16_t status = 0;
    //MonLecteur.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM1(&MonLecteur);
    qDebug () << "OpenCOM1" << status;

    if (!status)
    {
        char version[30];
        uint8_t serial[4];
        char stackReader[20];
        status = Version(&MonLecteur, version, serial, stackReader);
        ui->Affichage->setText(version);
        ui->Affichage->update();
    }
}

void MaFenetre::on_saisie_button_clicked()
{
    QString Text = ui->saisie_texteditor->toPlainText();
    qDebug() << "Text : " << Text;
}

void MaFenetre::on_Quit_clicked()
{
    int16_t status = 0;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = LEDBuzzer(&MonLecteur, BUZZER_ON);
    status = CloseCOM1(&MonLecteur);
    qApp->quit();
}
