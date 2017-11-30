#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include <QThread>

class TcpClient: public QTcpSocket
{
    Q_OBJECT
public:
    TcpClient(QObject *parent = Q_NULLPTR);

private:
    qint64 data_size;

public slots:
    void connecting();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);
    void onStop();
};

#endif // TCP_CLIENT_H
