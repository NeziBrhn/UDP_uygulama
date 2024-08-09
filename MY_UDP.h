#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include<QUdpSocket>

class MY_UDP: public QObject
{
    Q_OBJECT
public:

    explicit MY_UDP(QObject *parent =0);

public slots:

private:

};

#endif // MYUDP_H
