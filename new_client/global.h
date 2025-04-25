#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QVariantList>
#include <QStringList>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QImage>

extern QString sqlUser;
extern QString sqlPwd;
extern QString sqlLocalHost;

extern QString currentEmpid;
extern QString currentName;
extern QString currentGender;
extern QString currentDepartment;
extern QString currentHireDate;
extern QString currentPhotoUrl;
extern QImage currentImage;
extern bool currentStatus;

extern void connect_to_database(QSqlDatabase db);

#endif // GLOBAL_H
