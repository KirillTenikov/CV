#include "filterdialog.h"
#include "ui_filterdialog.h"


FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
}
FilterDialog::~FilterDialog()
{
   delete ui;
}

vector<double> FilterDialog::getData()
{
    vector<double>data=vector<double>(widgets.size());
    for(pair<vector<double>::iterator,vector<unique_ptr<QLineEdit>>::iterator> it(data.begin(),widgets.begin());
        it.first!=data.end();
        ++it.first,++it.second)
      *it.first=it.second->get()->text().toDouble();

    return data;


}

void FilterDialog::on_pushButton_clicked()
{

    n=ui->lineEdit->text().toInt();
    widgets=vector<unique_ptr<QLineEdit>>((n*2+1)*(n*2+1));
    for(vector<unique_ptr<QLineEdit>>::iterator it=widgets.begin();it!=widgets.end(); ++it){
      *it=unique_ptr<QLineEdit>(new QLineEdit(this));
       ui->groupBox_2->layout()->addWidget(it->get());
    separability=ui->checkBox->isChecked();
 }


}


