#ifndef GAUSSDIALOG_H
#define GAUSSDIALOG_H

#include <QDialog>

namespace Ui {
class GaussDialog;
}

class GaussDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussDialog(QWidget *parent = nullptr);
    ~GaussDialog();

    int getN();
    double getSigma();
    bool getSeparability();
private:
    Ui::GaussDialog *ui;

};

#endif // GAUSSDIALOG_H
