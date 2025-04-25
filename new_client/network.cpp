#include "network.h"
#include "ui_network.h"
#include "menu.h"
#include "global.h"
#include <QHBoxLayout>
#include <QPainter>

Network::Network(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::Network)
{
    ui->setupUi(this);

    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this);
    imageCapture = new QCameraImageCapture(camera, this);
    // 创建布局管理器
    QHBoxLayout *layout = new QHBoxLayout(ui->label);
    layout->setContentsMargins(0, 0, 0, 0); // 设置边距为0
    layout->setSpacing(0);                  // 设置间距为0
    layout->addWidget(viewfinder);          // 将viewfinder添加到布局中

    // 确保viewfinder能够自适应label的大小
    viewfinder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    camera->setViewfinder(viewfinder);

    timer = new QTimer(this);

    // camera->start();
    connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &Network::onImageCaptured);
    connect(timer, &QTimer::timeout, [=]()
            { imageCapture->capture(); });

    // 初始化两个socket连接
    imageSocket = new QTcpSocket(this);
    infoSocket = new QTcpSocket(this);

    serverIP = "119.29.26.131";

    // 连接到不同端口
    imageSocket->connectToHost(serverIP, 8880);
    infoSocket->connectToHost(serverIP, 8882);

    // 连接信号
    connect(imageSocket, &QTcpSocket::connected, [this]()
            { qDebug() << "Image socket connected!"; });
    connect(infoSocket, &QTcpSocket::connected, [this]()
            { qDebug() << "Info socket connected!"; });

    // 错误处理
    connect(imageSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [this](QAbstractSocket::SocketError error)
            { qDebug() << "Image socket error:" << error; });
    connect(infoSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [this](QAbstractSocket::SocketError error)
            { qDebug() << "Info socket error:" << error; });
}

Network::~Network()
{

    delete ui;
    if (camera)
    {
        camera->stop();
        delete imageCapture; // 先删除依赖于camera的对象
        delete viewfinder;   // 删除取景器
        delete camera;       // 最后删除相机对象
    }
    delete ui;
}

void Network::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // 加载图片
    QPixmap pixmap(":/image/banner1.jpg");

    // 4. 设置图片透明度
    painter.setOpacity(0.3); // 设置透明度为0.7（范围0.0-1.0）

    // 绘制图片（适应窗口大小）
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);

    // 恢复透明度设置，避免影响其他绘制
    // painter.setOpacity(1.0);
}

// 修改 onImageCaptured 方法
void Network::onImageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);

    // 发送员工信息
    if(infoSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray infoData;
        QDataStream infoStream(&infoData, QIODevice::WriteOnly);
        infoStream.setByteOrder(QDataStream::BigEndian);
        infoStream << currentEmpid;
        infoSocket->write(infoData);
    }

    // 发送图像数据
    if(imageSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        preview.save(&buffer, "JPG", 50);
        buffer.close();

        // 发送图像数据
        QByteArray header;
        QDataStream headerStream(&header, QIODevice::WriteOnly);
        headerStream.setByteOrder(QDataStream::BigEndian);
        headerStream << static_cast<quint32>(imageData.size());

        imageSocket->write(header);
        imageSocket->write(imageData);
    }
}
void Network::on_pushButton_2_clicked()
{
    if (camera)
    {
        camera->stop();
        delete imageCapture;
        delete viewfinder;
        delete camera;
        camera = nullptr;
        imageCapture = nullptr;
        viewfinder = nullptr;
    }

    if (timer)
    {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (imageSocket)
    {
        imageSocket->disconnectFromHost();
        delete imageSocket;
    }

    if (infoSocket)
    {
        infoSocket->disconnectFromHost();
        delete infoSocket;
    }

    this->hide();
    menu *m = new menu();
    m->show();
}

void Network::on_pushButton_clicked()
{
    camera->start();
    timer->start(125);
}
