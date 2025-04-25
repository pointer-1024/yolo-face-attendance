#ifndef RECORD_H
#define RECORD_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class Record;
}

class Record : public QWidget
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = nullptr);
    ~Record();
    void paintEvent(QPaintEvent *event);
    void loadComboBoxData(QComboBox *comboBox, const QString &columnName);
    void filterTable();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::Record *ui;
};

#endif // RECORD_H
