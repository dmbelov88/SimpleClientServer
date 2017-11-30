#include "server.h"

#include <QDataStream>
#include <QTcpSocket>

TcpServer::TcpServer(QObject *parent):
  QTcpServer(parent)
{}

void TcpServer::listening()
{
  if ( listen(QHostAddress::Any, 12345) )
    qDebug() << "Listening...";
  else
    qDebug() << "Error: " << errorString();
}

void TcpServer::incomingConnection(qintptr handle)
{
  qDebug() << "New connection";

  socket = new QTcpSocket(this);
  socket->setSocketDescriptor(handle);

  connect(socket, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
  connect(socket, &QTcpSocket::disconnected, this, &TcpServer::onDisconnected);
}

void TcpServer::onReadyRead()
{
    while(socket->bytesAvailable() > data_size)
    {
        if (data_size == 0)
        {
            QByteArray payload = socket->read(sizeof(data_size));
            QDataStream dataStream(payload);
            dataStream >> data_size;
        }
        while ( data_size > 0 )
        {
            QByteArray data = socket->read(data_size);
            qDebug() << data;

            QString message("Hello!");
            QByteArray payload;
            QDataStream stream(&payload, QIODevice::WriteOnly);
            stream << static_cast<decltype(data_size)>(message.size());
            payload.append(message);
            socket->write(payload);
            socket->waitForBytesWritten();

            data_size = 0;
        }
    }
}

void TcpServer::onDisconnected()
{
  qDebug() << "socket disconnected";
  socket->close();
  socket->deleteLater();
}
