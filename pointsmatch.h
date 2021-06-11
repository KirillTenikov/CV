#ifndef POINTSMATCH_H
#define POINTSMATCH_H

#include <QDialog>

namespace Ui {
class PointsMatch;
}

class PointsMatch : public QDialog
{
    Q_OBJECT

public:
    explicit PointsMatch(QWidget *parent = nullptr);
    ~PointsMatch();
    int getWindow();
    int getP();
    double getThreshold();
    int getN();

private:
    Ui::PointsMatch *ui;
};

#endif // POINTSMATCH_H
