#include "pyramiddialog.h"
#include "ui_pyramiddialog.h"

PyramidDialog::PyramidDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PyramidDialog)
{
    ui->setupUi(this);
}

PyramidDialog::~PyramidDialog()
{
    delete ui;
}

int PyramidDialog::getOctaves()
{
    return ui->lineEdit->text().toInt();
}


int PyramidDialog::getLevels()
{
    return ui->lineEdit_2->text().toInt();
}


double  PyramidDialog::getSigma()
{
    return ui->lineEdit_3->text().toDouble();
}

double  PyramidDialog::getAlphaSigma()
{
    return ui->lineEdit_4->text().toDouble();
}
