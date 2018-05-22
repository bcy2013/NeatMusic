#include "singleinstance.h"

SingleInstance::SingleInstance(QObject *parent) : QObject(parent)
{
    connect(&m_sever,&QLocalServer::newConnection,[this](){
       emit newInstance();
    });
}

void SingleInstance::listen(const QString &name)
{
    m_sever.removeServer(name);
    m_sever.listen(name);
}

bool SingleInstance::hasPrevious(QString name)
{
    QLocalSocket m_socket;
    m_socket.connectToServer(name,QLocalSocket::ReadOnly);
    return m_socket.waitForConnected();
}
