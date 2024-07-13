#ifndef COMPLEXLINEEDIT_H
#define COMPLEXLINEEDIT_H

#include <QLineEdit>
#include <QLabel>

class ComplexLineEdit : public QLineEdit
{
    Q_OBJECT
public:

    explicit ComplexLineEdit(QWidget *parent = nullptr);
    QLabel *postfixLabel;

signals:
};

#endif // COMPLEXLINEEDIT_H
