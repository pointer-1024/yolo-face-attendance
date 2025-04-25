#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QList>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

private slots:
    void onNewImageClientConnection();    // 8880端口
    void onNewImageAdminConnection();     // 8881端口
    void onNewInfoClientConnection();     // 8882端口
    void onNewInfoAdminConnection();      // 8883端口
    
    void onClientImageReceived();
    void onClientInfoReceived();
    void onClientDisconnected();
    void onAdminDisconnected();

private:
    void forwardImageToAdmin(const QByteArray &imageData, const QString &clientIP);
    void forwardInfoToAdmin(const QString &clientIP, const QString &empId);
    void cleanupSocket(QTcpSocket *socket);

    QTcpServer *imageClientServer;    // 8880端口
    QTcpServer *imageAdminServer;     // 8881端口
    QTcpServer *infoClientServer;     // 8882端口
    QTcpServer *infoAdminServer;      // 8883端口
    
    QList<QTcpSocket *> adminImageSockets;
    QList<QTcpSocket *> adminInfoSockets;
    QMap<QTcpSocket *, QString> clientIPs;
    QMap<QTcpSocket *, QByteArray> clientBuffers;
    QMap<QString, QString> clientInfo;  // clientIP -> empId
};

#endif // SERVER_H