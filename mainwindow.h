#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.h"
#include "filterdialog.h"
#include "gaussdialog.h"
#include "options.h"
#include "pyramiddialog.h"
#include "points.h"
#include "anms.h"
#include "pointsmatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_filtration_triggered();

    void on_gauss_triggered();

    void on_openFile_triggered();

    void on_xDerivative_triggered();

    void on_yDerivative_triggered();

    void on_sobel_triggered();

    void on_options_triggered();

    void on_save_triggered();

    void on_pyramid_triggered();

    void on_moraveck_triggered();

    void on_charis_triggered();

    void on_ANMS_triggered();

    void on_add_triggered();

    void on_matching_triggered();

private:
    Ui::MainWindow *ui;

    unique_ptr<FilterDialog>filterDialog;
    unique_ptr<GaussDialog>gaussDialog;
    unique_ptr<Options>options;
    unique_ptr<PyramidDialog>pyramidDialog;
    unique_ptr<points>pointsDialog;
    unique_ptr<anms>anmsDialog;
    unique_ptr<PointsMatch>pointsMatchDialog;



    unique_ptr<ImageFilterRep>cvImage;
    unique_ptr<ImageFilterRep>cvImageSecondary;


    void initImage(QString fileName);
    void showImage();
    void initSecondaryImage(QString fileName);
    void showImagePoints();
    void showPrmAndSecImg();
};
#endif // MAINWINDOW_H
