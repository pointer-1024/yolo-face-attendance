#ifndef PERSON_H
#define PERSON_H

#include <QWidget>

namespace Ui {
class person;
}

class person : public QWidget
{
    Q_OBJECT

public:
    explicit person(QWidget *parent = nullptr);
    ~person();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::person *ui;
};

#endif // PERSON_H
