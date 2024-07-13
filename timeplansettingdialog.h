#ifndef TIMEPLANSETTINGDIALOG_H
#define TIMEPLANSETTINGDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include "mytimeedit.h"
#include "timeplansql.h"

namespace Ui {
class TimePlanSettingDialog;
}

class TimePlanSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimePlanSettingDialog(QWidget *parent = nullptr);
    ~TimePlanSettingDialog();
    void initDialog(QString action,int newCurrentWeek);
    void initTimeList();
    void setTimePlanSql(TimePlanSql *newTimePlanSql);

    int getCurrentWeek() const;
    void setCurrentWeek(int newCurrentWeek);

    int getCurrentDuration() const;
    void setCurrentDuration(int newCurrentDuration);

private slots:
    void on_startTimeEdit_userTimeChanged(const QTime &time);
    void on_durationSpinBox_valueChanged(int arg1);
    void on_endTimeEdit_userTimeChanged(const QTime &time);
    void on_buttonBox_accepted();
    void on_unusedTimeComboBox_currentIndexChanged(int index);

private:
    bool is_initing;
    TimePlanSql *timePlanSql;
    Ui::TimePlanSettingDialog *ui;
    int timeListNum;
    int currentWeek;
    int currentDuration;
    QTime unusedTimeList[1500][2];
};

#endif // TIMEPLANSETTINGDIALOG_H
