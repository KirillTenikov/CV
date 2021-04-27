#include "image.h"
#include <algorithm>
#include <functional>
#include <cmath>
#include <numeric>

ImageFilterRep::ImageFilterRep(vector<double> aData, int aWidth, int aHeight)
{
  data=aData;
  width=aWidth;
  height=aHeight;
  kernelHeight=height/2;
  kernelWidth=width/2;

}


ImageFilterRep ImageFilterRep::getGaussKernel(double sigma)
{
    int n=sigma*3+1;
    ImageFilterRep filter=ImageFilterRep(vector<double>((n*2+1)*(n*2+1)),n*2+1,n*2+1);
    double value;
    for(int i=-filter.getKernerlHeight();i<=0;i++)
        for(int j=-filter.getKernerlWidth();j<=0;j++){
            value=exp((-j*j-i*i)/(2*sigma*sigma));
            filter.getPoint(j,i)=value;
            filter.getPoint(-j,-i)=value;
            filter.getPoint(-j,i)=value;
            filter.getPoint(j,-i)=value;
        }

    double sum=accumulate(filter.data.begin(),filter.data.end(),0.0);
    filter*=1/sum;
    filter.separable=true;
    return filter;

}

ImageFilterRep ImageFilterRep::getGaussX(double sigma)
{
    int n=sigma*3+1;
    ImageFilterRep filter=ImageFilterRep(vector<double>(n*2+1),n*2+1,0);

    double value;
     for(int j=-filter.getKernerlWidth();j<=0;j++){
        value=exp((-j*j)/(2*sigma*sigma));
        filter.getPoint(j,0)=value;
        filter.getPoint(-j,0)=value;
       }
    double sum=accumulate(filter.data.begin(),filter.data.end(),0.0);
    filter*=1/sum;
    return filter;
}

ImageFilterRep ImageFilterRep::getGaussY(double sigma)
{
   int n=sigma*3+1;
   ImageFilterRep filter=ImageFilterRep(vector<double>(n*2+1),0,n*2+1);
    double value;
     for(int i=-filter.getKernerlHeight();i<=0;i++){
        value=exp((-i*i)/(2*sigma*sigma));
        filter.getPoint(0,i)=value;
        filter.getPoint(0,-i)=value;
       }
    double sum=accumulate(filter.data.begin(),filter.data.end(),0.0);
    filter*=1/sum;
    return filter;
}

ImageFilterRep ImageFilterRep::getSobelXDerivative()
  {
    vector<double> v={1,0,-1,2,0,-2,1,0,-1};
    return ImageFilterRep(v,3,3)*=0.25;

  }


ImageFilterRep ImageFilterRep::getSobelYDerivative()
  {
    vector<double> v={1,2,1,0,0,0,-1,-2,-1};
    return ImageFilterRep(v,3,3)*=0.25;

  }

ImageFilterRep ImageFilterRep::getFilterX()
{
    int  start=(kernelWidth*2+1)*kernelHeight;
    vector<double> aData=vector<double>(next(data.begin(), start),next(data.begin(),start+kernelWidth*2+1));
    return ImageFilterRep(aData,kernelWidth*2+1,0)*=(1/accumulate(aData.begin(),aData.end(),0.0));
}

ImageFilterRep ImageFilterRep::getFilterY()
{
    vector<double> aData=vector<double>(kernelHeight*2+1);
    for(int i=-kernelHeight;i<=kernelHeight;i++)
        aData[i+kernelHeight]=getPoint(0,i);
    return ImageFilterRep(aData,0,kernelHeight*2+1)*=(1/accumulate(aData.begin(),aData.end(),0.0));
}

double& ImageFilterRep::getPoint(int x, int y)
{
    return data[(y+kernelHeight)*(kernelWidth*2+1)+(x+kernelWidth)];
}


double& ImageFilterRep::getPixel(int x, int y)
{
    return data[y*width+x];
}

double ImageFilterRep::getPixelValue(int x, int y)
{
   if(x<=width-1 && y<=height-1 && x>=0 && y>=0)
     return data[y*width+x];
   switch (mode)
   {
    case Mode::basic: return 0;
    case Mode::continuation: {
       if (x>=0 && x<=width-1){
         if(y<0) return getPixelValue(x,0);
         else return getPixelValue(x,height-1);
        }
        if (y>=0 && y<=height-1){
         if(x<0) return getPixelValue(0,y);
         else return getPixelValue(width-1,y);
         }
         if(y<0 && x<0) return getPixelValue(0,0);
         if(x<0) return getPixelValue(0, height-1);
         if(y<0) return getPixelValue(width-1, 0);
         return getPixelValue(width-1, height-1);

    }
   case Mode::reflection:
   {
       if (x>=0 && x<=width-1){
         if(y<0) return getPixelValue(x,height-1);
         else return getPixelValue(x,0);
        }
        if (y>=0 && y<=height-1){
         if(x<0) return getPixelValue(width-1,y);
         else return getPixelValue(0,y);
         }
         if(y<0 && x<0) return getPixelValue(width-1, height-1);
         if(x<0) return getPixelValue(width-1, 0);
         if(y<0) return getPixelValue(0,height-1);
         return getPixelValue(0, 0);
   }



   }

}


