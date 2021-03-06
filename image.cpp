#include "image.h"
#include "utilities.h"
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
    int n=round(sigma*3+1);
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


ImageFilterRep ImageFilterRep::getGaussWithEvenKernel(int n)
{
    double sigma=(n-2)/6.;
    ImageFilterRep filter=ImageFilterRep(vector<double>((n)*(n)),n,n);
    double value;
    for(int i=-filter.getHeight()/2+1;i<=filter.getHeight()/2;i++)
        for(int j=-filter.getWidth()/2+1;j<=filter.getWidth()/2;j++){
            value=exp((-j*j-i*i)/(2*sigma*sigma));
            filter.getPointEvenKernel(j,i)=value;
        }

    double sum=accumulate(filter.data.begin(),filter.data.end(),0.0);
    filter*=1/sum;
    filter.separable=false;
    return filter;

}



ImageFilterRep ImageFilterRep::getLaplassianKernel(double sigma)
{
    int n=round(sigma*3+1);
    ImageFilterRep filter=ImageFilterRep(vector<double>((n*2+1)*(n*2+1)),n*2+1,n*2+1);
    double value;
    for(int i=-filter.getKernerlHeight();i<=0;i++)
        for(int j=-filter.getKernerlWidth();j<=0;j++){
            value=(j*j+i*i-2*sigma*sigma)*exp((-j*j-i*i)/(2*sigma*sigma));
            filter.getPoint(j,i)=value;
            filter.getPoint(-j,-i)=value;
            filter.getPoint(-j,i)=value;
            filter.getPoint(j,-i)=value;
        }

    double sum=accumulate(filter.data.begin(),filter.data.end(),0.0);
    filter*=1/sum;
    filter.separable=false;
    return filter;

}




ImageFilterRep ImageFilterRep::getGaussX(double sigma)
{
    int n=round(sigma*3+1);
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
   int n=round(sigma*3+1);
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
    return ImageFilterRep(v,3,3)*=0.125;

  }


