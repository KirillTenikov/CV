#include "renderimage.h"
#include <algorithm>
#include <iostream>

namespace RenderImage  {

namespace  {

void turnGreyPAL(unsigned char *data, int size)
        {
            int result;
            for(int i=0;i<size;i+=4){

             result=0.299*data[i]+0.587*data[i+1]+0.114*data[i+2];
             if(result>255)
                 result=255;
             data[i]=data[i+1]=data[i+2]=result;
            }
        }
void turnGreyNTVD(unsigned char *data, int size)
        {
            int result;
            for(int i=0;i<size;i+=4){
             result=0.213*data[i]+0.715*data[i+1]+0.072*data[i+2];
             data[i]=data[i+1]=data[i+2]=result;
           }



}

void linearNormalization(vector<double> &aData)
{
    double max=*max_element(aData.begin(), aData.end());
    double min=*min_element(aData.begin(), aData.end());
    for(vector<double>::iterator it=aData.begin();it!=aData.end(); ++it)
       *it=(*it-min)/(max-min);
}

void linearReNormalization(vector<double> &aData)
{
     double max=*max_element(aData.begin(), aData.end());
     double min=*min_element(aData.begin(), aData.end());
     for(vector<double>::iterator it=aData.begin();it!=aData.end(); ++it)
      *it=(*it-min)*255/(max-min);
}


}

vector<double> processRawData(unsigned char *data, int size, Format format)
     {
         vector<double>processedData=vector<double>(size/4);
         switch(format){
          case Format::PAL:  turnGreyPAL(data, size);break;
          case Format::NTVD: turnGreyNTVD(data, size);break;
         }
         for(int i=0; i<size;i+=4)
            processedData[i/4]=data[i];

         linearNormalization(processedData);
         return processedData;
     }

unsigned char* render(vector<double> aData)
     {
        int size=aData.size()*4;
        unsigned char *data=new unsigned char[size];
        linearReNormalization(aData);
        for(int i=0,j=0;i<size;i+=4,j++)
        {
           data[i]=data[i+1]=data[i+2]=aData[j];
           data[i+3]=255;

        }
        return data;
      }
unsigned char* renderWithPoints(vector<double> aData, vector<int>points)
     {
        int size=aData.size()*4,k=0;
        unsigned char *data=new unsigned char[size];
        linearReNormalization(aData);
        for(int i=0,j=0;i<size;i+=4,j++)
        {
           if(k<points.size() && points[k]==j){data[i]=data[i+1]=0;data[i+2]=255;k++;}
           else {data[i]=data[i+1]=data[i+2]=aData[j];}
           data[i+3]=255;

        }
        return data;
      }
unsigned char* renderWithPointsDetector(int aSize, vector<int> points)
{
    int size=aSize*4,k=0;
    unsigned char *data=new unsigned char[size];
    for(int i=0,j=0;i<size;i+=4,j++)
    {
       if(k<points.size() && points[k]==j){data[i]=data[i+1]=data[i+2]=255;k++;}
       else {data[i]=data[i+1]=data[i+2]=0;}
       data[i+3]=255;

    }
    return data;

}
unsigned char* renderMaxMin(vector<double> aData)
{
    int size=aData.size()*4,k=0;
    unsigned char *data=new unsigned char[size];
    linearReNormalization(aData);
    for(int i=0,j=0;i<size;i+=4,j++)
    {
        if(aData[j]<=15){data[i]=255;data[i+1]=0;data[i+2]=0;}
         else if(aData[j]<=30){data[i]=255;data[i+1]=51;data[i+2]=51;}
            else if(aData[j]<=45){data[i]=255;data[i+1]=102;data[i+2]=102;}
               else if(aData[j]<=60){data[i]=255;data[i+1]=153;data[i+2]=153;}
                   else if(aData[j]<=75){data[i]=255;data[i+1]=204;data[i+2]=153;}
                      else if(aData[j]<=90){data[i]=255;data[i+1]=255;data[i+2]=153;}
                          else if(aData[j]<=105){data[i]=204;data[i+1]=255;data[i+2]=153;}
                              else if(aData[j]<=120){data[i]=153;data[i+1]=255;data[i+2]=153;}
        else if(aData[j]<=135){data[i]=153;data[i+1]=255;data[i+2]=204;}
         else if(aData[j]<=150){data[i]=153;data[i+1]=255;data[i+2]=255;}
            else if(aData[j]<=165){data[i]=153;data[i+1]=204;data[i+2]=255;}
               else if(aData[j]<=180){data[i]=153;data[i+1]=153;data[i+2]=255;}
                   else if(aData[j]<=195){data[i]=102;data[i+1]=102;data[i+2]=255;}
                      else if(aData[j]<=210){data[i]=51;data[i+1]=51;data[i+2]=255;}
                          else if(aData[j]<=225){data[i]=0;data[i+1]=0;data[i+2]=255;}
                              else if(aData[j]<=255){data[i]=0;data[i+1]=0;data[i+2]=204;}




        data[i+3]=255;

    }
    return data;

}





}




