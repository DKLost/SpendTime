#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolfunctions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init timePlanSettingDialog
    timePlanSettingDialog = new TimePlanSettingDialog{this};

    //init timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,&QTimer::timeout,this,&MainWindow::timerHandler);

    //init stayTimer
    stayTimer1 = new QTimer(this);
    stayTimer1->setInterval(1000);
    connect(stayTimer1,&QTimer::timeout,this,&MainWindow::stayTimerHandler1);


    stayTimer2 = new QTimer(this);
    stayTimer2->setInterval(1000);
    connect(stayTimer2,&QTimer::timeout,this,&MainWindow::stayTimerHandler2);

    stayTimer1->start();
    stayTimer2->start();

    //init skillsql
    skillSql = new SkillSql("skills.db");
    tableModel = new QSqlTableModel(this,skillSql->getDb());
    tableModel->setTable("skills");
    tableModel->select();
    ui->skillTableView->setModel(tableModel);
    ui->skillTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->skillTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //init todosql
    todoSql = new TodoSql("todo.db");
    todoTableModel = new QSqlTableModel(this,todoSql->getDb());
    todoTableModel->setTable("todolist");
    todoTableModel->select();
    ui->todoTableView->setModel(todoTableModel);
    ui->todoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->todoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->todoTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);

    //init timeRecordSql
    timeRecordSql = new TimeRecordSql("record.db");
    timeRecordTableModel = new QSqlTableModel(this,timeRecordSql->getDb());
    timeRecordTableModel->setTable("timeRecords");
    QString queryString = QString("SELECT * FROM timeRecords WHERE 日期 = '%1'")
                              .arg(QDate::currentDate().toString("yyyy/MM/dd"));
    timeRecordTableModel->setQuery(queryString,timeRecordSql->getDb());
    ui->timeRecordTableView->setModel(timeRecordTableModel);
    ui->timeRecordTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->timeRecordTableView->hideColumn(0);
    ui->timeRecordTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //init timePlanSql
    timePlanSql = new TimePlanSql("timePlan.db");
    timePlanTableModel = new QSqlTableModel(this,timePlanSql->getDb());
    timePlanTableModel->setTable("timePlans");
    ui->timePlanTableView->setModel(timePlanTableModel);
    QString queryString2 = QString("SELECT * FROM timePlans WHERE week = %1")
                              .arg(QDate::currentDate().dayOfWeek());
    timePlanTableModel->setQuery(queryString2,timePlanSql->getDb());
    ui->timePlanTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->timePlanTableView->hideColumn(0);
    ui->timePlanTableView->hideColumn(1);
    ui->timePlanTableView->hideColumn(2);
    timePlanTableModel->setSort(1,Qt::AscendingOrder);
    ui->timePlanTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //init dateEdit
    ui->dateEdit->setDate(QDate::currentDate());

    //init comboBox
    update_comboBox();
    ui->comboBox->setProperty("isLocked",false);

    //init timerLineEdit
    ui->timerLineEdit->setAlignment(Qt::AlignHCenter);

    //init musicPlayer
    musicPlayer = new MusicPlayer{this};
    QFile file("config.dat");
    file.open(QIODevice::ReadWrite);
    QDataStream fs(&file);
    QString fileName;
    fs >> fileName;
    musicPlayer->setMp3FilePath(fileName);

    //init timePlanSettingDialog
    timePlanSettingDialog->setTimePlanSql(timePlanSql);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_comboBox()
{
    QString old_text = ui->comboBox->currentText();
    ui->comboBox->clear();
    QSqlQuery query(skillSql->getDb());
    query.exec("SELECT 行动 FROM skills");
    while(query.next())
    {
        ui->comboBox->addItem(query.value(0).toString());
    }
    ui->comboBox->setCurrentText(old_text);
}

void MainWindow::update_time_sum(QDate day)
{
    QSqlQuery query1(skillSql->getDb());
    QSqlQuery query2(skillSql->getDb());
    query1.exec("SELECT 行动 FROM skills");
    while(query1.next())
    {
        QString skillName = query1.value(0).toString();
        int total_minutes = timeRecordSql->sum_minutes(day,skillName);
        int h = total_minutes / 60;
        int m = total_minutes % 60;
        QString total_time = QString("%1h:%2m").arg(h).arg(m);
        query2.prepare("UPDATE skills SET 当日时长 = ? WHERE 行动 = ?");
        query2.bindValue(0,total_time);
        query2.bindValue(1,skillName);
        query2.exec();
    }
}

