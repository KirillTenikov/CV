#ifndef IMAGE_H
#define IMAGE_H
#include "descriptor.h"
using namespace std;

struct Point {
    int x, y;
};


enum Mode{basic, continuation, reflection};

class ImageFilterRep
{
public:
    ImageFilterRep (vector<double> aData, int aWidth, int aHeight);

    static ImageFilterRep getGaussKernel(double sigma);
    static ImageFilterRep getGaussWithEvenKernel(int width);
    static ImageFilterRep getGaussX(double sigma);
    static ImageFilterRep getGaussY(double sigma);
    static ImageFilterRep getLaplassianKernel(double sigma);
    static ImageFilterRep getSobelXDerivative();
    static ImageFilterRep getSobelYDerivative();

    ImageFilterRep getFilterX();
    ImageFilterRep getFilterY();


    double& getPoint(int x, int y);
    double& getPointEvenKernel(int x, int y);
    double& getPixel(int x, int y);
    double  getPixelValue(int x, int y);
    double getPixelValueInterpolation(double x, double y);
    double L(int x, int y, double sigma);
    double interpolatedL(double x, double y, double sigma);


    vector<double>getData(){return data;}
    vector<Point>getPoints(){return points;}
    vector<int>getIndexesPoints(){return indexes;}
    vector<Descriptor> getDescriptors(){return descriptors;}
    int getWidth(){return width;}
    int getHeight(){return height;}
    int getKernerlWidth(){return kernelWidth;}
    int getKernerlHeight(){return kernelHeight;}
    bool isSeparable(){return separable;}
    void setSeparability(bool aSeparable){separable=aSeparable;}
    void setMode(Mode aMode){mode=aMode;}
    vector<vector<ImageFilterRep>> getPyramid(){return pyramid;}
    double getSigma(){return sigma;}
    double getEffectiveSigma(){return effectiveSigma;}
    void setSigma(double aSigma){sigma=aSigma;}
    void setEffectiveSigma(double aEffetctiveSigma){effectiveSigma=aEffetctiveSigma;}

    ImageFilterRep& downSample();
    void buildPyramid(int octaves, int levels, double alphaSigma, double sigma);



    void getMaxPoints(int p, ImageFilterRep &minMax);
    void filterThreshold(double t, ImageFilterRep &minMax);
    ImageFilterRep moraveck(int window, int p);
    ImageFilterRep harris(int window,int p);
    void anms(int number);




    ImageFilterRep& power(double power);

    ImageFilterRep& sobel();
    ImageFilterRep& sobelAngle();

    void calcDescriptors(int  n, int gridX, int gridY, int  pixels);
    void calcDescriptorsRI(int  n0, int n1, double sigma);
    vector<pair<Point, Point>> findClosePoints(ImageFilterRep &image2);






    ImageFilterRep& operator *=(ImageFilterRep filter);
    ImageFilterRep& operator+=(ImageFilterRep image);
    ImageFilterRep& operator *=(double scalar);



private:
 int width;
 int height;
 int kernelWidth;
 int kernelHeight;
 bool separable=false;

 double sigma;
 double effectiveSigma;

 Mode mode=Mode::basic;
 vector<double> data;

 vector<vector<ImageFilterRep>> pyramid;

 vector<Point>points;
 vector<int> indexes;

 vector<Descriptor> descriptors;

 ImageFilterRep& simpleConvolution(ImageFilterRep filter);
 ImageFilterRep& separatableConvolution(ImageFilterRep filter);


};






#endif // IMAGE_H
