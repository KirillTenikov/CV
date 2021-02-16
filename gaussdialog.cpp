#include "gaussdialog.h"
#include "ui_gaussdialog.h"

GaussDialog::GaussDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussDialog)
{
    ui->setupUi(this);
}

GaussDialog::~GaussDialog()
{
    delete ui;
}

int GaussDialog::getN()
{
   return ui->lineEdit->text().toInt();
}

double GaussDialog::getSigma()
{
   return  ui->lineEdit_2->text().toDouble();
}

bool GaussDialog::getSeparability()
{
    return ui->checkBox->isChecked();
}
