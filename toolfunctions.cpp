#include "toolfunctions.h"
#include <QTime>

ToolFunctions::ToolFunctions(){}

QString ToolFunctions::sec2string(int seconds)
{
    int s = seconds % 60;
    int m = seconds / 60 % 60;
    int h = seconds / 60 / 60;
    QString result = QString("%1h:%2m:%3s").arg(h).arg(m).arg(s);
    return result;
}

int ToolFunctions::string2sec(QString string)
{
    QStringList list = string.split(':');
    if(list.size() != 3)
        return -1;
    int h = list[0].remove('h').toInt();
    int m = list[1].remove('m').toInt();
    int s = list[2].remove('s').toInt();
    int result = h*60*60 + m*60 + s;
    return result;
}

QString ToolFunctions::timeDurationText(QTime startTime,QTime endTime)
{
    int d = (endTime.hour()*60 + endTime.minute()) - (startTime.hour()*60 + startTime.minute());
    QString text = QString("%1-%2(%3m)")
                           .arg(startTime.toString("HH:mm"))
                           .arg(endTime.toString("HH:mm"))
                           .arg(d);
    return text;
}
