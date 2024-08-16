#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QObject>
#include<QUdpSocket>
#include<QWidget>
#include<QToolTip>
#include<QMessageBox>
#include<QTcpSocket>
#include<QBrush>
#include<QTcpServer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket= new QUdpSocket(this);
    tcpSocket= new QTcpSocket(this);
    tcpServer= new QTcpServer(this);
    clientSocket=nullptr;

    //udp
    ui->BTN_CONNECT->setStyleSheet(connectStylesheet);
    ui->btn_SEND->setStyleSheet(disabledStlylesheet);
    ui->ipAdress_line->setStyleSheet(ipStylesheet);
    ui->spn_PORT->setStyleSheet(portStylesheet);
    ui->spinBox->setStyleSheet(portStylesheet);
    ui->btn_SEND->setEnabled(false);
    ui->ipAdress_line->setEnabled(true);
    ui->spinBox->setEnabled(true);
    ui->spn_PORT->setEnabled(true);
    //client
    ui->BTN_CONNECT_TCP->setStyleSheet(connectStylesheet);
    ui->btn_SEND_TCP->setStyleSheet(disabledStlylesheet);
    ui->ipAdress_TCP->setStyleSheet(ipStylesheet);
    ui->spn_PORT_TCP->setStyleSheet(portStylesheet);
    ui->btn_SEND_TCP->setEnabled(false);
    ui->ipAdress_TCP->setEnabled(true);
    ui->spn_PORT_TCP->setEnabled(true);
    //server
    ui->btn_listen->setStyleSheet(connectStylesheet);
    ui->btn_send_server->setStyleSheet(disabledStlylesheet);
    ui->spn_port_server->setStyleSheet(portStylesheet);
    ui->btn_send_server->setEnabled(false);
    ui->spn_port_server->setEnabled(true);


    connect(tcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(errorOccurredSlot(QAbstractSocket::SocketError)));

}
MainWindow::~MainWindow()
{
    delete ui;
}
//udp haberleşmesi
void MainWindow::on_BTN_CONNECT_clicked()
{
    QString ipAdress=ui->ipAdress_line->text();
    QHostAddress hostAdress(ipAdress);
    quint16 port =ui->spn_PORT->value();
    quint16 localport= ui->spinBox->value();

    if(ipAdress.isEmpty() || port == 0|| localport==0){
        QMessageBox messagebox;
        messagebox.setText("Please enter a valid IP adress and port number.");
        messagebox.setWindowTitle("Error");
        messagebox.setStyleSheet(MessageBox);
        messagebox.exec();//mesaj kutusunu göster
        return;
    }

    if(ui->BTN_CONNECT->text() == "DİSCONNECT")
    {
        connected= false;
        udpSocket->disconnectFromHost();
        disconnected();
        udpSocket->close();
    }
    else{
        if(!udpSocket->bind(QHostAddress::Any,localport)){
            QListWidgetItem*errorItem=new QListWidgetItem("Failed to bind to local port");
            errorItem->setForeground(QBrush(Qt::red));
            ui->console->addItem(errorItem);
            return;
        }

        ui->BTN_CONNECT->setText("DİSCONNECT");
        ui->BTN_CONNECT->setStyleSheet(disconnectStylesheet);
        ui->btn_SEND->setStyleSheet(sendStylesheet);
        ui->ipAdress_line->setStyleSheet(ipdisabledStylesheet);
        ui->spn_PORT->setStyleSheet(portdisabledStylesheet);
        ui->spinBox->setStyleSheet(portdisabledStylesheet);
        QListWidgetItem *item =new QListWidgetItem("UDP Socket created");
        item->setForeground(QBrush(Qt::green));
        ui->console->addItem(item);
        ui->btn_SEND->setEnabled(true);
        ui->ipAdress_line->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->spn_PORT->setEnabled(false);

        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));//sinyal gönderme
        connected=true;
        const char data[]="Succes";
        int size =sizeof(data)-1;
        QByteArray datagram(data,size);
        udpSocket->writeDatagram(datagram,QHostAddress(ipAdress),port);

    }
}
void MainWindow::send(QString message)
{
    QString ipAdress= ui->ipAdress_line->text();
    QHostAddress hostAdress(ipAdress);
    QByteArray datagrams= (message.toUtf8());// iletisim kurma(ag üzerinden veri gönderme)
    udpSocket->writeDatagram(datagrams,hostAdress,ui->spn_PORT->value());
    ui->listWidget->addItem(datagrams);
}


