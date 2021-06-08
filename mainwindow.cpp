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
    ui->menu_4->setEnabled(false);
    ui->menu_5->setEnabled(false);
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
    cvImage =unique_ptr<ImageFilterRep>(new ImageFilterRep(processRawData(img.bits(),img.sizeInBytes(),RenderImage::Format::PAL),img.width(),img.height()));
}

void MainWindow::showImage()
{
    unsigned char* data=RenderImage::render(cvImage->getData());
    QImage img=QImage(data, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
    ui->label->setPixmap(QPixmap::fromImage(img));
    delete [] data;

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
      ui->menu_4->setEnabled(true);
      ui->menu_5->setEnabled(true);

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
    unsigned char*data=RenderImage::render(cvImage->getData());
    QImage img=QImage(data, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
    QString fileName=QFileDialog::getSaveFileName(this,"Сохранить файл",QDir::homePath(), "Image Files (*.png)");
    if (!fileName.isEmpty())
        img.save(fileName);
    delete [] data;
}


void MainWindow::on_pyramid_triggered()
{
    pyramidDialog=unique_ptr<PyramidDialog>(new PyramidDialog(this));
    pyramidDialog->setModal(true);
    if(pyramidDialog->exec()==QDialog::Accepted)
     {
       cvImage->buildPyramid(pyramidDialog->getOctaves(),pyramidDialog->getLevels(),pyramidDialog->getAlphaSigma(), pyramidDialog->getSigma());
       vector<vector<ImageFilterRep>>cvImages=cvImage->getPyramid();
       for(int i=0;i<cvImages.size();i++)
           for(int j=0;j<cvImages[i].size();j++) {
             unsigned char*data=RenderImage::render(cvImages[i][j].getData());
             QImage img=QImage(data,cvImages[i][j].getWidth(),cvImages[i][j].getHeight(),QImage::Format::Format_ARGB32);
             QString fileName=QDir::homePath()+"/sigma_"+QString::number(cvImages[i][j].getSigma())+"effectivesigma_"+QString::number(cvImages[i][j].getEffectiveSigma())+".png";
             if (!fileName.isEmpty())
               img.save(fileName);
             delete [] data;
           }
    }
}


void MainWindow::on_moraveck_triggered()
{
    pointsDialog=unique_ptr<points>(new points(this));
    pointsDialog->setModal(true);

    if(pointsDialog->exec()==QDialog::Accepted)
    {
      ImageFilterRep cvImageMaxMin=cvImage->moraveck(pointsDialog->getWindow(),pointsDialog->getP());
      unsigned char*initialData=RenderImage::renderWithPointsDetector(cvImage->getData().size(),cvImage->getIndexesPoints());
      unsigned char*dataMaxMin=RenderImage::renderMaxMin(cvImageMaxMin.getData());
      cvImage->filterThreshold(pointsDialog->getThreshold(), cvImageMaxMin);
      unsigned char*intermdediateData=RenderImage::renderWithPointsDetector(cvImage->getData().size(),cvImage->getIndexesPoints());
      unsigned char*finalData=RenderImage::renderWithPoints(cvImage->getData(),cvImage->getIndexesPoints());
      QImage imgIntial=QImage(initialData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
      QImage imgIntermediate=QImage(intermdediateData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
      QImage imgFinal=QImage(finalData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
      QImage imgMaxMin=QImage(dataMaxMin, cvImageMaxMin.getWidth(),cvImageMaxMin.getHeight(),QImage::Format::Format_ARGB32);
      ui->label->setPixmap(QPixmap::fromImage(imgFinal));
      QString fileName=QDir::homePath()+"/image_max.png";
      QString fileNameInitial=QDir::homePath()+"/image_detector.png";
      QString fileNameIntermediate=QDir::homePath()+"/image_detector_threshold.png";
      QString fileNameFinal=QDir::homePath()+"/image_final.png";
      imgMaxMin.save(fileName);
      imgIntial.save(fileNameInitial);
      imgIntermediate.save(fileNameIntermediate);
      imgFinal.save(fileNameFinal);


      delete [] initialData;
      delete [] dataMaxMin;
      delete [] intermdediateData;

    }
}

void MainWindow::on_charis_triggered()
{
    pointsDialog=unique_ptr<points>(new points(this));
    pointsDialog->setModal(true);
    if(pointsDialog->exec()==QDialog::Accepted)
    {
        ImageFilterRep cvImageMaxMin=cvImage->harris(pointsDialog->getWindow(),pointsDialog->getP());
        unsigned char*initialData=RenderImage::renderWithPointsDetector(cvImage->getData().size(),cvImage->getIndexesPoints());
        unsigned char*dataMaxMin=RenderImage::renderMaxMin(cvImageMaxMin.getData());
        cvImage->filterThreshold(pointsDialog->getThreshold(), cvImageMaxMin);
        unsigned char*intermdediateData=RenderImage::renderWithPointsDetector(cvImage->getData().size(),cvImage->getIndexesPoints());
        unsigned char*finalData=RenderImage::renderWithPoints(cvImage->getData(),cvImage->getIndexesPoints());
        QImage imgIntial=QImage(initialData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
        QImage imgIntermediate=QImage(intermdediateData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
        QImage imgFinal=QImage(finalData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
        QImage imgMaxMin=QImage(dataMaxMin, cvImageMaxMin.getWidth(),cvImageMaxMin.getHeight(),QImage::Format::Format_ARGB32);
        ui->label->setPixmap(QPixmap::fromImage(imgFinal));
        QString fileName=QDir::homePath()+"/image_max.png";
        QString fileNameInitial=QDir::homePath()+"/image_detector.png";
        QString fileNameIntermediate=QDir::homePath()+"/image_detector_threshold.png";
        QString fileNameFinal=QDir::homePath()+"/image_final.png";
        imgMaxMin.save(fileName);
        imgIntial.save(fileNameInitial);
        imgIntermediate.save(fileNameIntermediate);
        imgFinal.save(fileNameFinal);

        delete [] initialData;
        delete [] dataMaxMin;
        delete [] intermdediateData;

    }

}

void MainWindow::on_ANMS_triggered()
{
    anmsDialog=unique_ptr<anms>(new anms(this));
    anmsDialog->setModal(true);
    if(anmsDialog->exec()==QDialog::Accepted)
    {
      cvImage->anms(anmsDialog->getNumber());
      unsigned char*data=RenderImage::renderWithPoints(cvImage->getData(),cvImage->getIndexesPoints());
      unsigned char*intermdediateData=RenderImage::renderWithPointsDetector(cvImage->getData().size(),cvImage->getIndexesPoints());
      QImage imgFinal=QImage(data, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
      QImage imgIntermediate=QImage(intermdediateData, cvImage->getWidth(),cvImage->getHeight(),QImage::Format::Format_ARGB32);
      ui->label->setPixmap(QPixmap::fromImage(imgFinal));
      QString fileNameIntermediate=QDir::homePath()+"/image_detector_threshold_anms.png";
      QString fileNameFinal=QDir::homePath()+"/image_final_anms.png";
      imgFinal.save(fileNameFinal);
      imgIntermediate.save(fileNameIntermediate);


      delete [] data;

    }
}