void MainWindow::on_skillAddButton_clicked()
{
    QString skillName = ui->skillLineEdit->text();
    skillSql->add_skill(skillName);
    tableModel->select();
    update_comboBox();
}


void MainWindow::on_skillTableView_clicked(const QModelIndex &index)
{
    QString skillName = index.siblingAtColumn(0).data().toString();
    if(!ui->comboBox->property("isLocked").toBool())
    {
        ui->comboBox->setCurrentText(skillName);
    }
}


void MainWindow::on_timerButton_clicked()
{
    QString skillName = ui->comboBox->currentText();
    if(ui->timerButton->text() == "开始计时")
    {
        ui->timerButton->setText("结束计时");

        //set oneTimeShot
        if(ui->shotTimeLineEdit->text() == "")
            oneTimeShot = ui->shotTimeLineEdit->placeholderText().toInt() * 60;
        else
            oneTimeShot = ui->shotTimeLineEdit->text().toInt() * 60;

        timer->start();
        timeCounter = 0;
        ui->timerLineEdit->setText(ToolFunctions::sec2string(timeCounter));
        ui->comboBox->setProperty("isLocked",true);
        ui->comboBox->setEnabled(false);
        ui->shotTimeLineEdit->setEnabled(false);
    }
    else
    {
        ui->timerButton->setText("开始计时");
        timer->stop();
        ui->comboBox->setProperty("isLocked",false);
        ui->comboBox->setEnabled(true);
        ui->shotTimeLineEdit->setEnabled(true);

        skillSql->increase_skill_time(skillName,timeCounter);

        //add time record
        timeRecordSql->add_record(QTime::currentTime(),timeCounter,skillName);
        ui->dateEdit->setDate(QDate::currentDate());
        on_dateEdit_userDateChanged(QDate::currentDate());
    }
}

void MainWindow::popTimeOverBox()
{
    musicPlayer->playMusic();
    QMessageBox msgBox;
    msgBox.setText("计时已结束");
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
    msgBox.exec();
    musicPlayer->stopMusic();
}
void MainWindow::timerHandler()
{
    timeCounter++;
    ui->timerLineEdit->setText(ToolFunctions::sec2string(timeCounter));
    if(timeCounter >= oneTimeShot)
    {
        on_timerButton_clicked();
        popTimeOverBox();
    }
}

void MainWindow::stayTimerHandler1()
{
    QTime currentTime = QTime::currentTime();
    ui->timeLabel->setText(currentTime.toString("HH:mm:ss"));
    if(currentTime == QTime::fromString("23:59:59"))
    {
        if(ui->timerButton->text() == "结束计时")
        {
            on_timerButton_clicked();
            popTimeOverBox();
        }
    }
    if(currentTime == QTime::fromString("00:00:01"))
    {
        ui->dateEdit->setDate(QDate::currentDate());
    }
}

void MainWindow::stayTimerHandler2()
{

    QTime currentTime = QTime::currentTime();
    if(currentTime.second() != 0)
        return;

    QSqlQuery query(timePlanSql->getDb());

    query.prepare("SELECT * FROM timePlans WHERE week = ? AND 开始时间 = ?");
    query.bindValue(0,QDate::currentDate().dayOfWeek());
    query.bindValue(1,currentTime.toString("HH:mm"));
    query.exec();
    if(query.next())
    {
        musicPlayer->playMusic();
        QMessageBox msgBox;
        QTime endTime = query.value(2).toTime();
        connect(&msgBox,&QMessageBox::buttonClicked,this,[=](){
            QTime startTime = QTime::currentTime();
            int d = (endTime.hour() - startTime.hour()) * 60 + endTime.minute() - startTime.minute();
            if(d <= 0)
                return;

            if(ui->timerButton->text() == "结束计时")
                on_timerButton_clicked();

            ui->shotTimeLineEdit->setText(QString::number(d));
            on_timerButton_clicked();
        });
        QString text = QString("<p align='center'>%1<br>%2</p>")
                           .arg(query.value(4).toString())
                           .arg(query.value(3).toString());
        msgBox.setText(text);
        QSpacerItem* horizontalSpacer = new QSpacerItem(125, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 1, 1, layout->columnCount());

        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
        msgBox.exec();
        musicPlayer->stopMusic();
    }
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->timerLineEdit->clear();
}


