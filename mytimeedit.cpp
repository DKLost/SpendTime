#include "mytimeedit.h"

MyTimeEdit::MyTimeEdit(QWidget *parent)
    : QTimeEdit{parent}
{
    setWrapping(true);
}

void MyTimeEdit::stepBy(int steps)
{
    int maxt = maximumTime().hour()*60 + maximumTime().minute();
    int mint = minimumTime().hour()*60 + minimumTime().minute();

    if (currentSection() == QDateTimeEdit::MinuteSection) {
        int t = time().hour() * 60 + (time().minute() + steps);
        if(steps > 0 && t > maxt)
            return;
        else if(steps < 0 && t <= mint)
            return;
        setTime(time().addSecs(steps*60));
    }
    else if (currentSection() == QDateTimeEdit::HourSection) {
        int t = (time().hour()+steps) * 60 + time().minute();
        if(steps > 0 && t > maxt)
        {
            setTime(maximumTime());
            return;
        }
        else if(steps < 0 && t <= mint)
        {
            setTime(minimumTime());
            return;
        }
        setTime(time().addSecs(steps*60*60));
    }
}
