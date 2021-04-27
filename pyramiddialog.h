#ifndef PYRAMIDDIALOG_H
#define PYRAMIDDIALOG_H

#include <QDialog>

namespace Ui {
class PyramidDialog;
}

class PyramidDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PyramidDialog(QWidget *parent = nullptr);
    ~PyramidDialog();
    double getSigma();
    double getAlphaSigma();
    int getLevels();
    int getOctaves();

private:
    Ui::PyramidDialog *ui;
};

#endif // PYRAMIDDIALOG_H
