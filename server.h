#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QElapsedTimer>

class TcpServer: public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = Q_NULLPTR);

    void incomingConnection(qintptr handle);

public slots:
    void listening();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    qint64 data_size;
};

#endif // SERVER_H