void MainWindow::on_skillDelButton_clicked()
{
    QModelIndex index = ui->skillTableView->currentIndex();
    QString skillName = index.siblingAtColumn(0).data().toString();
    if(skillName == ui->comboBox->currentText() && ui->comboBox->property("isLocked").toBool())
    {
        QMessageBox::critical(this,"错误","计时中不可删除行动");
        return;
    }
    skillSql->delete_skill(skillName);
    tableModel->select();
    ui->comboBox->setCurrentText("");
    update_comboBox();
}


void MainWindow::on_todoAddButton_clicked()
{
    QString todoName = ui->skillLineEdit->text();
    todoSql->add_todo(todoName);
    todoTableModel->select();
}


void MainWindow::on_todoChangeButton_clicked()
{
    QModelIndex index = ui->todoTableView->currentIndex();
    QString todoName = index.siblingAtColumn(0).data().toString();
    todoSql->change_todo_state(todoName);
    todoTableModel->select();
    ui->todoTableView->selectRow(index.row());
}

void MainWindow::on_todoDelButton_clicked()
{
    QModelIndex index = ui->todoTableView->currentIndex();
    QString todoName = index.siblingAtColumn(0).data().toString();
    todoSql->delete_todo(todoName);
    todoTableModel->select();
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    QString queryString = QString("SELECT * FROM timeRecords WHERE 日期 = '%1'")
                              .arg(date.toString("yyyy/MM/dd"));
    timeRecordTableModel->setQuery(queryString,timeRecordSql->getDb());
    update_time_sum(date);
    tableModel->select();
    ui->weekComboBox->setCurrentIndex(date.dayOfWeek() - 1);
}


void MainWindow::on_setMusicButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            "打开音乐", "./", tr("Music Files (*.mp3 *.wav *.flac)"));
    if(fileName.isNull() || fileName.isEmpty() || fileName == "")
        return;
    QFile file("config.dat");
    file.open(QIODevice::ReadWrite);
    QDataStream fs(&file);
    fs << fileName;
    musicPlayer->setMp3FilePath(fileName);
}


void MainWindow::on_addTimePlanButton_clicked()
{
    QString action = ui->comboBox->currentText();
    timePlanSettingDialog->initDialog(action,ui->weekComboBox->currentIndex() + 1);
    timePlanSettingDialog->exec();
    QString queryString = QString("SELECT * FROM timePlans WHERE week = %1")
                              .arg(ui->weekComboBox->currentIndex() + 1);
    timePlanTableModel->setQuery(queryString,timePlanSql->getDb());
}


void MainWindow::on_delTimePlanButton_clicked()
{
    QModelIndex index = ui->timePlanTableView->currentIndex();
    int week = index.siblingAtColumn(0).data().toInt();
    QTime startTime = index.siblingAtColumn(1).data().toTime();
    timePlanSql->delPlan(week,startTime);
    timePlanTableModel->setSort(1,Qt::AscendingOrder);
    QString queryString = QString("SELECT * FROM timePlans WHERE week = %1")
                              .arg(ui->weekComboBox->currentIndex() + 1);
    timePlanTableModel->setQuery(queryString,timePlanSql->getDb());
}


void MainWindow::on_weekComboBox_currentIndexChanged(int index)
{
    QString queryString = QString("SELECT * FROM timePlans WHERE week = %1")
                               .arg(index + 1);
    timePlanTableModel->setQuery(queryString,timePlanSql->getDb());
}


void MainWindow::on_shotTimeLineEdit_textChanged(const QString &arg1)
{
    int duration = arg1.toInt();
    if(duration == 0)
        duration = ui->shotTimeLineEdit->placeholderText().toInt();
    timePlanSettingDialog->setCurrentDuration(duration);
}

