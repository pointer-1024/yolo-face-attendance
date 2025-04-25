#include "record.h"
#include "ui_record.h"
#include "global.h"
#include "menu.h"
#include "QStringList"
#include <QDebug>
#include <QPainter>
#include <QDateTime>
#include <QDate>

Record::Record(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Record)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query(db);
    query.prepare("SELECT * FROM attendance WHERE emp_id = :emp_id");
    query.bindValue(":emp_id", currentEmpid);
    query.exec();

    // 设置表头
    QStringList headers;
    headers << "id" << "date" << "startTime" << "status";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 填充数据
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // 获取记录数据
        QDate date = query.value("date").toDate();
        QDateTime startTime = query.value("start_time").toDateTime();
        QString status = query.value("status").toString();

        // 添加到表格
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(currentEmpid));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(date.toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(startTime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(status));
    }

    // 调整表格显示
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置combox
    loadComboBoxData(ui->comboBox,"date");
    loadComboBoxData(ui->comboBox_2,"status");
}

Record::~Record()
{
    delete ui;
}

void Record::on_pushButton_clicked()
{
    this->hide();
    menu *m = new menu();
    m->show();
}

void Record::paintEvent(QPaintEvent *event)
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

void Record::loadComboBoxData(QComboBox *comboBox, const QString &columnName) {
    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query;
    query.exec(QString("SELECT DISTINCT %1 FROM attendance").arg(columnName));

    comboBox->addItem("All");
    while (query.next()) {
        comboBox->addItem(query.value(0).toString());
    }
}

void Record::filterTable()
{
    // 获取过滤条件
    QString dateFilter = ui->comboBox->currentText();                 // 从日期选择框获取日期
    QString statusFilter = ui->comboBox_2->currentText();  // 从下拉框获取状态

    // 清空表格
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // 构建动态SQL查询
    QString sql = "SELECT * FROM attendance WHERE emp_id = :emp_id";
    QVariantList params;
    params << currentEmpid;

    // 添加状态过滤条件（如果未选择"全部"）
    if (statusFilter != "All" && !statusFilter.isEmpty()) {
        sql += " AND status = :status";
        params << statusFilter;
    }

    // 添加状态过滤条件（如果未选择"全部"）
    if (dateFilter != "All" && !dateFilter.isEmpty()) {
        sql += " AND date = :date";
        params << dateFilter;
    }

    // 执行查询
    QSqlQuery query;
    query.prepare(sql);

    // 绑定参数
    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    query.exec();

    // 填充数据
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // 获取记录数据
        QDate date = query.value("date").toDate();
        QDateTime startTime = query.value("start_time").toDateTime();
        QString status = query.value("status").toString();

        // 添加到表格
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(currentEmpid));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(date.toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(startTime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(status));
    }

    // 调整表格显示
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Record::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    filterTable();
}

void Record::on_comboBox_2_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    filterTable();
}
