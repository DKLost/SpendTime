#include "complexlineedit.h"

ComplexLineEdit::ComplexLineEdit(QWidget *parent)
    : QLineEdit{parent}
{
    postfixLabel = new QLabel(this);
    postfixLabel->setText("m");
    int xpos = 35;
    int ypos = 3;
    setTextMargins(0,0,13,0);
    postfixLabel->move(xpos,ypos);
}
