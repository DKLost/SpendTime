#include "timeplansql.h"
#include "toolfunctions.h"

TimePlanSql::TimePlanSql(QString fileName,QObject *parent)
    : QObject{parent}
{
    //init db
    dbFileName = fileName;
    db = QSqlDatabase::addDatabase("QSQLITE","connection04");
    db.setDatabaseName(dbFileName);
    db.open();

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS timePlans(week INTEGER,开始时间 TEXT,结束时间 TEXT,时间段 TEXT,行动 TEXT)");
}

void TimePlanSql::addPlan(int week,QTime startTime,QTime endTime,QString action)
{
    QString duration = ToolFunctions::timeDurationText(startTime,endTime);
    QSqlQuery query(db);
    query.prepare("INSERT INTO timePlans VALUES(?,?,?,?,?)");
    query.bindValue(0,week);
    query.bindValue(1,startTime.toString("HH:mm"));
    query.bindValue(2,endTime.toString("HH:mm"));
    query.bindValue(3,duration);
    query.bindValue(4,action);
    query.exec();
}

void TimePlanSql::delPlan(int week, QTime startTime)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM timePlans WHERE week = ? AND 开始时间 = ?");
    query.bindValue(0,week);
    query.bindValue(1,startTime.toString("HH:mm"));
    query.exec();
}

QSqlDatabase TimePlanSql::getDb() const
{
    return db;
}
