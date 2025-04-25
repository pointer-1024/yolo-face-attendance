#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QVariantList>
#include <QStringList>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QImage>
#include <QList>
#include <QDate>

struct Emp{
  QString empid;
  QString name;
  QString gender;
  QString department;
  QDate hireDate;
  QString photoUrl;
  QImage image;
};

extern QString sqlUser;
extern QString sqlPwd;
extern QString sqlLocalHost;

extern QList<Emp> emps;

extern void connect_to_database(QSqlDatabase db);

#endif // GLOBAL_H
