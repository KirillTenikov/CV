#include "pointsmatch.h"
#include "ui_pointsmatch.h"

PointsMatch::PointsMatch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointsMatch)
{
    ui->setupUi(this);
}

PointsMatch::~PointsMatch()
{
    delete ui;
}

int PointsMatch::getWindow()
{
     return ui->lineEdit->text().toInt();
}

int PointsMatch::getP()
{
     return ui->lineEdit_2->text().toInt();
}

double PointsMatch::getThreshold()
{
    return ui->lineEdit_3->text().toDouble();
}

int PointsMatch::getN()
{
    return ui->lineEdit_4->text().toInt();
}

