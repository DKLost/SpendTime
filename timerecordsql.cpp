#include "timerecordsql.h"

TimeRecordSql::TimeRecordSql(QString fileName,QObject *parent)
    : QObject{parent}
{
    //init db
    dbFileName = fileName;
    db = QSqlDatabase::addDatabase("QSQLITE","connection03");
    db.setDatabaseName(dbFileName);
    db.open();

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS timeRecords(日期 TEXT,时间段 TEXT,行动记录 TEXT)");
}

void TimeRecordSql::add_record(QTime currentTime,int timeCounter,QString skillName)
{
    if(timeCounter < 60)
        return;
    QTime startTime = currentTime.addSecs(-timeCounter);
    QDate currentDate = QDate::currentDate();
    QString mString = QString("%1m").arg(timeCounter/60);
    QString duration = QString("%1-%2(%3)")
                           .arg(startTime.toString("hh:mm"))
                           .arg(currentTime.toString("hh:mm"))
                                    .arg(mString);
    QSqlQuery query(db);
    query.prepare("INSERT INTO timeRecords VALUES(?,?,?)");
    query.bindValue(0,currentDate.toString("yyyy/MM/dd"));
    query.bindValue(1,duration);
    query.bindValue(2,skillName);
    query.exec();
}

int TimeRecordSql::sum_minutes(QDate day, QString skillName)
{
    QSqlQuery query(db);
    query.prepare("SELECT 时间段 FROM timeRecords WHERE 日期 = ? AND 行动记录 = ?");
    query.bindValue(0,day.toString("yyyy/MM/dd"));
    query.bindValue(1,skillName);
    query.exec();
    int total_time = 0;
    while(query.next())
    {
        QString s = query.value(0).toString();
        auto list1 = s.split('(');
        auto list2 = list1[1].split('m');
        total_time += list2[0].toInt();
    }
    return total_time;
}

QSqlDatabase TimeRecordSql::getDb() const
{
    return db;
}
