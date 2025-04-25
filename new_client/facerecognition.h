#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <QThread>
#include <QObject>
using namespace  std;
using namespace cv;
using namespace cv::dnn;

namespace Ui {
class FaceRecognition;
}

class FaceRecognition : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRecognition(QWidget *parent = nullptr);
    ~FaceRecognition();
    void paintEvent(QPaintEvent *event);
    cv::Mat QImageToCvMat(const QImage &image);
    QImage cvMatToQImage(const cv::Mat &mat);

private slots:
    void takePhoto();
    void onImageCaptured(int id, const QImage &preview);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FaceRecognition *ui;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
};

#endif // FACERECOGNITION_H
