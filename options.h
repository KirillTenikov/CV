#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "image.h"

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    Mode getMode(){return mode;}

private slots:
    void on__clicked();

    void on_groupBox_clicked();

    void on_buttonBox_clicked();

private:
    Ui::Options *ui;
    Mode mode;
};

#endif // OPTIONS_H
