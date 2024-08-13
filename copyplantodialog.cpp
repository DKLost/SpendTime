#include "copyplantodialog.h"
#include "ui_copyplantodialog.h"

CopyPlanToDialog::CopyPlanToDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CopyPlanToDialog)
{
    ui->setupUi(this);
    week = 0;
}

CopyPlanToDialog::~CopyPlanToDialog()
{
    delete ui;
}

int CopyPlanToDialog::getWeek() const
{
    return week;
}

void CopyPlanToDialog::on_buttonBox_accepted()
{
    week = ui->comboBox->currentIndex() + 1;
}

void CopyPlanToDialog::setWeek(int newWeek)
{
    week = newWeek;
}

