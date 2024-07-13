#ifndef MYTIMEEDIT_H
#define MYTIMEEDIT_H

#include <QWidget>
#include <QTimeEdit>

class MyTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    explicit MyTimeEdit(QWidget *parent = nullptr);

signals:
private:
    void stepBy(int steps);
};

#endif // MYTIMEEDIT_H
