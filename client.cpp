#include "client.h"

#include <QDataStream>

TcpClient::TcpClient(QObject *parent):
    QTcpSocket(parent)
{
    data_size = 0;

    connect(this, &TcpClient::connected, this, &TcpClient::onConnected);
    connect(this, &TcpClient::disconnected, this, &TcpClient::onDisconnected);
    connect(this, &TcpClient::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::connecting()
{
    connectToHost(QHostAddress::LocalHost, 12345);
    if ( !waitForConnected(2000) )
        qDebug() << "Cannot connect to host";
}

void TcpClient::onConnected()
{
    QString message("Hello!");
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream << static_cast<decltype(data_size)>(message.size());

    payload.append(message);
    write(payload);
    waitForBytesWritten();
}

void TcpClient::onDisconnected()
{
    qDebug() << "Disconected";
}

void TcpClient::onReadyRead()
{
    while(bytesAvailable() > data_size)
    {
        if (data_size == 0)
        {
            QByteArray payload = read(sizeof(data_size));
            QDataStream dataStream(payload);
            dataStream >> data_size;
        }
        while ( data_size > 0 )
        {
            QByteArray data = read(data_size);
            data_size = 0;
            qDebug() << data;
        }
    }
    disconnectFromHost();
}

void TcpClient::onError(QAbstractSocket::SocketError error)
{
    QString errorMessage;

    switch(error)
    {
    case QAbstractSocket::HostNotFoundError:
        errorMessage.append("The host was not found.");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMessage.append("The remote host is closed.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMessage.append("The connection was refused.");
        break;
    default:
        errorMessage.append("The unknown error.");
    }

    qDebug() << errorMessage;
}

void TcpClient::onStop()
{
    disconnectFromHost();
}
