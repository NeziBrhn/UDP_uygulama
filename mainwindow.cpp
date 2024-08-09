#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MY_UDP.h"
#include<QObject>
#include<QUdpSocket>
#include<QWidget>
#include<QToolTip>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->BTN_CONNECT->setStyleSheet(connectStylesheet);
    ui->btn_SEND->setStyleSheet(disabledStlylesheet);
    ui->ipAdress_line->setStyleSheet(ipStylesheet);
    ui->spn_PORT->setStyleSheet(portStylesheet);
    udpSocket= new QUdpSocket(this);
    ui->btn_SEND->setEnabled(false);

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_BTN_CONNECT_clicked()
{
    if(ui->BTN_CONNECT->text() == "DİSCONNECT")
    {
        connected= false;
        udpSocket->disconnected();
        disconnected();
        udpSocket->close();
    }
    else{
        QString ipAdress=ui->ipAdress_line->text();
        quint16 port =ui->spn_PORT->value();
        if(ipAdress.isEmpty() || port == 0){
            QMessageBox:: warning(this,"Error","Please enter a valid IP adress and port number.");
            return;
        }

    ui->BTN_CONNECT->setText("DİSCONNECT");
    ui->BTN_CONNECT->setStyleSheet(disconnectStylesheet);
    ui->btn_SEND->setStyleSheet(sendStylesheet);
    ui->ipAdress_line->setStyleSheet(ipdisabledStylesheet);
    ui->spn_PORT->setStyleSheet(portdisabledStylesheet);
    QListWidgetItem *item =new QListWidgetItem("UDP Socket created");
    item->setForeground(QBrush(Qt::green));
    ui->console->addItem(item);
    ui->btn_SEND->setEnabled(true);
    ui->ipAdress_line->text();
    QHostAddress hostAdress(ipAdress);
    udpSocket->bind(hostAdress,ui->spn_PORT->value());//dinleme ve port seçimi
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));//sinyal gönderme
    if(!connected){
        const char data[]="Succes";
        int size =sizeof(data)-1;
        QByteArray datagram(data,size);
        udpSocket->writeDatagram(datagram,hostAdress,ui->spn_PORT->value());
        }
    }
}
void MainWindow::readPendingDatagrams(){
    while(false!=udpSocket->hasPendingDatagrams()){
        QByteArray message;
        message.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(message.data(),message.size());
        ui->console->addItem(QString::fromUtf8(message));
    }
}
void MainWindow::disconnected()
{
    ui->BTN_CONNECT->setText("CONNECT");
    ui->BTN_CONNECT->setStyleSheet(connectStylesheet);
    ui->btn_SEND->setStyleSheet(disabledStlylesheet);
    ui->ipAdress_line->setStyleSheet(ipStylesheet);
    ui->spn_PORT->setStyleSheet(portStylesheet);
    QListWidgetItem *item =new QListWidgetItem("UDP Socket deleted");
    item->setForeground(QBrush(Qt::red));
    ui->console->addItem(item);
    ui->btn_SEND->setEnabled(false);
}
void MainWindow::on_btn_SEND_clicked()
{
    QString mesaj= ui->textEdit->toPlainText();
    if(mesaj.trimmed().isEmpty()){//mesaj yoksa ve boşluktan oluşuyorsa  bir şey yapma
        return;
    }
    send(mesaj);
}
void MainWindow::send(QString message)
{
    QString ipAdress= ui->ipAdress_line->text();
    QHostAddress hostAdress(ipAdress);
    QByteArray datagrams= (message.toUtf8());// iletisim kurma(ag üzerinden veri gönderme)
    udpSocket->writeDatagram(datagrams,hostAdress,ui->spn_PORT->value());
    ui->listWidget->addItem(datagrams);
}