void MainWindow::on_btn_SEND_clicked()
{
    QString mesaj= ui->textEdit->toPlainText();
    if(mesaj.trimmed().isEmpty()){//mesaj yoksa ve boşluktan oluşuyorsa  bir şey yapma
        return;
    }
    send(mesaj);
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
    ui->spinBox->setStyleSheet(portStylesheet);
    QListWidgetItem *item =new QListWidgetItem("UDP Socket deleted");
    item->setForeground(QBrush(Qt::red));
    ui->console->addItem(item);
    ui->btn_SEND->setEnabled(false);
    ui->ipAdress_line->setEnabled(true);
    ui->spinBox->setEnabled(true);
    ui->spn_PORT->setEnabled(true);
}
//tcp client haberleşmesi
void MainWindow::on_BTN_CONNECT_TCP_clicked()
{
    QString ipAdress_TCP=ui->ipAdress_TCP->text();
    QHostAddress hostAdress(ipAdress_TCP);
    quint16 port_TCP =ui->spn_PORT_TCP->value();

    if(ipAdress_TCP.isEmpty() || port_TCP == 0){
        QMessageBox messagebox;
        messagebox.setText("Please enter a valid IP adress and port number.");
        messagebox.setWindowTitle("Error");
        messagebox.setStyleSheet(MessageBox);
        messagebox.exec();//mesaj kutusunu göster
        return;
    }

    if(ui->BTN_CONNECT_TCP->text() == "DİSCONNECT")
    {
        connected= false;
        tcpSocket->disconnectFromHost();
        tcp_disconnected();
        tcpSocket->close();
    }
    else {

        tcpSocket->connectToHost(QHostAddress(ipAdress_TCP),port_TCP);



        if(tcpSocket->waitForConnected(300)){
            tcpSocket->state()==QAbstractSocket::ConnectedState;
            ui->BTN_CONNECT_TCP->setText("DİSCONNECT");
            QListWidgetItem *item =new QListWidgetItem("Connected");
            item->setForeground(QBrush(Qt::green));
            ui->console_2->addItem(item);
            ui->BTN_CONNECT_TCP->setStyleSheet(disconnectStylesheet);
            ui->btn_SEND_TCP->setStyleSheet(sendStylesheet);
            ui->ipAdress_TCP->setStyleSheet(ipdisabledStylesheet);
            ui->spn_PORT_TCP->setStyleSheet(portdisabledStylesheet);
            ui->btn_SEND_TCP->setEnabled(true);
            ui->ipAdress_TCP->setEnabled(false);
            ui->spn_PORT_TCP->setEnabled(false);
            disconnect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::TCP_READY);
            connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::TCP_READY);
            connected= true;

        }
        else{
            ui->console_2->addItem("TCP Conncection Error");
        }


    }
}
void MainWindow::tcp_disconnected()
{
    ui->BTN_CONNECT_TCP->setText("CONNECT");
    ui->BTN_CONNECT_TCP->setStyleSheet(connectStylesheet);
    ui->btn_SEND_TCP->setStyleSheet(disabledStlylesheet);
    ui->ipAdress_TCP->setStyleSheet(ipStylesheet);
    ui->spn_PORT_TCP->setStyleSheet(portStylesheet);
    QListWidgetItem *item =new QListWidgetItem("Connection closed");
    item->setForeground(QBrush(Qt::red));
    ui->console_2->addItem(item);
    ui->btn_SEND_TCP->setEnabled(false);
    ui->ipAdress_TCP->setEnabled(true);
    ui->spn_PORT_TCP->setEnabled(true);
}

