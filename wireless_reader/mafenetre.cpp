//#pragma comment(lib, "ODALID.lib") // a utiliser si on utilise MSVC
#include "mafenetre.h"

MaFenetre::MaFenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaFenetre)
{
    ui->setupUi(this);
    status = 0;
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
        readCard();
    }
}

void MaFenetre::readCard()
{
    unsigned char key_ff[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    BYTE key_index=0;

    // Key A
    status = Mf_Classic_LoadKey(&reader, TRUE, key_ff, key_index);
    // Key B
    status = Mf_Classic_LoadKey(&reader, FALSE, key_ff, key_index);

    // Activate RF field (mise sous tension)
    RF_Power_Control(&reader, TRUE, 0);

    // Connection to card
    uint8_t *name = NULL;
    uint8_t atq[2];
    uint8_t sak[1];
    uint16_t uid_len = 12;
    qDebug() << "Code d'erreur poll : " << ISO14443_3_A_PollCard(&reader, atq, sak, uid, &uid_len);

    // Data of Poll // enlever printf
    printf("Tag found: UID = ");
    for (int i = 0; i < uid_len; i++)
        printf("%02X ", uid[i]);
    printf(" ATQ=%02X%02X SAK=%02X\n", atq[1], atq[0], sak[0]);

    // Get firstname and family name of user // Probleme
    qDebug() << "Code d'erreur read : " << Mf_Classic_Read_Block(&reader, true, 10, name, Auth_KeyA, 0);
    qDebug() << "Name : " << name;

    // Get number of trip

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
