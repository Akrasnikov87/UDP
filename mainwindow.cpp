#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("UDP Client Server");

    QSettings settings("fileSettingsUDP/settings.ini",QSettings::IniFormat);
    settings.beginGroup("IP");
    ui->lineEdit_2->setText(settings.value("ip").toString());
    settings.endGroup();
    settings.beginGroup("PORT");
    ui->lineEdit_3->setText(settings.value("port").toString());
    settings.endGroup();
    settings.beginGroup("MEAS");
    ui->lineEdit->setText(settings.value("meas").toString());
    settings.endGroup();

    QStringList stringList;
    stringList << "LN" << "CR" << "LN CR" << "CR LN" << "NONE";
    ui->comboBox->addItems(stringList);

    connect (ui->lineEdit_2,&QLineEdit::textChanged,[&](){
        updSocket->close();

        QSettings settings("fileSettingsUDP/settings.ini",QSettings::IniFormat);
        settings.beginGroup("IP");
        settings.setValue("ip",ui->lineEdit_2->text());
        settings.endGroup();

        updSocket->bind(QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toUShort());
    });
    connect (ui->lineEdit_3,&QLineEdit::textChanged,[&](){
        updSocket->close();

        QSettings settings("fileSettingsUDP/settings.ini",QSettings::IniFormat);
        settings.beginGroup("PORT");
        settings.setValue("port",ui->lineEdit_3->text());
        settings.endGroup();

        updSocket->bind(QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toUShort());
    });
    connect (ui->lineEdit,&QLineEdit::textChanged,[&](){
        QSettings settings("fileSettingsUDP/settings.ini",QSettings::IniFormat);
        settings.beginGroup("MEAS");
        settings.setValue("meas",ui->lineEdit->text());
        settings.endGroup();
    });

    updSocket->bind(QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toUShort());

    connect (ui->pushButton,&QPushButton::clicked,[&](){
        /*pAnimation = new QPropertyAnimation(this,"pos");
        pAnimation->setDuration(500);
        pAnimation->setLoopCount(2);
        pAnimation->setKeyValueAt(0,QPoint(geometry().x()-3,geometry().y()-3));
        pAnimation->setKeyValueAt(0.1,QPoint(geometry().x()+6,geometry().y()+6));
        pAnimation->setKeyValueAt(0.2,QPoint(geometry().x()-6,geometry().y()+6));
        pAnimation->setKeyValueAt(0.3,QPoint(geometry().x()+6,geometry().y()-6));
        pAnimation->setKeyValueAt(0.4,QPoint(geometry().x()-6,geometry().y()-6));
        pAnimation->setKeyValueAt(0.5,QPoint(geometry().x()+6,geometry().y()+6));
        pAnimation->setKeyValueAt(0.6,QPoint(geometry().x()-6,geometry().y()+6));
        pAnimation->setKeyValueAt(0.7,QPoint(geometry().x()+6,geometry().y()-6));
        pAnimation->setKeyValueAt(0.8,QPoint(geometry().x()-6,geometry().y()-6));
        pAnimation->setKeyValueAt(0.9,QPoint(geometry().x()+6,geometry().y()+6));
        pAnimation->setKeyValueAt(1,QPoint(geometry().x()-3,geometry().y()-3));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);*/

        QByteArray dataGram;
        if (ui->comboBox->currentIndex() == 0)
            dataGram = ui->lineEdit->text().toLocal8Bit().append('\n');
        if (ui->comboBox->currentIndex() == 1)
            dataGram = ui->lineEdit->text().toLocal8Bit().append('\r');
        if (ui->comboBox->currentIndex() == 2)
            dataGram = ui->lineEdit->text().toLocal8Bit().append('\n').append('\r');
        if (ui->comboBox->currentIndex() == 3)
            dataGram = ui->lineEdit->text().toLocal8Bit().append('\r').append('\n');
        if (ui->comboBox->currentIndex() == 4)
            dataGram = ui->lineEdit->text().toLocal8Bit();

        //updSocket->writeDatagram(dataGram,dataGram.size(),QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toUShort());
        updSocket->writeDatagram(dataGram,QHostAddress("169.254.135.34"),5025);
    });

    connect (updSocket,&QUdpSocket::readyRead,[&](){
        QHostAddress address;
        unsigned short port;

        while (updSocket->hasPendingDatagrams()) {
            QByteArray datagrams;
            datagrams.resize(updSocket->pendingDatagramSize());
            updSocket->readDatagram(datagrams.data(),datagrams.size(),&address,&port);
            ui->textEdit->append(QString(datagrams));
        }
    });
}
MainWindow::~MainWindow() { delete ui; }
