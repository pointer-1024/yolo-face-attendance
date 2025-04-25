#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QTimer>
#include <QTcpSocket>
#include <QBuffer>

namespace Ui {
class Network;
}

class Network : public QWidget
{
    Q_OBJECT

public:
    explicit Network(QWidget *parent = nullptr);
    ~Network();
    void paintEvent(QPaintEvent *event);

private slots:
    void onImageCaptured(int id, const QImage &preview);


    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Network *ui;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QTimer *timer;
    QTcpSocket *imageSocket;  // 图像数据socket
    QTcpSocket *infoSocket;   // 信息数据socket
    QString serverIP;
};

#endif // NETWORK_H
