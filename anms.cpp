#include "anms.h"
#include "ui_anms.h"

anms::anms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::anms)
{
    ui->setupUi(this);
}

anms::~anms()
{
    delete ui;
}

int anms::getNumber()
{
    return ui->lineEdit->text().toInt();
}
