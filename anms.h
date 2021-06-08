#ifndef ANMS_H
#define ANMS_H

#include <QDialog>

namespace Ui {
class anms;
}

class anms : public QDialog
{
    Q_OBJECT

public:
    explicit anms(QWidget *parent = nullptr);
    ~anms();
    int getNumber();

private:
    Ui::anms *ui;
};

#endif // ANMS_H
