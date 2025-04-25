#ifndef PRESONS_H
#define PRESONS_H

#include <QWidget>
#include <QComboBox>
#include <QMap>
#include <QImage>

namespace Ui {
class Presons;
}

class Presons : public QWidget
{
    Q_OBJECT

public:
    explicit Presons(QWidget *parent = nullptr);
    ~Presons();
    void paintEvent(QPaintEvent *event);
    void loadComboBoxData(QComboBox *comboBox, const QString &columnName);
    void filterTable();

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

private:
    Ui::Presons *ui;
    QMap<QString,QImage> images;
};

#endif // PRESONS_H
