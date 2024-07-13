#include "skillsql.h"
#include "toolfunctions.h"

SkillSql::SkillSql(QString fileName, QObject *parent)
    : QObject{parent}
{
    //init db
    dbFileName = fileName;
    db = QSqlDatabase::addDatabase("QSQLITE","connection01");
    db.setDatabaseName(dbFileName);
    db.open();

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS skills(行动 TEXT PRIMARY KEY,总时长 TEXT,当日时长 TEXT)");
}

void SkillSql::add_skill(QString skillName)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO skills VALUES(?,'0h:0m:0s','0h:0m')");
    query.bindValue(0,skillName);
    query.exec();
}

void SkillSql::delete_skill(QString skillName)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM skills WHERE 行动 = ?");
    query.bindValue(0,skillName);
    query.exec();
}

void SkillSql::increase_skill_time(QString skillName, int seconds)
{
    int t;
    QSqlQuery query(db);
    query.prepare("SELECT 总时长 FROM skills WHERE 行动 = ?");
    query.bindValue(0,skillName);
    query.exec();
    while(query.next())
    {
        t = ToolFunctions::string2sec(query.value(0).toString());
        t += seconds;
        query.prepare("UPDATE skills SET 总时长 = ? WHERE 行动 = ?");
        query.bindValue(0,ToolFunctions::sec2string(t));
        query.bindValue(1,skillName);
        query.exec();
    }

}

QSqlDatabase SkillSql::getDb() const
{
    return db;
}

