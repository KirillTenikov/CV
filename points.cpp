#include "points.h"
#include "ui_points.h"

points::points(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::points)
{
    ui->setupUi(this);
}

points::~points()
{
    delete ui;
}

int points::getWindow()
{
     return ui->lineEdit->text().toInt();
}

int points::getP()
{
     return ui->lineEdit_2->text().toInt();
}

double points::getThreshold()
{
    return ui->lineEdit_3->text().toDouble();
}
