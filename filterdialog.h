#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <memory>
#include <vector>

using  namespace std;
namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();

    vector<double> getData();
    int getN(){return n;}
    bool getSeparability(){return separability;}

private slots:
    void on_pushButton_clicked();

private:
    Ui::FilterDialog *ui;
    vector<unique_ptr<QLineEdit>> widgets;
    int n=0;
    bool separability=false;
};

#endif // FILTERDIALOG_H
