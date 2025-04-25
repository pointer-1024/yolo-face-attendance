#include "record.h"
#include "ui_record.h"
#include "global.h"
#include "menu.h"
#include <QPainter>

Record::Record(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Record)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query(db);

    // 清空表格
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        // 设置表头
        QStringList headers;
        headers << "id" << "name" << "date" << "starTime" << "status";
        ui->tableWidget->setColumnCount(headers.size());
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        // 构建并执行查询
        query.prepare("SELECT a.emp_id AS id, e.name AS name, a.date AS date, "
                     "a.start_time AS starTime, a.status AS status "
                     "FROM attendance a JOIN employees e ON a.emp_id = e.emp_id");
        query.exec();
        // 填充数据
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // 按表头顺序获取数据
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("name").toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("date").toDate().toString("yyyy-MM-dd")));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("starTime").toDateTime().toString("hh:mm:ss")));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
        }

        // 调整表格显示
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->verticalHeader()->setVisible(false);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //加载combox
    loadComboBoxData(ui->comboBox, "emp_id");
    loadComboBoxData(ui->comboBox_2,"date");
    loadComboBoxData(ui->comboBox_3,"status");
}

Record::~Record()
{
    delete ui;
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

void Record::loadComboBoxData(QComboBox *comboBox, const QString &columnName)
{
    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query;
    if("emp_id" == columnName){
        query.exec(QString("SELECT DISTINCT %1 FROM employees").arg(columnName));
    } else {
        query.exec(QString("SELECT DISTINCT %1 FROM attendance").arg(columnName));
    }


    comboBox->addItem("All");
    while (query.next()) {
        comboBox->addItem(query.value(0).toString());
    }
}

void Record::on_pushButton_clicked()
{
    this->hide();
    Menu *m = new Menu();
    m->show();
}

void Record::filterTable()
{
    // 获取过滤条件
    QString empIdFilter = ui->comboBox->currentText();                 // 从日期选择框获取日期
    QString dateFilter = ui->comboBox_2->currentText();  // 从下拉框获取状态
    QString statusFilter = ui->comboBox_3->currentText();

    // 清空表格
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // 构建动态SQL查询
    QString sql = "SELECT a.emp_id AS id, e.name AS name, a.date AS date, "
                 "a.start_time AS starTime, a.status AS status "
                 "FROM attendance a JOIN employees e ON a.emp_id = e.emp_id WHERE 1=1";
    QVariantList params;

    // 添加员工ID过滤 - 修改为使用表别名
    if (!empIdFilter.isEmpty() && empIdFilter != "All") {
        sql += " AND a.emp_id = :emp_id";  // 添加 a.
        params << empIdFilter;
    }

    // 添加状态过滤
    if (statusFilter != "All" && !statusFilter.isEmpty()) {
        sql += " AND a.status = :status";  // 添加 a.
        params << statusFilter;
    }

    // 添加日期过滤
    if (dateFilter != "All" && !dateFilter.isEmpty()) {
        sql += " AND a.date = :date";  // 添加 a.
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

        // 按表头顺序获取所有列的数据
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("date").toDate().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("starTime").toDateTime().toString("hh:mm:ss")));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
    }

    // 调整表格显示
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void Record::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}

void Record::on_comboBox_2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}

void Record::on_comboBox_3_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}
