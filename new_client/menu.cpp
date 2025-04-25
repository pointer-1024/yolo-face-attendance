#include "menu.h"
#include "ui_menu.h"
#include "mainwindow.h"
#include "person.h"
#include "facerecognition.h"
#include "global.h"
#include "record.h"
#include "network.h"
#include <QPainter>

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(!currentStatus);
    ui->pushButton_3->setEnabled(currentStatus);
    ui->pushButton_4->setEnabled(currentStatus);
}

menu::~menu()
{
    delete ui;
}

void menu::paintEvent(QPaintEvent *event)
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

void menu::on_pushButton_5_clicked()
{
    this->hide();
    MainWindow *mw = new MainWindow;
    mw->show();
}

void menu::on_pushButton_clicked()
{
    this->hide();
    person *p = new person();
    p->show();
}

void menu::on_pushButton_2_clicked()
{
    this->hide();
    FaceRecognition *f = new FaceRecognition();
    f->show();
}

void menu::on_pushButton_4_clicked()
{
    this->hide();
    Record *r = new Record();
    r->show();
}

void menu::on_pushButton_3_clicked()
{
    this->hide();
    Network *n = new Network();
    n->show();
}