ImageFilterRep ImageFilterRep::getSobelYDerivative()
  {
    vector<double> v={1,2,1,0,0,0,-1,-2,-1};
    return ImageFilterRep(v,3,3)*=0.125;

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

double& ImageFilterRep::getPointEvenKernel(int x, int y)
{
    return data[(y+kernelHeight-1)*(kernelWidth*2)+(x+kernelWidth-1)];
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

double ImageFilterRep::getPixelValueInterpolation(double x, double y)
{
    double fX0Y0=getPixelValue(floor(x), floor(y));
    double fX1Y0=getPixelValue(ceil(x), floor(y));
    double fX0Y1=getPixelValue(floor(x), ceil(y));
    double fX1Y1=getPixelValue(ceil(x), ceil(y));
    double fXY0, fXY1, result;
    if(ceil(x)!=floor(x)){
      fXY0=(ceil(x)-x)/(ceil(x)-floor(x))*fX0Y0+(x-floor(x))/(ceil(x)-floor(x))*fX1Y0;
      fXY1=(ceil(x)-x)/(ceil(x)-floor(x))*fX0Y1+(x-floor(x))/(ceil(x)-floor(x))*fX1Y1;
    }
    else {
      fXY0=fX0Y0;
      fXY1=fX0Y1;
        }

    if(ceil(y)!=floor(y))result=(ceil(y)-y)/(ceil(y)-floor(y))*fXY0+(y-floor(y))/(ceil(y)-floor(y))*fXY1;
     else result=fXY0;

    return result;
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

ImageFilterRep& ImageFilterRep::sobelAngle()
{
    ImageFilterRep filterX=ImageFilterRep::getSobelXDerivative();
    ImageFilterRep filterY=ImageFilterRep::getSobelYDerivative();
    ImageFilterRep tempImage=*this;
    *this*=filterX;
    tempImage*=filterY;
    transform(data.begin(),data.end(),tempImage.getData().begin(),data.begin(),[=](double x, double y){
        double result=atan2(y,x); return result>0?result:2*M_PI+result;});
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

    int k=pyramid[0].size()-1;
    int index = round(log(aSigma/pyramid[0][0].getEffectiveSigma())/log(2)*k);
    if(index<0)index=0;
     else if(index>pyramid.size()*k+1)index=pyramid.size()*k+1;
    int octave=index/k;
    int level=index%k;
    return pyramid[octave][level].getPixelValue(x/pow(2,octave),y/pow(2,octave));
}

double ImageFilterRep::interpolatedL(double x, double y, double aSigma)
{
    if (pyramid.empty())return -1;

    int k=pyramid[0].size()-1;
    int index =  round(log(aSigma/pyramid[0][0].getEffectiveSigma())/log(2)*k);
    if(index<0)index=0;
     else if(index>pyramid.size()*k+1)index=pyramid.size()*k;
    int octave=index/k;
    int level=index%k;
    return pyramid[octave][level].getPixelValueInterpolation(x/pow(2,octave),y/pow(2,octave));
}

void ImageFilterRep::getMaxPoints(int p, ImageFilterRep &minMax)
{
    points.clear();
    indexes.clear();
    Point tempPoint;
    for (int i=0;i<getHeight();i++)
        for (int j=0;j<getWidth();j++){
            for(int ki=-p;ki<=p;ki++)
                for(int kj=-p;kj<=p;kj++)
                {
                    if(ki==0 && kj==0)continue;
                    if(minMax.getPixelValue(j,i)<minMax.getPixelValue(j+kj,i+ki))goto outer;
                }
            tempPoint.x=j; tempPoint.y=i;
           points.push_back(tempPoint);
           indexes.push_back(i*getWidth()+j);
           outer:continue;
        }

}

void ImageFilterRep::filterThreshold(double t, ImageFilterRep &minMax)
{
    for (int i=0;i<points.size();i++)
        if(minMax.getPixelValue(points[i].x, points[i].y)<t){
            points.erase(points.begin()+i);
            indexes.erase(indexes.begin()+i);
            i--;
        }

}

ImageFilterRep ImageFilterRep::moraveck(int window, int p)
{
    ImageFilterRep operatorResult=*this;
    vector<double> ssd(8);
    double result=0;
    int k;
    for (int i=0;i<operatorResult.getHeight();i++)
        for (int j=0;j<operatorResult.getWidth();j++)
        {
          k=0;      
          for (int dY=-1;dY<=1;dY++)
              for (int dX=-1;dX<=1;dX++){
                  if(dY==0 && dX==0)continue;
                   for(int ki=-window; ki<=window;ki++)
                       for(int kj=-window; kj<=window;kj++)
                           result+=pow(getPixelValue(j+kj,i+ki)-getPixelValue(j+kj+dX, i+ki+dY),2);
                   ssd[k++]=result;
                   result=0;

              }
          operatorResult.getPixel(j,i)=*min_element(ssd.begin(), ssd.end());
        }
    getMaxPoints(p,operatorResult);
    return operatorResult;
}

ImageFilterRep ImageFilterRep::harris(int window,int p)
{
    double a=0,b=0,c=0, trace, det;
    ImageFilterRep operatorResult=*this;
    ImageFilterRep xDer=ImageFilterRep::getSobelXDerivative();
    ImageFilterRep yDer=ImageFilterRep::getSobelYDerivative();
    ImageFilterRep imageXder=operatorResult;
    ImageFilterRep imageYder=operatorResult;
    imageXder*=xDer;imageYder*=yDer;
    for (int i=0;i<operatorResult.getHeight();i++)
        for (int j=0;j<operatorResult.getWidth();j++)
        {
            for (int ki=-window;ki<=window;ki++)
                for (int kj=-window;kj<=window;kj++){
                    a+=imageXder.getPixelValue(j+kj,i+ki)*imageXder.getPixelValue(j+kj,i+ki);
                    c+=imageYder.getPixelValue(j+kj,i+ki)*imageYder.getPixelValue(j+kj,i+ki);
                    b+=imageXder.getPixelValue(j+kj,i+ki)*imageYder.getPixelValue(j+kj,i+ki);
                }
            trace=a+c;
            det=a*c-b*b;
            if(trace>0)operatorResult.getPixel(j,i)=abs((trace-sqrt(trace*trace-4*det))/2);
             else operatorResult.getPixel(j,i)=abs((trace+sqrt(trace*trace-4*det))/2);
             a=b=c=0;

        }
    getMaxPoints(p, operatorResult);
    return operatorResult;

}

void ImageFilterRep::anms(int number){
  int r=2;
  while(points.size()>number){
   for(int i=0; i<points.size();i++)
      for(int j=i+1;j<points.size();j++)
        if (points[j].x<=points[i].x+r && points[j].y<=points[i].y+r &&
              points[j].x>=points[i].x-r && points[j].y>=points[i].y-r){
                points.erase(points.begin()+j);
                indexes.erase(indexes.begin()+j);
                j--;
        }
   r++;
  }

}

void ImageFilterRep::calcDescriptors(int n, int gridX, int gridY, int  pixels)
                                     {
    descriptors.resize(points.size());
    int windowSizeX=gridX*pixels;
    int windowSizeY=gridY*pixels;

    ImageFilterRep gradientAngle=*this;
    ImageFilterRep gradientAmplitude=*this;

    gradientAmplitude.sobel();
    gradientAngle.sobelAngle();

    map<double, double> dsc;
    map<double, double>::iterator up, low;
    int kk;

    double value;
    double angle;
    double coefficient;
    double norm;


    for(double p=M_PI/n;p<2*M_PI;p+=2*M_PI/n )
       dsc.insert(pair<double, double>(p,0));

    for(int k=0; k<descriptors.size();k++)
    {
        kk=0;
        descriptors[k].resize(gridX*gridY);

        for(int i=windowSizeY%2?-windowSizeY/2:-windowSizeY/2+1;i<=windowSizeY/2;i+=pixels)
            for(int j=windowSizeX%2?-windowSizeX/2:-windowSizeX/2+1;j<=windowSizeX/2;j+=pixels){
                descriptors[k][kk]=dsc;
                for(int ki=0;ki<pixels;ki++)
                    for(int kj=0;kj<pixels;kj++)
                    {
                       angle=gradientAngle.getPixelValue(points[k].x+j+kj,points[k].y+i+ki);
                       value=gradientAmplitude.getPixelValue(points[k].x+j+kj,points[k].y+i+ki);
                       up=descriptors[k][kk].lower_bound(angle);
                       if(up==descriptors[k][kk].begin()){
                          coefficient=(up->first-angle)/(up->first+M_PI/n);
                          low=prev(descriptors[k][kk].end());
                       }
                       else {
                         low=prev(up);
                         if(up==descriptors[k][kk].end()){
                            coefficient=(2*M_PI+M_PI/n-angle)/(2*M_PI+M_PI/n-low->first);
                            up=descriptors[k][kk].begin();
                         }
                         else
                           coefficient=(up->first-angle)/(up->first-low->first);
                       }
                           low->second+=value*coefficient;
                           up->second+=value*(1-coefficient);



                    }
                norm=accumulate(descriptors[k][kk].begin(),descriptors[k][kk].end(), 0.0, [=](double value, pair<double, double> p)
                                  {return value+pow(p.second,2);});
                for (auto& [key, value] : descriptors[k][kk])value/=sqrt(norm);


                kk++;
            }
    }

}

void ImageFilterRep::calcDescriptorsRI(int n0, int n1, double sigma)
{
    const int gridX=4;
    const int gridY=4;
    const int pixels=4*sigma;

    descriptors.resize(points.size());
    int windowSizeX=gridX*pixels;
    int windowSizeY=gridY*pixels;

    ImageFilterRep gaussianForOrientation=ImageFilterRep::getGaussKernel(sigma*2.3);
    ImageFilterRep gaussianDsc=ImageFilterRep::getGaussKernel(2.3*sigma);

    buildPyramid(2,2,0,1);

    map<double, double> dsc0;
    map<double, double> dsc1;
    map<double, double>::iterator up, low;


    double x;
    double y;
    int kk;


    double value;
    double angle;
    double coefficient;
    double norm;

    double coefficientX, coefficientY;



    int diagramX;
    int diagramY;

    bool reverseDirectionY;
    bool reverseDirectionX;


    for(double p=M_PI/n1;p<2*M_PI;p+=2*M_PI/n1 )
       dsc1.insert(pair<double, double>(p,0));

    for(int k=0; k<descriptors.size();k++)
    {
        diagramX=0;
        diagramY=0;
        descriptors[k].resize(gridX*gridY);

        for(int i=0; i<descriptors[k].size();i++)
             descriptors[k][i]=dsc1;

        for(double p=M_PI/n0;p<2*M_PI;p+=2*M_PI/n0 )
           dsc0.insert(pair<double, double>(p,0));


        for(int i=-gaussianForOrientation.getKernerlHeight();i<=gaussianForOrientation.getKernerlHeight();i++)
           for(int j=-gaussianForOrientation.getKernerlWidth();j<=gaussianForOrientation.getKernerlWidth();j++)
           {
               value=gaussianForOrientation.getPoint(j,i)*sqrt(pow(L(points[k].x+1+j,points[k].y+i, sigma)-L(points[k].x-1+j,points[k].y+i,sigma),2)+
                                   pow(L(points[k].x+j,points[k].y+1+i, sigma)-L(points[k].x+j,points[k].y-1+i, sigma),2));
               angle=atan2(L(points[k].x+j,points[k].y+1+i,sigma)-L(points[k].x+j,points[k].y-1+i, sigma),
                                 L(points[k].x+1+j,points[k].y+i, sigma)-L(points[k].x-1+j,points[k].y+i, sigma));
               angle=angle>=0?angle:2*M_PI+angle;


               up=dsc0.lower_bound(angle);
               if(up==dsc0.begin()){
                  coefficient=(up->first-angle)/(up->first+M_PI/n0);
                  low=prev(dsc0.end());
               }
               else {
                 low=prev(up);
                 if(up==dsc0.end()){
                    coefficient=(2*M_PI+M_PI/n0-angle)/(2*M_PI+M_PI/n0-low->first);
                    up=dsc0.begin();
                 }
                 else
                   coefficient=(up->first-angle)/(up->first-low->first);
               }
                   low->second+=value*coefficient;
                   up->second+=value*(1-coefficient);
           }



         vector<double> alpha;
         auto it=max_element(dsc0.begin(),dsc0.end(),[](const pair<double, double> &a,const pair<double, double> &b)
                               {return a.second<b.second;});

         double tempValue=it->second;
         alpha.push_back(it->first);
         dsc0.erase(it);
         it=max_element(dsc0.begin(),dsc0.end(),[](const pair<double, double> &a,const pair<double, double> &b)
                                                                            {return a.second<b.second;});
         if(it->second>=tempValue*0.8)
         {
             alpha.push_back(it->first);
             points.insert(points.begin()+k+1, points[k]);
             indexes.insert(indexes.begin()+k+1, indexes[k]);
             descriptors.insert(descriptors.begin()+k+1, descriptors[k]);
         }
         dsc0.clear();

         for(int indexAngle=0; indexAngle<alpha.size(); indexAngle++){
         for(int i=-windowSizeY/2;i<=windowSizeY/2;i+=pixels){
            if(i==0)i++;         
            for(int j=-windowSizeX/2;j<=windowSizeX/2;j+=pixels){
                if(j==0)j++;
                 for(int ki=0;ki<pixels;ki++)
                    for(int kj=0;kj<pixels;kj++)
                    {

                       x=(j+kj)*cos(alpha[indexAngle])+(i+ki)*sin(alpha[indexAngle]);
                       y=(i+ki)*cos(alpha[indexAngle])-(j+kj)*sin(alpha[indexAngle]);
                       calcGridCoordinates(x,y, &diagramX, &diagramY, &coefficientX, &coefficientY);
                       kk=diagramY*gridX+diagramX;



                       angle=atan2(L(points[k].x+j+kj,points[k].y+i+ki+1,sigma)-L(points[k].x+j+kj,points[k].y+i+ki-1, sigma),
                                   L(points[k].x+j+kj+1,points[k].y+i+ki, sigma)-L(points[k].x+j+kj-1,points[k].y+i+ki, sigma));
                       value=gaussianDsc.getPoint(j+kj,i+ki)*sqrt(pow(L(points[k].x+j+kj+1,points[k].y+i+ki,sigma)-L(points[k].x+j+kj-1,points[k].y+i+ki,sigma),2)+
                                  pow(L(points[k].x+j+kj,points[k].y+i+ki+1,sigma)-L(points[k].x+j+kj,points[k].y+i+ki-1,sigma),2));
                       angle=angle>=0?angle:2*M_PI+angle;
                       angle-=alpha[indexAngle];
                       angle=angle>=0?angle:2*M_PI+angle;

                       up=descriptors[k][kk].lower_bound(angle);
                       if(up==descriptors[k][kk].begin()){
                          coefficient=(up->first-angle)/(up->first+M_PI/n1);
                       }
                       else {
                         low=prev(up);
                         if(up==descriptors[k][kk].end()){
                            coefficient=(2*M_PI+M_PI/n1-angle)/(2*M_PI+M_PI/n1-low->first);
                            up=descriptors[k][kk].begin();
                         }
                         else
                           coefficient=(up->first-angle)/(up->first-low->first);
                       }
                         if(coefficientX!=0)
                             if(coefficientY!=0)
                              {descriptors[k][(diagramY+1)*gridX+diagramX+1][low->first]+=value*coefficient*coefficientY*coefficientX;
                               descriptors[k][(diagramY+1)*gridX+diagramX+1][up->first]+=value*(1-coefficient)*coefficientY*coefficientX;
                               descriptors[k][(diagramY+1)*gridX+diagramX][low->first]+=value*coefficient*coefficientY*(1-coefficientX);
                               descriptors[k][(diagramY+1)*gridX+diagramX][up->first]+=value*(1-coefficient)*coefficientY*(1-coefficientX);
                               descriptors[k][diagramY*gridX+diagramX+1][low->first]+=value*coefficient*(1-coefficientY)*coefficientX;
                               descriptors[k][diagramY*gridX+diagramX+1][up->first]+=value*(1-coefficient)*(1-coefficientY)*coefficientX;
                               low->second+=value*coefficient*(1-coefficientY)*(1-coefficientX);
                               up->second+=value*(1-coefficient)*(1-coefficientY)*(1-coefficientX);
                             }
                            else
                             {
                                 descriptors[k][diagramY*gridX+diagramX+1][low->first]+=value*coefficient*(1-coefficientY)*coefficientX;
                                 descriptors[k][diagramY*gridX+diagramX+1][up->first]+=value*(1-coefficient)*(1-coefficientY)*coefficientX;
                                 low->second+=value*coefficient*(1-coefficientY)*(1-coefficientX);
                                 up->second+=value*(1-coefficient)*(1-coefficientY)*(1-coefficientX);
                             }
                         else
                          if(coefficientY!=0)
                          {
                              descriptors[k][(diagramY+1)*gridX+diagramX][low->first]+=value*coefficient*coefficientY*(1-coefficientX);
                              descriptors[k][(diagramY+1)*gridX+diagramX][up->first]+=value*(1-coefficient)*coefficientY*(1-coefficientX);
                              low->second+=value*coefficient*(1-coefficientY)*(1-coefficientX);
                              up->second+=value*(1-coefficient)*(1-coefficientY)*(1-coefficientX);
                          }
                          else
                          {
                              low->second+=value*coefficient*(1-coefficientY)*(1-coefficientX);
                              up->second+=value*(1-coefficient)*(1-coefficientY)*(1-coefficientX);

                          }



                       }
             }


         }

         /*    double superNorm=0;
        for(int i=0; i<descriptors[k].size();i++){
         norm=accumulate(descriptors[k][i].begin(),descriptors[k][i].end(), 0.0, [=](double value, pair<double, double> p)
                   {return value+pow(p.second,2);});
         superNorm+=norm;
        }
        if(superNorm!=0)
            low=prev(descriptors[k][kk].end());
         for(int i=0; i<descriptors[k].size();i++)
          for (auto& [key, value] : descriptors[k][0])value/=sqrt(superNorm);*/
        if(alpha.size()>1) k+=(1-indexAngle);
    }

    }
}

vector<pair<Point, Point>> ImageFilterRep::findClosePoints(ImageFilterRep &img2)
{
  vector<pair<Point, Point>>result(points.size());
  vector<double> distance(img2.getPoints().size());
  if(points.size()>img2.getPoints().size())
   {
      result.resize(img2.getPoints().size());
      distance.resize(this->points.size());
      for(int i=0; i<result.size(); i++)
      {
          for(int j=0; j<points.size();j++)
           distance[j]=calcDistanceDescriptors(descriptors[j], img2.getDescriptors()[i]);

         result[i].first=points[min_element(distance.begin(), distance.end())-distance.begin()];
         result[i].second=img2.points[i];

      }

   }
  else
      for(int i=0; i<result.size(); i++)
      {
          for(int j=0; j<img2.points.size();j++)
           distance[j]=calcDistanceDescriptors(descriptors[i], img2.getDescriptors()[j]);

         result[i].first=points[i];
         result[i].second=img2.points[min_element(distance.begin(), distance.end())-distance.begin()];
      }
  return result;
}


