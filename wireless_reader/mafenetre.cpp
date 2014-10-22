//#pragma comment(lib, "ODALID.lib") // a utiliser si on utilise MSVC
#include "mafenetre.h"
#include <cstring>

MaFenetre::MaFenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaFenetre)
{
    ui->setupUi(this);
    status = 0;
    key_index = 0;
}

MaFenetre::~MaFenetre()
{
    delete ui;
}


//char pszHost[] = "192.168.1.4";

void MaFenetre::on_connect_clicked()
{
    char version[30];
    uint8_t serial[4];
    char stackReader[20];
    //MonLecteur.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    reader.Type = ReaderCDC;
    reader.device = 0;
    status = OpenCOM1(&reader);
    qDebug () << "OpenCOM1" << status;

    if (!status)
    {
        // Get reader name
        status = Version(&reader, version, serial, stackReader);
        ui->readerName->setText(version);
        ui->readerName->update();
        // Activate RF field (mise sous tension)
        RF_Power_Control(&reader, TRUE, 0);

        // Loading keys sector 2
        // Key A
        qDebug() << "Code d'erreur LoadKey A : " << Mf_Classic_LoadKey(&reader, TRUE, key_ff, key_index);
        // Key B
        qDebug() << "Code d'erreur LoadKey B : " << Mf_Classic_LoadKey(&reader, FALSE, key_ff, key_index);

        // Loading keys sector 3
        // Key A
        qDebug() << "Code d'erreur LoadKey A : " << Mf_Classic_LoadKey(&reader, TRUE, key_ff, key_index);
        // Key B
        qDebug() << "Code d'erreur LoadKey B : " << Mf_Classic_LoadKey(&reader, FALSE, key_ff, key_index);

        // Poll
        poll();

        readCard();
    }
}

void MaFenetre::readCard()
{
    // Sector number and data
    uint8_t block = 0;
    uint8_t data[16];
    char tmp[16];

    // Authentification
    qDebug() << "Code d'erreur authentification : " << Mf_Classic_Authenticate(&reader, Auth_KeyB, false, block, key_ff, key_index);

    //  Prenom
    block = 9;
    qDebug() << "Code d'erreur read block " << block << " : " << Mf_Classic_Read_Block(&reader, true, block, data, Auth_KeyA, key_index);
    for (int i = 0; i < 16; i++) {
          tmp[i] = (char)data[i];
    }
    ui->userFirstNameEditor->setText(QString(tmp));

    //  Nom
    block = 10;
    qDebug() << "Code d'erreur read block " << block << " : " << Mf_Classic_Read_Block(&reader, true, block, data, Auth_KeyA, key_index);
    for (int i = 0; i < 16; i++) {
          tmp[i] = (char)data[i];
    }
    ui->userNameEditor->setText(QString(tmp));

    //  NbVoyages
    block = 10;
    qDebug() << "Code d'erreur read block " << block << " : " << Mf_Classic_Read_Block(&reader, true, block, data, Auth_KeyA, key_index);
    for (int i = 0; i < 16; i++) {
          tmp[i] = (char)data[i];
    }
    ui->userNameEditor->setText(QString(tmp));
}

void MaFenetre::on_quit_clicked()
{
    RF_Power_Control(&reader, FALSE, 0);
    status = LEDBuzzer(&reader, LED_OFF);
    status = CloseCOM1(&reader);
    qApp->quit();
}

void MaFenetre::on_saveParameters_clicked()
{
    const char *tmp;
    uint8_t *data = new uint8_t[16];
    uint8_t block;

    // Authentification
    qDebug() << "Code d'erreur authentification : " << Mf_Classic_Authenticate(&reader, Auth_KeyB, false, block, key_ff, key_index);

    // NOM
    tmp = ui->userNameEditor->text().toStdString().c_str();
    for (int i = 0; i < 16; i++) {
          data[i] = (uint8_t)tmp[i];
    }
    block = 10;
    qDebug() << "Code d'erreur write : " << Mf_Classic_Write_Block(&reader, true, block, data, Auth_KeyA, key_index);

    // PRENOM
    tmp = ui->userFirstNameEditor->text().toStdString().c_str();
    for (int i = 0; i < 16; i++) {
          data[i] = (uint8_t)tmp[i];
    }
    block = 9;
    qDebug() << "Code d'erreur write : " << Mf_Classic_Write_Block(&reader, true, block, data, Auth_KeyA, key_index);

    // NB Voyages
    qDebug() << "Code d'erreur authentification : " << Mf_Classic_Authenticate(&reader, Auth_KeyB, false, block, key_ff, key_index);
    tmp = ui->countTripEditor->text().toStdString().c_str();
    for (int i = 0; i < 16; i++) {
          data[i] = (uint8_t)tmp[i];
    }
    block = 14;
    qDebug() << "Code d'erreur write : " << Mf_Classic_Write_Block(&reader, true, block, data, Auth_KeyA, key_index);

}

void MaFenetre::on_userFirstNameEditor_textEdited(const QString &text)
{
    qDebug() << "Text : " << text;
}

void MaFenetre::on_userNameEditor_textEdited(const QString &text)
{
    qDebug() << "Text : " << text;
}

void MaFenetre::on_readCard_clicked()
{
    readCard();
}

void MaFenetre::poll() {
    uint8_t atq[2];
    uint8_t sak[1];
    uint16_t uid_len = 12;
    qDebug() << "Code d'erreur poll : " << ISO14443_3_A_PollCard(&reader, atq, sak, uid, &uid_len);

    // Data of Poll
    qDebug() << "Tag found: UID = ";
    for (int i = 0; i < uid_len; i++)
        qDebug() << uid[i];
    qDebug() << " ATQ =" << atq[1] << atq[0];
    qDebug() << " SAK =" << sak[0] << "\n";
}
