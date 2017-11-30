#include <QCoreApplication>
#include <QThread>

#include "client.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread *serverThread = new QThread;
    QThread *clientThread = new QThread;

    TcpServer server;
    TcpClient client;

    server.moveToThread(serverThread);
    client.moveToThread(clientThread);

    serverThread->start();
    clientThread->start();

    QObject::connect(serverThread, &QThread::started, &server, &TcpServer::listening);
    QObject::connect(clientThread, &QThread::started, &client, &TcpClient::connecting);

    return a.exec();
}
