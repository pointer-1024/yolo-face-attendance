#include "presons.h"
#include "ui_presons.h"
#include "global.h"
#include "menu.h"
#include <QPixmap>
#include <QPainter>

Presons::Presons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Presons)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query(db);
    query.exec("SELECT * FROM employees");

    //加载图像
    for(auto emp : emps){
        images[emp.empid] = emp.image;
    }

    // 设置表头
    QStringList headers;
    headers << "id" << "name" << "gender" << "department" << "hireDate";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 填充数据
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // 获取记录数据
        Emp e;
        e.empid = query.value("emp_id").toString();
        e.name = query.value("name").toString();
        e.gender = query.value("gender").toString();
        e.department = query.value("department").toString();
        e.hireDate = query.value("hire_date").toDate();

        // 添加到表格
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(e.empid));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(e.name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(e.gender));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(e.department));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(e.hireDate.toString("yyyy-MM-dd")));
    }

    // 调整表格显示
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //加载combox
    loadComboBoxData(ui->comboBox, "emp_id");
    loadComboBoxData(ui->comboBox_2, "gender");
    loadComboBoxData(ui->comboBox_3, "department");
    loadComboBoxData(ui->comboBox_4, "hire_date");
}

Presons::~Presons()
{
    delete ui;
}

void Presons::paintEvent(QPaintEvent *event)
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

void Presons::on_pushButton_clicked()
{
    this->hide();
    Menu *m = new Menu();
    m->show();
}

void Presons::loadComboBoxData(QComboBox *comboBox, const QString &columnName) {
    QSqlDatabase db = QSqlDatabase::database(); // 获取默认连接
    QSqlQuery query;
    query.exec(QString("SELECT DISTINCT %1 FROM employees").arg(columnName));

    comboBox->addItem("All");
    while (query.next()) {
        comboBox->addItem(query.value(0).toString());
    }
}

void Presons::on_tableWidget_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return; // 确保索引有效
        QAbstractItemModel *model = ui->tableWidget->model();
        int row = index.row(); // 获取当前行号

        ui->lineEdit->setText(model->index(row, 0).data().toString());
        ui->lineEdit_2->setText(model->index(row, 1).data().toString());
        ui->lineEdit_3->setText(model->index(row, 2).data().toString());
        ui->lineEdit_4->setText(model->index(row, 3).data().toString());
        ui->lineEdit_5->setText(model->index(row, 4).data().toString());
        QPixmap pix = QPixmap::fromImage(images[model->index(row, 0).data().toString()]);

        ui->label_5->setPixmap(
            pix.scaled(
                ui->label_5->size(),       // 目标大小（QLabel 的当前尺寸）
                Qt::KeepAspectRatio,       // 保持宽高比
                Qt::SmoothTransformation   // 平滑缩放（避免锯齿）
                )
        );
}

void Presons::filterTable()
{
    // 获取过滤条件
    QString empIdFilter = ui->comboBox->currentText();                 // 从日期选择框获取日期
    QString genderFilter = ui->comboBox_2->currentText();  // 从下拉框获取状态
    QString departmentFilter = ui->comboBox_3->currentText();
    QString hireDateFilter = ui->comboBox_4->currentText();

    // 清空表格
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // 构建动态SQL查询
    QString sql = "SELECT * FROM employees WHERE 1=1";  // 使用1=1方便拼接条件
    QVariantList params;

    // 添加员工ID过滤
   if (!empIdFilter.isEmpty() && empIdFilter != "All") {
       sql += " AND emp_id = :emp_id";
       params << empIdFilter;
   }

   // 添加性别过滤
   if (!genderFilter.isEmpty() && genderFilter != "All") {
       sql += " AND gender = :gender";
       params << genderFilter;
   }

   // 添加部门过滤
   if (!departmentFilter.isEmpty() && departmentFilter != "All") {
       sql += " AND department = :department";
       params << departmentFilter;
   }

   // 添加入职日期过滤
   if (!hireDateFilter.isEmpty() && hireDateFilter != "All") {
       sql += " AND hire_date = :hire_date";
       params << QDate::fromString(hireDateFilter, "yyyy-MM-dd");
   }

   // 执行查询
   QSqlQuery query;
   query.prepare(sql);

   // 绑定参数
   for (int i = 0; i < params.size(); ++i) {
       query.bindValue(i, params[i]);
   }

    query.exec();

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // 获取记录数据
        Emp e;
        e.empid = query.value("emp_id").toString();
        e.name = query.value("name").toString();
        e.gender = query.value("gender").toString();
        e.department = query.value("department").toString();
        e.hireDate = query.value("hire_date").toDate();

        // 添加到表格
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(e.empid));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(e.name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(e.gender));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(e.department));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(e.hireDate.toString("yyyy-MM-dd")));
    }

    // 调整表格显示
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Presons::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}

void Presons::on_comboBox_2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}

void Presons::on_comboBox_3_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}

void Presons::on_comboBox_4_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    filterTable();
}
