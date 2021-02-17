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



double GaussDialog::getSigma()
{
   return  ui->lineEdit_2->text().toDouble();
}

bool GaussDialog::getSeparability()
{
    return ui->checkBox->isChecked();
}
