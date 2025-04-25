#include "network.h"
#include "ui_network.h"
#include <QPainter>
#include <global.h>
#include "menu.h"
#include <QDebug>

Network::Network(QWidget *parent) : QWidget(parent), ui(new Ui::Network)
{
    ui->setupUi(this);

    imageSocket = new QTcpSocket(this);
    infoSocket = new QTcpSocket(this);

    connect(imageSocket, &QTcpSocket::connected, this, &Network::onConnected);
    connect(imageSocket, &QTcpSocket::disconnected, this, &Network::onDisconnected);
    connect(imageSocket, &QTcpSocket::readyRead, this, &Network::onReadyRead);
    connect(infoSocket, &QTcpSocket::readyRead, this, &Network::onInfoReadyRead);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &Network::on_listWidget_itemClicked);

    imageSocket->connectToHost("119.29.26.131", 8881);
    infoSocket->connectToHost("119.29.26.131", 8883);

    ui->label->setMinimumSize(320, 240);
    ui->label->setScaledContents(true);
    ui->label->setAlignment(Qt::AlignCenter);
    // 初始化 YOLO 推理器
    pInf_ = new Inference("D:/YOLO/yolov5s.onnx", cv::Size(640, 640), "./classes.txt", true); // 降低分辨率
}

Network::~Network()
{
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
    delete ui;
}

void Network::onConnected()
{
    qDebug() << "Connected to server";
}

void Network::onDisconnected()
{
    qDebug() << "Disconnected from server";
}

