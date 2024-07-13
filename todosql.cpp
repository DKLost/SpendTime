#include "todosql.h"

TodoSql::TodoSql(QString fileName,QObject *parent)
    : QObject{parent}
{
    //init db
    dbFileName = fileName;
    db = QSqlDatabase::addDatabase("QSQLITE","connection02");
    db.setDatabaseName(dbFileName);
    db.open();

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS todolist(待办事项 TEXT PRIMARY KEY,状态 TEXT)");
}

void TodoSql::add_todo(QString todoName)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO todolist VALUES(?,'')");
    query.bindValue(0,todoName);
    query.exec();
}

void TodoSql::delete_todo(QString todoName)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM todolist WHERE 待办事项 = ?");
    query.bindValue(0,todoName);
    query.exec();
}

void TodoSql::complete_todo(QString todoName)
{
    QSqlQuery query(db);
    query.prepare("UPDATE todolist SET 状态 = '[✓]' WHERE 待办事项 = ?");
    query.bindValue(0,todoName);
    query.exec();
}

void TodoSql::change_todo_state(QString todoName)
{
    QSqlQuery query(db);
    query.prepare("SELECT 状态 FROM todolist WHERE 待办事项 = ?");
    query.bindValue(0,todoName);
    query.exec();

    while(query.next())
    {
        if(query.value(0).toString() == "")
        {
            query.prepare("UPDATE todolist SET 状态 = '[✓]' WHERE 待办事项 = ?");
            query.bindValue(0,todoName);
            query.exec();
        }
        else {
            query.prepare("UPDATE todolist SET 状态 = '' WHERE 待办事项 = ?");
            query.bindValue(0,todoName);
            query.exec();
        }
    }

}

QSqlDatabase TodoSql::getDb() const
{
    return db;
}
