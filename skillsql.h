#ifndef SKILLSQL_H
#define SKILLSQL_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QMessageBox>

class SkillSql : public QObject
{
    Q_OBJECT
public:
    explicit SkillSql(QString fileName,QObject *parent = nullptr);
    void add_skill(QString skillName);
    void delete_skill(QString skillName);
    void increase_skill_time(QString skillName,int seconds);

    QSqlDatabase getDb() const;

private:
    QSqlDatabase db;
    QString dbFileName;

signals:

};

#endif // SKILLSQL_H
