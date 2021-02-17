#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menu_2->setEnabled(false);
    ui->menu_3->setEnabled(false);
    ui->save->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initImage(QString fileName)
{
    QImage img;
    img.load(fileName);
    img=img.convertToFormat(QImage::Format::Format_ARGB32,0);
    ui->label->setScaledContents(true);
    render=unique_ptr<RenderImage>(new RenderImage(img.bits(),img.sizeInBytes()));
    cvImage =unique_ptr<ImageFilterRep>(new ImageFilterRep(render->processRawData(Format::PAL),img.width(),img.height()));
}

void MainWindow::showImage()
{
    QImage img=QImage(render->render(cvImage->getData()),cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
    ui->label->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::on_openFile_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"Открыть файл",QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
    if(!fileName.isEmpty()){
    initImage(fileName);
    showImage();
    if(cvImage)
    {
      ui->menu_2->setEnabled(true);
      ui->menu_3->setEnabled(true);
      ui->save->setEnabled(true);

    }
    }
}



void MainWindow::on_filtration_triggered()
{
    filterDialog=unique_ptr<FilterDialog>(new FilterDialog(this));
    filterDialog->setModal(true);
    if(filterDialog->exec()==QDialog::Accepted)
     {
        ImageFilterRep filter=ImageFilterRep(filterDialog->getData(), filterDialog->getN(),filterDialog->getN());
        filter.setSeparability(filterDialog->getSeparability());
        *cvImage*=filter;
        showImage();
    }
}

void MainWindow::on_gauss_triggered()
{
    gaussDialog=unique_ptr<GaussDialog>(new GaussDialog(this));
    gaussDialog->setModal(true);
    if(gaussDialog->exec()==QDialog::Accepted)
     {
        ImageFilterRep filter=ImageFilterRep::getGaussKernel(gaussDialog->getSigma());
        filter.setSeparability(gaussDialog->getSeparability());
        *cvImage*=filter;
        showImage();
    }
}


void MainWindow::on_xDerivative_triggered()
{
    ImageFilterRep filter=ImageFilterRep::getSobelXDerivative();
    *cvImage*=filter;
    showImage();
}

void MainWindow::on_yDerivative_triggered()
{
    ImageFilterRep filter=ImageFilterRep::getSobelYDerivative();
    *cvImage*=filter;
    showImage();
}

void MainWindow::on_sobel_triggered()
{
    cvImage->sobel();
    showImage();

}

void MainWindow::on_options_triggered()
{
    options=unique_ptr<Options>(new Options(this));
    options->setModal(true);
    if(options->exec()==QDialog::Accepted)
     cvImage->setMode(options->getMode());

}
void MainWindow::on_save_triggered()
{
    QImage img=QImage(render->render(cvImage->getData()),cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
    QString fileName=QFileDialog::getSaveFileName(this,"Сохранить файл",QDir::homePath(), "Image Files (*.png)");
    if (!fileName.isEmpty())
        img.save(fileName);
}
