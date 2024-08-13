#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QFileDialog>
#include "skillsql.h"
#include "todosql.h"
#include "timerecordsql.h"
#include "musicplayer.h"
#include "timeplansettingdialog.h"
#include "timeplansql.h"
#include "copyplantodialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    TimePlanSettingDialog *timePlanSettingDialog;
    SkillSql *skillSql;
    TodoSql *todoSql;
    TimeRecordSql *timeRecordSql;
    TimePlanSql *timePlanSql;
    QSqlTableModel *tableModel;
    QSqlTableModel *todoTableModel;
    QSqlTableModel *timeRecordTableModel;
    QSqlTableModel *timePlanTableModel;

    QTimer *timer;
    QTimer *stayTimer1;
    QTimer *stayTimer2;
    MusicPlayer *musicPlayer;
    int timeCounter;
    int oneTimeShot;
    void update_comboBox();
    void update_time_sum(QDate day);
    void popTimeOverBox();

private slots:
    void on_skillAddButton_clicked();
    void on_skillTableView_clicked(const QModelIndex &index);
    void on_timerButton_clicked();
    void timerHandler();
    void stayTimerHandler1();
    void stayTimerHandler2();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_skillDelButton_clicked();

    void on_todoAddButton_clicked();
    void on_todoChangeButton_clicked();

    void on_todoDelButton_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_setMusicButton_clicked();

    void on_addTimePlanButton_clicked();


    void on_delTimePlanButton_clicked();

    void on_weekComboBox_currentIndexChanged(int index);

    void on_shotTimeLineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    CopyPlanToDialog *copyPlanToDialog;
};
#endif // MAINWINDOW_H
