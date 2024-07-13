#ifndef TODOSQL_H
#define TODOSQL_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QMessageBox>

class TodoSql : public QObject
{
    Q_OBJECT
public:
    explicit TodoSql(QString fileName,QObject *parent = nullptr);
    void add_todo(QString todoName);
    void delete_todo(QString todoName);
    void complete_todo(QString todoName);
    void change_todo_state(QString todoName);

    QSqlDatabase getDb() const;

private:
    QSqlDatabase db;
    QString dbFileName;

signals:
};

#endif // TODOSQL_H
