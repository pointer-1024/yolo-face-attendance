#include "menu.h"
#include "ui_menu.h"
#include "presons.h"
#include <QPainter>
#include "mainwindow.h"
#include "network.h"
#include "record.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::paintEvent(QPaintEvent *event)
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

void Menu::on_pushButton_4_clicked()
{
    this->hide();
    MainWindow *m = new MainWindow();
    m->show();
}

void Menu::on_pushButton_clicked()
{
    this->hide();
    Presons *p = new Presons();
    p->show();
}

void Menu::on_pushButton_3_clicked()
{
    this->hide();
    Record *r = new Record();
    r->show();
}

void Menu::on_pushButton_2_clicked()
{
    this->hide();
    Network *n = new Network();
    n->show();
}