void MainWindow::on_btn_SEND_TCP_clicked()
{
    QString message_TCP= ui->textEdit_2->toPlainText();
    if(message_TCP.trimmed().isEmpty()){//mesaj yoksa ve boşluktan oluşuyorsa  bir şey yapma
        return;
    }
    if(tcpSocket->state()==QAbstractSocket::ConnectedState){//bağlantı varsa sent data kısmına yaz
        send_TCP(message_TCP);
        ui->listWidget_2->addItem(message_TCP);
    }
}
void MainWindow::send_TCP(QString message1)
{
    tcpSocket->write(message1.toUtf8());// iletisim kurma(ag üzerinden veri gönderme)
}
void MainWindow::TCP_READY()
{
    QByteArray data;
    if(sender()== clientSocket){
        data=clientSocket->readAll();
        ui->console_3_Server->addItem(data);
    }
    if(sender()== tcpSocket){
        data= tcpSocket->readAll();
        ui->console_2->addItem(data);
    }
}


//tcp server haberleşmesi

void MainWindow::on_btn_listen_clicked()
{
    quint16 port_TCP_SERVER =ui->spn_port_server->value();

    if(port_TCP_SERVER == 0){
        QMessageBox messagebox;
        messagebox.setText("Please enter a valid IP adress and port number.");
        messagebox.setWindowTitle("Error");
        messagebox.setStyleSheet(MessageBox);
        messagebox.exec();//mesaj kutusunu göster
        return;
    }
    if(ui->btn_listen->text() == "CLOSE")
    {
        connected= false;
        if(clientSocket){
          clientSocket->disconnectFromHost();
          clientSocket->close();
        }
        tcp_SERVER_disconnected();
        tcpServer->close();
    }
    else {
        if(!tcpServer->listen(QHostAddress::Any,port_TCP_SERVER)){
           ui->console_3_Server->addItem("failed to tcp start the server.");
        }
        ui->btn_listen->setText("CLOSE");
        ui->btn_listen->setStyleSheet(disconnectStylesheet);
        ui->btn_send_server->setStyleSheet(sendStylesheet);
        ui->spn_port_server->setStyleSheet(portdisabledStylesheet);
        ui->btn_send_server->setEnabled(true);
        ui->spn_port_server->setEnabled(false);
        disconnect(tcpServer,&QTcpServer::newConnection,this,&MainWindow::acceptConnection);
        connect(tcpServer,&QTcpServer::newConnection,this,&MainWindow::acceptConnection);
        connected= true;

    }
}
void MainWindow::acceptConnection()
{
    clientSocket =tcpServer->nextPendingConnection();//yeni nesne oluşturuldu
    connect(clientSocket,&QTcpSocket::readyRead,this,&MainWindow::TCP_READY);
    QListWidgetItem *item =new QListWidgetItem("Client connected");
    item->setForeground(QBrush(Qt::green));
    ui->console_3_Server->addItem(item);
}
void MainWindow::tcp_SERVER_disconnected()
{
    ui->btn_listen->setText("LISTEN");
    ui->btn_listen->setStyleSheet(connectStylesheet);
    ui->btn_send_server->setStyleSheet(disabledStlylesheet);
    ui->spn_port_server->setStyleSheet(portStylesheet);
    QListWidgetItem *item =new QListWidgetItem("All connections closed");
    item->setForeground(QBrush(Qt::red));
    ui->console_3_Server->addItem(item);
    ui->btn_send_server->setEnabled(false);
    ui->spn_port_server->setEnabled(true);
}
void MainWindow::on_btn_send_server_clicked()
{
    QString message_TCP_SERVER= ui->textEdit_3_Server->toPlainText();
    if(message_TCP_SERVER.trimmed().isEmpty()){//mesaj yoksa ve boşluktan oluşuyorsa  bir şey yapma
        return;
    }
    if(clientSocket->state()==QAbstractSocket::ConnectedState){//bağlantı varsa sent data kısmına yaz
        send_TCP_SERVER(message_TCP_SERVER);
        ui->listWidget_3_Server->addItem(message_TCP_SERVER);
    }
}

void MainWindow::errorOccurredSlot(QAbstractSocket::SocketError error)
{
   qDebug() << "ERROR OCCURED!!!!! - " << error;
   tcp_disconnected();
}
void MainWindow::send_TCP_SERVER(QString message2)
{
    clientSocket->write(message2.toUtf8());
}
