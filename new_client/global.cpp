#include "global.h"

QString sqlUser = "root";
QString sqlPwd = "123456";
QString sqlLocalHost = "119.29.26.131";

QString currentEmpid = "";
QString currentName = "";
QString currentGender = "";
QString currentDepartment = "";
QString currentHireDate = "";
QString currentPhotoUrl = "";
QImage currentImage;
bool currentStatus = false;


void connect_to_database(QSqlDatabase db)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(sqlLocalHost);
    db.setUserName(sqlUser);
    db.setPassword(sqlPwd);
    db.setDatabaseName("company");

    if(!db.open())
        qDebug() << "connect fail";
    else
    {
        qDebug() << "success";
    }
}
