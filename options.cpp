#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::on__clicked()
{

}

void Options::on_groupBox_clicked()
{

}

void Options::on_buttonBox_clicked()
{
    if (ui->radioButton->isChecked())mode=Mode::basic;
     else if(ui->radioButton_2->isChecked())mode=Mode::continuation;
      else if(ui->radioButton_3->isChecked())mode=Mode::reflection;

}
