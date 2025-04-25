#include "person.h"
#include "ui_person.h"
#include "global.h"
#include <QPixmap>
#include <QPainter>
#include "menu.h"

person::person(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::person)
{
    ui->setupUi(this);
    QPixmap pix = QPixmap::fromImage(currentImage);
    ui->label->setMinimumSize(pix.size());  // 或固定值如 QSize(400, 300)
    ui->label->setPixmap(pix.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lineEdit->setText(currentEmpid);
    ui->lineEdit_2->setText(currentName);
    ui->lineEdit_3->setText(currentGender);
    ui->lineEdit_4->setText(currentHireDate);
    ui->lineEdit_5->setText(currentDepartment);
}

person::~person()
{
    delete ui;
}

void person::paintEvent(QPaintEvent *event)
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

void person::on_pushButton_clicked()
{
    this->hide();
    menu *m = new menu();
    m->show();
}
