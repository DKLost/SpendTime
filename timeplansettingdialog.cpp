#include "timeplansettingdialog.h"
#include "ui_timeplansettingdialog.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "toolfunctions.h"

TimePlanSettingDialog::TimePlanSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimePlanSettingDialog)
{
    ui->setupUi(this);

    //init dateTimeEdit
    ui->startTimeEdit->setCurrentSection(QDateTimeEdit::MinuteSection);
    ui->endTimeEdit->setCurrentSection(QDateTimeEdit::MinuteSection);\

    //init is_initing
    is_initing = true;

    //init timeList
    timeListNum = 0;

    //init currentDuration
    currentDuration = 25;
}

TimePlanSettingDialog::~TimePlanSettingDialog()
{
    delete ui;
}

void TimePlanSettingDialog::initDialog(QString action,int newCurrentWeek)
{
    is_initing = true;

    currentWeek = newCurrentWeek;
    QSqlDatabase db = QSqlDatabase::database("connection01");
    QSqlQuery query(db);
    query.exec("SELECT 行动 FROM skills");
    ui->actionComboBox->clear();
    while(query.next())
    {
        ui->actionComboBox->addItem(query.value(0).toString());
    }
    ui->actionComboBox->setCurrentText(action);

/*
    QTime startTime = unusedTimeList[0][0];
    int h = startTime.hour();
    int m = startTime.minute();

    m = m/5*5 + 10;
    if(m >= 60)
    {
        h += 1;
        m -= 60;
    }
    if(h >= 24)
    {
        h = 23;
        m = 59;
    }
    startTime.setHMS(h,m,0);
*/

    //duration
    ui->durationSpinBox->setValue(currentDuration);
    ui->durationSpinBox->setMinimum(1);
    ui->durationSpinBox->setMaximum(1440);

    ui->endTimeEdit->setTime(QTime::fromString("23:59:59"));

    initTimeList();

    is_initing = false;

    ui->unusedTimeComboBox->setCurrentIndex(-1);
    ui->unusedTimeComboBox->setCurrentIndex(timeListNum);
}

void TimePlanSettingDialog::initTimeList()
{
    ui->unusedTimeComboBox->clear();
    timeListNum = 0;
    unusedTimeList[0][0] = QTime(0,0,1);
    unusedTimeList[0][1] = QTime(23,59,59);

    QSqlDatabase db = QSqlDatabase::database("connection04");
    QSqlQuery query(db);
    query.prepare("SELECT * FROM timePlans WHERE week = ? ORDER BY 开始时间");
    query.bindValue(0,currentWeek);
    query.exec();

    QString dText = ToolFunctions::timeDurationText(unusedTimeList[timeListNum][0],unusedTimeList[timeListNum][1]);
    ui->unusedTimeComboBox->addItem(dText);
    while(query.next())
    {

        if(query.value(1).toString() != unusedTimeList[timeListNum][0].toString("HH:mm"))
        {
             unusedTimeList[timeListNum++][1] = query.value(1).toTime();
        }

        unusedTimeList[timeListNum][0] = query.value(2).toTime();
        unusedTimeList[timeListNum][1] = QTime(23,59,59);

        if(ui->unusedTimeComboBox->count() <= timeListNum)
        {
            if(timeListNum > 0)
            {
                QString dText = ToolFunctions::timeDurationText(unusedTimeList[timeListNum-1][0],unusedTimeList[timeListNum-1][1]);
                ui->unusedTimeComboBox->setItemText(timeListNum-1,dText);
            }
            QString dText = ToolFunctions::timeDurationText(unusedTimeList[timeListNum][0],unusedTimeList[timeListNum][1]);
            ui->unusedTimeComboBox->addItem(dText);
        }else
        {
            QString dText = ToolFunctions::timeDurationText(unusedTimeList[timeListNum][0],unusedTimeList[timeListNum][1]);
            ui->unusedTimeComboBox->setItemText(timeListNum,dText);
        }
    }
}

void TimePlanSettingDialog::on_startTimeEdit_userTimeChanged(const QTime &time)
{
    if(is_initing)
        return;

    QTime startTime = time;
    int duration = ui->durationSpinBox->value();
    QTime endTime = startTime.addSecs(duration * 60);
    QTime maxTime = unusedTimeList[ui->unusedTimeComboBox->currentIndex()][1];
    int h = startTime.hour();
    int m = startTime.minute();
    int h2 = maxTime.hour();
    int m2 = maxTime.minute();
    int maxDuration = (h2 - h) * 60 + (m2 - m);

    ui->endTimeEdit->setMinimumTime(startTime);
    ui->endTimeEdit->setTime(endTime);
    ui->durationSpinBox->setMaximum(maxDuration);

}


void TimePlanSettingDialog::on_durationSpinBox_valueChanged(int arg1)
{
    if(is_initing)
        return;

    int duration = arg1;
    QTime startTime = ui->startTimeEdit->time();
    QTime endTime = startTime.addSecs(duration * 60);
    int index = ui->unusedTimeComboBox->currentIndex();
    ui->startTimeEdit->setMaximumTime(unusedTimeList[index][1].addSecs(-duration * 60));
    ui->endTimeEdit->setTime(endTime);
}


void TimePlanSettingDialog::on_endTimeEdit_userTimeChanged(const QTime &time)
{
    if(is_initing)
        return;

    QTime endTime = time;
    QTime startTime = ui->startTimeEdit->time();
    int duration = (endTime.hour()*60 + endTime.minute()) - (startTime.hour()*60 + startTime.minute());
    ui->durationSpinBox->setValue(duration);
}


void TimePlanSettingDialog::on_buttonBox_accepted()
{
    timePlanSql->addPlan(currentWeek,ui->startTimeEdit->time(),ui->endTimeEdit->time(),ui->actionComboBox->currentText());
}

void TimePlanSettingDialog::setTimePlanSql(TimePlanSql *newTimePlanSql)
{
    timePlanSql = newTimePlanSql;
}

void TimePlanSettingDialog::on_unusedTimeComboBox_currentIndexChanged(int index)
{
    if(index == -1 || is_initing)
        return;

    //duration
    int duration = ui->durationSpinBox->value();
    int h1 = unusedTimeList[index][0].hour();
    int m1 = unusedTimeList[index][0].minute();
    int h2 = unusedTimeList[index][1].hour();
    int m2 = unusedTimeList[index][1].minute();
    int maxDuration = (h2 - h1) * 60 + (m2 - m1);

    duration = std::min(maxDuration,duration);
    ui->durationSpinBox->setMaximum(maxDuration);
    ui->durationSpinBox->setValue(duration);

    is_initing = true;
    ui->startTimeEdit->setMaximumTime(unusedTimeList[index][1].addSecs(-duration * 60));
    ui->startTimeEdit->setMinimumTime(unusedTimeList[index][0]);
    ui->endTimeEdit->setMaximumTime(unusedTimeList[index][1]);
    ui->endTimeEdit->setMinimumTime(unusedTimeList[index][0]);
    is_initing = false;

    ui->startTimeEdit->setTime(unusedTimeList[index][0]);
}

int TimePlanSettingDialog::getCurrentDuration() const
{
    return currentDuration;
}

void TimePlanSettingDialog::setCurrentDuration(int newCurrentDuration)
{
    currentDuration = newCurrentDuration;
}

int TimePlanSettingDialog::getCurrentWeek() const
{
    return currentWeek;
}

void TimePlanSettingDialog::setCurrentWeek(int newCurrentWeek)
{
    currentWeek = newCurrentWeek;
}

