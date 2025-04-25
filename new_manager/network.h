#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidget>
#include <QMap>
#include "inference.h"

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
    cv::Mat QImageToCvMat(const QImage &image);
    QImage cvMatToQImage(const cv::Mat &mat);
    QImage detect(const QImage& image);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onInfoReadyRead();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

private:
    Ui::Network *ui;
    QTcpSocket *imageSocket;
    QTcpSocket *infoSocket;
    QByteArray imageBuffer;
    QMap<QString, QPixmap> clientImages;
    QMap<QString, QString> clientEmpIds;
    QString currentClientIP;
    QString currentEmpId;
    void processClientInfo(const QString &info);
    void processImageData();
    Inference *pInf_;                 // YOLO 推理器
};

#endif // MANAGER_H
