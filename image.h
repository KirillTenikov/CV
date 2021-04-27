#ifndef IMAGE_H
#define IMAGE_H
#include<vector>
using namespace std;
enum Mode{basic, continuation, reflection};

class ImageFilterRep
{
public:
    ImageFilterRep (vector<double> aData, int aWidth, int aHeight);

    static ImageFilterRep getGaussKernel(double sigma);
    static ImageFilterRep getGaussX(double sigma);
    static ImageFilterRep getGaussY(double sigma);
    static ImageFilterRep getSobelXDerivative();
    static ImageFilterRep getSobelYDerivative();

    ImageFilterRep getFilterX();
    ImageFilterRep getFilterY();


    double& getPoint(int x, int y);
    double& getPixel(int x, int y);
    double  getPixelValue(int x, int y);
    double L(int x, int y, double sigma);

    vector<double>getData(){return data;}
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



    ImageFilterRep& power(double power);
    ImageFilterRep& sobel();

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

 ImageFilterRep& simpleConvolution(ImageFilterRep filter);
 ImageFilterRep& separatableConvolution(ImageFilterRep filter);

};



#endif // IMAGE_H
