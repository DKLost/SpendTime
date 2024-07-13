#ifndef TIMEPLANSQL_H
#define TIMEPLANSQL_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QMessageBox>

class TimePlanSql : public QObject
{
    Q_OBJECT
public:
    explicit TimePlanSql(QString fileName,QObject *parent = nullptr);
    void addPlan(int week,QTime startTime,QTime endTime,QString action);
    void delPlan(int week,QTime startTime);

    QSqlDatabase getDb() const;

private:
    QSqlDatabase db;
    QString dbFileName;


signals:
};

#endif // TIMEPLANSQL_H
