#ifndef COPYPLANTODIALOG_H
#define COPYPLANTODIALOG_H

#include <QDialog>

namespace Ui {
class CopyPlanToDialog;
}

class CopyPlanToDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyPlanToDialog(QWidget *parent = nullptr);
    ~CopyPlanToDialog();

    int getWeek() const;

    void setWeek(int newWeek);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CopyPlanToDialog *ui;
    int week;
};

#endif // COPYPLANTODIALOG_H
