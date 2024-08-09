#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MY_UDP.h"
#include <QMainWindow>
#include<QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void readPendingDatagrams();
    void disconnected();
    void send(QString message);
private slots:
    void on_BTN_CONNECT_clicked();

    void on_btn_SEND_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    MY_UDP *_socket;
    bool connected;

    const QString connectStylesheet =        "QPushButton {"
                                             "    background-color: #0baa10;"  // Başlangıçta yeşil arka plan rengi
                                             "    color: white;"               // Metin rengi
                                             "    border-radius: 5px;"         // Yuvarlak köşeler
                                             "    font-size: 14px;"            // Yazı tipi boyutu
                                             "}"
                                             ""
                                             "QPushButton::pressed{"
                                             "    background-color: #077609;"  // Basılıyken koyu yeşil arka plan rengi
                                             "    color: white;"               // Metin rengi
                                             "    border-radius: 5px;"         // Yuvarlak köşeler
                                             "    font-size: 14px;"            // Yazı tipi boyutu
                                             "}";

    const QString disconnectStylesheet=        "QPushButton {"
                                               "    background-color: #aa1a10;"  // disconnect iken kırmızı arka plan rengi
                                               "    color: white;"               // Metin rengi
                                               "    border-radius: 5px;"         // Yuvarlak köşeler
                                               "    font-size: 14px;"            // Yazı tipi boyutu
                                               "}"
                                                ""
                                               "QPushButton::pressed{"
                                               "    background-color: #8c140d;"  // Basılıyken koyu kırmızı arka plan rengi
                                               "    color: white;"               // Metin rengi
                                               "    border-radius: 5px;"         // Yuvarlak köşeler
                                               "    font-size: 14px;"            // Yazı tipi boyutu
                                               "}";

    const QString disabledStlylesheet=          "QPushButton :: disabled{"
                                               "    background-color :#2d2d2d;"
                                               "    font-size: 14px;"
                                               "}";
    const QString sendStylesheet=               "QPushButton {"
                                                "    background-color: #474747;"
                                                "    color: white;"
                                                "    font-size: 14px;"
                                                "}"
                                                "QPushButton::pressed{"
                                                "    background-color: #0baa10;"
                                                "    color: white;"
                                                "    font-size:14px;"
                                                "}" ;
    const QString ipStylesheet=                "QLineEdit {"
                                               "     background-color: #474747; "
                                               "     color : white;"
                                               "     font-size:14px;"
                                               "}";
    const QString ipdisabledStylesheet=        "QLineEdit::disabled {"
                                               "     background-color: #2d2d2d; "
                                               "     font-size:14px;"
                                               "}";
    const QString portStylesheet=                "QSpinBox {"
                                               "     background-color: #474747; "
                                               "     color : white;"
                                               "     font-size:14px;"
                                               "}";
    const QString portdisabledStylesheet=        "QSpinBox::disabled {"
                                               "     background-color: #2d2d2d; "
                                               "     font-size:14px;"
                                               "}";
};
#endif // MAINWINDOW_H
