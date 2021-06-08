#ifndef POINTS_H
#define POINTS_H

#include <QDialog>

namespace Ui {
class points;
}

class points : public QDialog
{
    Q_OBJECT

public:
    explicit points(QWidget *parent = nullptr);
    ~points();
    int getWindow();
    int getP();
    double getThreshold();


private:
    Ui::points *ui;
};

#endif // POINTS_H
