#ifndef TIMERECORDSQL_H
#define TIMERECORDSQL_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QMessageBox>

class TimeRecordSql : public QObject
{
    Q_OBJECT
public:
    explicit TimeRecordSql(QString fileName,QObject *parent = nullptr);
    void add_record(QTime currentTime,int timeCounter,QString skillName);
    int sum_minutes(QDate day,QString skillName);
    QSqlDatabase getDb() const;

private:
    QSqlDatabase db;
    QString dbFileName;

signals:
};

#endif // TIMERECORDSQL_H
