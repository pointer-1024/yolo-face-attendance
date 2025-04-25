#include "server.h"
#include <QDebug>
#include <QDataStream>

Server::Server(QObject *parent) : QObject(parent)
{
    imageClientServer = new QTcpServer(this);
    imageAdminServer = new QTcpServer(this);
    infoClientServer = new QTcpServer(this);
    infoAdminServer = new QTcpServer(this);

    connect(imageClientServer, &QTcpServer::newConnection, this, &Server::onNewImageClientConnection);
    connect(imageAdminServer, &QTcpServer::newConnection, this, &Server::onNewImageAdminConnection);
    connect(infoClientServer, &QTcpServer::newConnection, this, &Server::onNewInfoClientConnection);
    connect(infoAdminServer, &QTcpServer::newConnection, this, &Server::onNewInfoAdminConnection);

    if(!imageClientServer->listen(QHostAddress::Any, 8880)) {
        qDebug() << "图像客户端服务器启动失败:" << imageClientServer->errorString();
    }
    if(!imageAdminServer->listen(QHostAddress::Any, 8881)) {
        qDebug() << "图像管理端服务器启动失败:" << imageAdminServer->errorString();
    }
    if(!infoClientServer->listen(QHostAddress::Any, 8882)) {
        qDebug() << "信息客户端服务器启动失败:" << infoClientServer->errorString();
    }
    if(!infoAdminServer->listen(QHostAddress::Any, 8883)) {
        qDebug() << "信息管理端服务器启动失败:" << infoAdminServer->errorString();
    }
}

Server::~Server()
{
    // 清理所有连接
    for(auto socket : clientIPs.keys()) {
        cleanupSocket(socket);
    }
    for(auto socket : adminImageSockets) {
        cleanupSocket(socket);
    }
    for(auto socket : adminInfoSockets) {
        cleanupSocket(socket);
    }
}

void Server::onNewImageClientConnection()
{
    QTcpSocket *socket = imageClientServer->nextPendingConnection();
    if(!socket) return;

    QString clientIP = socket->peerAddress().toString();
    clientIPs[socket] = clientIP;
    clientBuffers[socket] = QByteArray();

    connect(socket, &QTcpSocket::readyRead, this, &Server::onClientImageReceived);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "新图像客户端连接:" << clientIP;
}

void Server::onNewImageAdminConnection()
{
    QTcpSocket *socket = imageAdminServer->nextPendingConnection();
    if(!socket) return;

    adminImageSockets.append(socket);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onAdminDisconnected);

    qDebug() << "新图像管理端连接:" << socket->peerAddress().toString();
}

void Server::onNewInfoClientConnection()
{
    QTcpSocket *socket = infoClientServer->nextPendingConnection();
    if(!socket) return;

    QString clientIP = socket->peerAddress().toString();
    clientIPs[socket] = clientIP;

    connect(socket, &QTcpSocket::readyRead, this, &Server::onClientInfoReceived);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "新信息客户端连接:" << clientIP;
}

void Server::onNewInfoAdminConnection()
{
    QTcpSocket *socket = infoAdminServer->nextPendingConnection();
    if(!socket) return;

    adminInfoSockets.append(socket);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onAdminDisconnected);

    qDebug() << "新信息管理端连接:" << socket->peerAddress().toString();
}

void Server::onClientImageReceived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(!socket || !clientIPs.contains(socket)) return;

    QByteArray &buffer = clientBuffers[socket];
    buffer.append(socket->readAll());

    while(buffer.size() >= sizeof(quint32)) {
        QDataStream stream(buffer);
        stream.setByteOrder(QDataStream::BigEndian);
        
        quint32 imageSize;
        stream >> imageSize;

        if(buffer.size() < sizeof(quint32) + imageSize) {
            return;
        }

        QByteArray imageData = buffer.mid(sizeof(quint32), imageSize);
        buffer.remove(0, sizeof(quint32) + imageSize);

        QString clientIP = clientIPs[socket];
        forwardImageToAdmin(imageData, clientIP);
    }
}

void Server::onClientInfoReceived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(!socket || !clientIPs.contains(socket)) return;

    QByteArray data = socket->readAll();
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);
    
    QString empId;
    stream >> empId;

    QString clientIP = clientIPs[socket];
    clientInfo[clientIP] = empId;
    
    forwardInfoToAdmin(clientIP, empId);
}

void Server::forwardImageToAdmin(const QByteArray &imageData, const QString &clientIP)
{
    QByteArray package;
    QDataStream stream(&package, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint32>(imageData.size());
    package.append(imageData);

    for(QTcpSocket *socket : adminImageSockets) {
        if(socket && socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(package);
            socket->flush();
        }
    }
}

void Server::forwardInfoToAdmin(const QString &clientIP, const QString &empId)
{
    QByteArray package;
    QDataStream stream(&package, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << clientIP << empId;

    for(QTcpSocket *socket : adminInfoSockets) {
        if(socket && socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(package);
            socket->flush();
        }
    }
}

void Server::onClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(!socket) return;

    QString clientIP = clientIPs.value(socket);
    qDebug() << "客户端断开连接:" << clientIP;

    clientIPs.remove(socket);
    clientBuffers.remove(socket);
    clientInfo.remove(clientIP);
    cleanupSocket(socket);
}

void Server::onAdminDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(!socket) return;

    adminImageSockets.removeOne(socket);
    adminInfoSockets.removeOne(socket);
    cleanupSocket(socket);

    qDebug() << "管理端断开连接:" << socket->peerAddress().toString();
}

void Server::cleanupSocket(QTcpSocket *socket)
{
    if(!socket) return;
    
    socket->disconnect();
    socket->deleteLater();
}