ImageFilterRep& ImageFilterRep::simpleConvolution(ImageFilterRep filter)
{
    ImageFilterRep tempImage=*this;
    double result;

    for(int i=0;i<=height-1;i++)
       for(int j=0;j<=width-1;j++){
        result=0;
        for(int ki=-filter.getKernerlHeight();ki<=filter.getKernerlHeight();ki++)
          for(int kj=-filter.getKernerlWidth();kj<=filter.getKernerlWidth();kj++)
            result+=getPixelValue(j+kj, i+ki)*filter.getPoint(-kj,-ki);
         tempImage.getPixel(j,i)=result;
       }

    *this=tempImage;
    return *this;
}

ImageFilterRep& ImageFilterRep::separatableConvolution(ImageFilterRep filter)
{
    ImageFilterRep filterX=filter.getFilterX();
    ImageFilterRep filterY=filter.getFilterY();
    return simpleConvolution(filterX).simpleConvolution(filterY);
}

ImageFilterRep& ImageFilterRep::operator*=(ImageFilterRep filter)
{


   if(!filter.isSeparable()){
    return simpleConvolution(filter);
   }
   else
   {
      return separatableConvolution(filter);
   }

}


ImageFilterRep& ImageFilterRep::operator *=(double scalar)
{

      transform(data.begin(),data.end(),data.begin(),[=](double x){return x*scalar;});
      return *this;
}





ImageFilterRep& ImageFilterRep::operator+=(ImageFilterRep image)
{
    transform(data.begin(),data.end(),image.getData().begin(),data.begin(), plus<double>());
    return *this;
}

ImageFilterRep& ImageFilterRep::power(double power)
{
    transform(data.begin(),data.end(),data.begin(),[=](double x){return pow(x,power);});
    return *this;
}

ImageFilterRep& ImageFilterRep::sobel()
{
    ImageFilterRep filterX=ImageFilterRep::getSobelXDerivative();
    ImageFilterRep filterY=ImageFilterRep::getSobelYDerivative();
    ImageFilterRep tempImage=*this;
    *this*=filterX;
    tempImage*=filterY;
    (this->power(2)+=tempImage.power(2)).power(0.5);
    return *this;
 }

ImageFilterRep& ImageFilterRep::downSample()
{
    vector<double> newData=vector<double>((getHeight()/2)*(getWidth()/2));
    int k=0;
    for(int i=0;i<getHeight()/2;i++)
       for(int j=0;j<getWidth()/2;j++)
        newData[k++]=getPixel(j*2,i*2);


    *this=ImageFilterRep(newData,getWidth()/2, getHeight()/2);
     return *this;
 }

void ImageFilterRep::buildPyramid(int octaves, int levels, double alphaSigma, double sigma)
{
  setSigma(alphaSigma);
  setEffectiveSigma(alphaSigma);
  double nextSigma=sigma;
  double curSigma=alphaSigma;
  double aEffectiveSigma=sigma;
  ImageFilterRep templFilt=ImageFilterRep::getGaussKernel(sqrt(nextSigma*nextSigma-curSigma*curSigma));
  ImageFilterRep tempImage=*this;
  tempImage*=templFilt;
  pyramid=vector<vector<ImageFilterRep>>(octaves);

  for(int i=0; i<octaves;i++){
    tempImage.setSigma(nextSigma);
    tempImage.setEffectiveSigma(aEffectiveSigma);
    pyramid[i].push_back(tempImage);
    for(int j=0; j<levels;j++){
       aEffectiveSigma*=pow(2,1./levels);
       curSigma=nextSigma;
       nextSigma=curSigma*pow(2,1./levels);
       templFilt=ImageFilterRep::getGaussKernel(sqrt(nextSigma*nextSigma-curSigma*curSigma));
       tempImage.setSigma(nextSigma);
       tempImage.setEffectiveSigma(aEffectiveSigma);
       pyramid[i].push_back(tempImage*=templFilt);
      }
     tempImage.downSample();
     nextSigma=sigma;
    }
}

double ImageFilterRep::L(int x, int y, double aSigma)
{
    if (pyramid.empty())return -1;

    double k=1./(pyramid[0].size()-1);
    int index = round(log(aSigma/effectiveSigma)/log(k));
    if(index<0)index=0;
     else if(index>pyramid[0].size()*pyramid.size()-1)index=pyramid[0].size()*pyramid.size()-1;
    int octave=index/pyramid[0].size();
    int level=index%pyramid[0].size();
    return pyramid[octave][level].getPixelValue(x/pow(2,octave),y/pow(2,octave));
}


