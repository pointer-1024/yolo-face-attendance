#include "facerecognition.h"
#include "ui_facerecognition.h"
#include "global.h"
#include "menu.h"
#include <QPainter>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QMessageBox>

// 获取人脸特征（128D）
Mat getFaceEmbedding(const Mat& face, Net& recognizer) {
    Mat blob = blobFromImage(face, 1.0 / 255, Size(112, 112), Scalar(), true, false);
    recognizer.setInput(blob);
    return recognizer.forward().clone(); // 返回特征向量
}

// 计算欧氏距离
double calcDistance(const Mat& feat1, const Mat& feat2) {
    return norm(feat1 - feat2);
}

// 从图像中检测并提取第一张人脸
Mat detectAndCropFace(Mat img, Ptr<FaceDetectorYN> detector) {
    detector->setInputSize(img.size());
    Mat faces;
    detector->detect(img, faces);
    //if (faces.rows == 0) throw runtime_error("未检测到人脸！");
    float* data = (float*)faces.ptr(0);
    Rect box(cvRound(data[0]), cvRound(data[1]), cvRound(data[2]), cvRound(data[3]));
    return img(box).clone();
}

FaceRecognition::FaceRecognition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceRecognition)
{
    ui->setupUi(this);

    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(ui->label);
    viewfinder->setGeometry(0, 0, 400, 400);
    imageCapture = new QCameraImageCapture(camera, this);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    //viewfinder->setParent(ui->label);
    camera->setViewfinder(viewfinder);
    camera->start();

    connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &FaceRecognition::onImageCaptured);
}

FaceRecognition::~FaceRecognition()
{
    delete ui;
    if (camera) {
        camera->stop();
        delete imageCapture;  // 先删除依赖于camera的对象
        delete viewfinder;    // 删除取景器
        delete camera;        // 最后删除相机对象
    }
    delete ui;
}

void FaceRecognition::paintEvent(QPaintEvent *event)
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
    //painter.setOpacity(1.0);
}

// QImage 转换为 OpenCV Mat
cv::Mat FaceRecognition::QImageToCvMat(const QImage &image) {
    cv::Mat mat;
    switch (image.format()) {
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    default:
        QImage converted = image.convertToFormat(QImage::Format_RGB888);
        mat = cv::Mat(converted.height(), converted.width(), CV_8UC3, (void*)converted.bits(), converted.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    }
    return mat;
}

// OpenCV Mat 转换为 QImage
QImage FaceRecognition::cvMatToQImage(const cv::Mat &mat) {
    if (mat.empty()) return QImage();

    cv::Mat temp;
    switch (mat.type()) {
    case CV_8UC3:
        cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
        return QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888).copy();
    default:
        qWarning("Unsupported cv::Mat format for conversion to QImage.");
        return QImage();
    }
}

void FaceRecognition::takePhoto()
{
    //QString path = QDir::currentPath() + "/photo_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".png";
    imageCapture->capture();
}

void FaceRecognition::onImageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);
    
    // 设置 YuNet 使用 CUDA 后端
    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
        "C:/Users/zq/Downloads/face_detection_yunet_2023mar.onnx",
        "", Size(320, 320), 0.9, 0.3, 5000
    );

   // 加载识别模型（sface）
   Net recognizer = readNet("C:/Users/zq/Downloads/face_recognition_sface_2021dec.onnx");
   recognizer.setPreferableBackend(DNN_BACKEND_CUDA);
   recognizer.setPreferableTarget(DNN_TARGET_CUDA);
   // 加载两张图像
   Mat img1 = QImageToCvMat(currentImage);
   Mat img2 = QImageToCvMat(preview);

   // 检测 + 裁剪人脸
   Mat face1 = detectAndCropFace(img1, detector);
   Mat face2 = detectAndCropFace(img2, detector);

   //显示
   QPixmap pixmap = QPixmap::fromImage(cvMatToQImage(face1));
   ui->label_2->setPixmap(pixmap.scaled(ui->label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   pixmap = QPixmap::fromImage(cvMatToQImage(face2));
   ui->label_3->setPixmap(pixmap.scaled(ui->label_3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

   // 调整到模型输入大小（112x112）
   cv::resize(face1, face1, Size(112, 112));
   cv::resize(face2, face2, Size(112, 112));

   // 提取特征
   Mat feat1 = getFaceEmbedding(face1, recognizer);
   Mat feat2 = getFaceEmbedding(face2, recognizer);

   // 计算距离
   double dist = calcDistance(feat1, feat2);
   cout << "dist: " << dist << endl;

   if (dist < 1){
       QMessageBox::information(this,"提示","认证成功");
       currentStatus = true;
       camera->stop();

       QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
       QSqlQuery query(db);

       // 使用参数化查询（避免 SQL 注入）
       query.prepare("INSERT INTO attendance (emp_id, date, start_time, status) "
                     "VALUES (:emp_id, :date, :start_time, :status)");

       // 绑定参数
       query.bindValue(":emp_id", currentEmpid);
       query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd")); // 格式化为 SQL 日期
       query.bindValue(":start_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

       QTime current = QTime::currentTime();

       // 定义两个时间段
       QTime morningStart(7, 0);    // 07:00
       QTime morningEnd(9, 0);      // 09:00
       QTime noonStart(12, 0);      // 12:00
       QTime noonEnd(14, 0);

       if((current >= morningStart && current <= morningEnd) ||
                  (current >= noonStart && current <= noonEnd)){
           query.bindValue(":status", "正常");
       } else {
           query.bindValue(":status", "迟到");
       }

       query.exec();
   } else {
       QMessageBox::critical(this,"提示","认证失败");
       currentStatus = false;
   }
}



void FaceRecognition::on_pushButton_clicked()
{
    takePhoto();
}

void FaceRecognition::on_pushButton_2_clicked()
{
    if (camera) {
            camera->stop();
            delete imageCapture;
            delete viewfinder;
            delete camera;
            camera = nullptr;
            imageCapture = nullptr;
            viewfinder = nullptr;
        }

        this->hide();
        menu *m = new menu();
        m->show();
}
