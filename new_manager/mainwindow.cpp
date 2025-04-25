#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "menu.h"
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
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

QPixmap MainWindow::loadNetworkImage(const QString &url) {
    QNetworkAccessManager manager;
    QEventLoop loop;  // 用于同步等待下载完成

    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();  // 阻塞直到下载完成

    QPixmap pixmap;
    if (reply->error() == QNetworkReply::NoError) {
        pixmap.loadFromData(reply->readAll());
    }
    reply->deleteLater();
    return pixmap;
}


void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    //qDebug() << username;
    //qDebug() << password;

    QSqlDatabase db;
    connect_to_database(db);
    QSqlQuery query(db);

    // 第一步：查询 admin 表是否存在且密码匹配
    query.prepare("SELECT * FROM admins WHERE username = :username AND password_hash = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        // 第二步：如果匹配成功，从 employees 表获取详细信息
        query.prepare("SELECT * FROM employees");
        if(query.exec()){
            while (query.next()) {
                Emp e;
                e.empid = query.value("emp_id").toString();
                e.name = query.value("name").toString();
                e.gender = query.value("gender").toString();
                e.department = query.value("department").toString();
                e.hireDate = query.value("hire_date").toDate();
                e.photoUrl = query.value("photo_url").toString();
                QPixmap pix = loadNetworkImage(e.photoUrl);
                e.image = pix.toImage();
                //qDebug() << e.empid;
                emps.append(e);
            }
            //打开菜单栏
            this->hide();
            Menu *m = new Menu();
            m->show();
        }

    } else {
        QMessageBox::critical(this,"提示","密码错误");
    }
}