void Network::onReadyRead()
{
    imageBuffer.append(imageSocket->readAll());

    while (imageBuffer.size() >= sizeof(quint32))
    {
        QDataStream stream(imageBuffer);
        stream.setByteOrder(QDataStream::BigEndian);

        quint32 imageSize;
        stream >> imageSize;

        if (imageBuffer.size() < sizeof(quint32) + imageSize)
        {
            return; // 等待更多数据
        }

        QByteArray imageData = imageBuffer.mid(sizeof(quint32), imageSize);
        imageBuffer.remove(0, sizeof(quint32) + imageSize);

        QPixmap pixmap;
        if (pixmap.loadFromData(imageData))
        {
            // 将 QPixmap 转换为 QImage 进行检测
            QImage image = pixmap.toImage();
            // 进行目标检测
            QImage detectedImage = detect(image);
            // 将检测后的图像转换回 QPixmap
            QPixmap detectedPixmap = QPixmap::fromImage(detectedImage);

            if (!currentClientIP.isEmpty())
            {
                clientImages[currentClientIP] = detectedPixmap;

                // 如果当前有选中的项目，则更新显示
                QListWidgetItem *currentItem = ui->listWidget->currentItem();
                if (currentItem && currentItem->text() == currentClientIP)
                {
                    ui->label->setPixmap(detectedPixmap.scaled(
                        ui->label->size(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
                    ui->label->repaint();
                }
            }
        }
    }
}

void Network::processClientInfo(const QString &info)
{
    QStringList parts = info.split(',');
    if (parts.size() >= 2)
    {
        QString newClientIP = parts[0];
        QString newEmpId = parts[1];

        currentClientIP = newClientIP; // 更新当前客户端IP
        currentEmpId = newEmpId;
        clientEmpIds[currentClientIP] = currentEmpId;

        // 如果列表中不存在该IP，则添加
        if (ui->listWidget->findItems(currentClientIP, Qt::MatchExactly).isEmpty())
        {
            QListWidgetItem *item = new QListWidgetItem(currentClientIP);
            ui->listWidget->addItem(item);
            // 自动选中新添加的项
            ui->listWidget->setCurrentItem(item);
        }
    }
}
void Network::processImageData()
{
    QPixmap pixmap;
    // 注意这里使用 imageData 而不是 imageBuffer
    if (pixmap.loadFromData(imageBuffer))
    {
        if (!currentClientIP.isEmpty())
        { // 确保有当前客户端IP
            clientImages[currentClientIP] = pixmap;

            // 如果当前选中的就是这个客户端，则更新显示
            if (ui->listWidget->currentItem() &&
                ui->listWidget->currentItem()->text() == currentClientIP)
            {
                ui->label->setPixmap(pixmap.scaled(
                    ui->label->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
                ui->label->repaint();
            }
        }
    }

    // 清空缓冲区
    imageBuffer.clear();
}

void Network::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString selectedIP = item->text();

    if (clientImages.contains(selectedIP))
    {
        ui->label->setPixmap(clientImages[selectedIP].scaled(
            ui->label->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    }

    if (clientEmpIds.contains(selectedIP))
    {
        QString empId = clientEmpIds[selectedIP];
        QString name;
        for (auto emp : emps)
        {
            if (empId == emp.empid)
            {
                name = emp.name;
                break;
            }
        }

        ui->label_2->setText("id: " + empId + " 当前用户: " + name);
    }
}

void Network::on_pushButton_clicked()
{
    // 先创建新窗口
    Menu *m = new Menu();

    // 显示新窗口
    m->show();

    // 释放 socket 资源
    if (imageSocket)
    {
        imageSocket->disconnectFromHost();
        imageSocket->deleteLater();
        imageSocket = nullptr;
    }

    if (infoSocket)
    {
        infoSocket->disconnectFromHost();
        infoSocket->deleteLater();
        infoSocket = nullptr;
    }

    // 清理数据缓存
    imageBuffer.clear();
    clientImages.clear();
    clientEmpIds.clear();

    // 关闭当前窗口
    this->close();
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

void Network::onInfoReadyRead()
{
    QByteArray data = infoSocket->readAll();
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    QString clientIP, empId;
    stream >> clientIP >> empId;

    processClientInfo(clientIP + "," + empId);
}

// QImage 转换为 OpenCV Mat
cv::Mat Network::QImageToCvMat(const QImage &image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void *)image.bits(), image.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    default:
        QImage converted = image.convertToFormat(QImage::Format_RGB888);
        mat = cv::Mat(converted.height(), converted.width(), CV_8UC3, (void *)converted.bits(), converted.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    }
    return mat;
}

// OpenCV Mat 转换为 QImage
QImage Network::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.empty())
        return QImage();

    cv::Mat temp;
    switch (mat.type())
    {
    case CV_8UC3:
        cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
        return QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888).copy();
    default:
        qWarning("Unsupported cv::Mat format for conversion to QImage.");
        return QImage();
    }
}

QImage Network::detect(const QImage &image)
{
    // QImage scaledImage = image.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 降低分辨率
    cv::Mat frame = QImageToCvMat(image);

    // 运行 YOLO 目标检测
    std::vector<Detection> output = pInf_->runInference(frame);
    std::cout << "Number of detections: " << output.size() << std::endl;

    for (const Detection &detection : output)
    {
        if (detection.confidence < 0.5)
            continue; // 过滤低置信度检测框

        cv::Rect box = detection.box;
        box &= cv::Rect(0, 0, frame.cols, frame.rows); // 确保框在图像范围内

        // 输出检测框的坐标信息到控制台
        /*
        std::cout << "Detection Box: "
                  << "x=" << box.x << ", y=" << box.y
                  << ", width=" << box.width << ", height=" << box.height
                  << ", class=" << detection.className
                  << ", confidence=" << detection.confidence << std::endl;
        */
        // 绘制检测框
        cv::Scalar color = detection.color;
        std::swap(color[0], color[2]); // BGR 转 RGB
        cv::rectangle(frame, box, color, 2);

        // 绘制检测标签
        std::string classString = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);
        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, nullptr);

        int text_x = box.x;
        int text_y = std::max(box.y - 10, 10); // 防止文本超出顶部
        cv::Rect textBox(text_x, text_y - textSize.height - 5, textSize.width + 10, textSize.height + 10);

        cv::rectangle(frame, textBox, color, cv::FILLED);
        cv::putText(frame, classString, cv::Point(text_x + 5, text_y),
                    cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, cv::LINE_AA);
    }

    // 仅用于预览，缩放图像
    float scale = std::min(1.0, 800.0 / frame.cols);
    cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_LINEAR);

    // 显示检测后的图像
    return cvMatToQImage(frame);
}
