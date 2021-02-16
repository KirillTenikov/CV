#include "renderimage.h"
#include <algorithm>
#include <iostream>

RenderImage:: RenderImage (unsigned char* aData, int aSize){
        size=aSize;
        data=new unsigned char[size];
        memcpy(data, aData,size);
       }

void RenderImage::turnGreyPAL()
        {
            int result;
            for(int i=0;i<size;i+=4){

             result=0.299*data[i]+0.587*data[i+1]+0.114*data[i+2];
             data[i]=data[i+1]=data[i+2]=result;
            }
        }

void RenderImage::turnGreyNTVD()
        {
            int result;
            for(int i=0;i<size;i+=4){
             result=0.213*data[i]+0.715*data[i+1]+0.072*data[i+2];
             data[i]=data[i+1]=data[i+2]=result;
            }
        }
void RenderImage::linearNormalization(vector<double> &aData)
{
    double max=*max_element(aData.begin(), aData.end());
    double min=*min_element(aData.begin(), aData.end());
    for(vector<double>::iterator it=aData.begin();it!=aData.end(); ++it)
       *it=(*it-min)/(max-min);
}

void RenderImage::linearReNormalization(vector<double> &aData)
{
     double max=*max_element(aData.begin(), aData.end());
     double min=*min_element(aData.begin(), aData.end());
     for(vector<double>::iterator it=aData.begin();it!=aData.end(); ++it)
       *it=(*it-min)*255/(max-min);
}



vector<double>RenderImage::processRawData(Format format)
     {
         vector<double>processedData=vector<double>(size/4);
         switch(format){
          case Format::PAL:  turnGreyPAL();break;
          case Format::NTVD: turnGreyNTVD();break;
         }
         for(int i=0; i<size;i+=4)
            processedData[i/4]=data[i];

         linearNormalization(processedData);
         return processedData;
     }

unsigned char*RenderImage::render(vector<double> aData)
     {

        linearReNormalization(aData);
        for(int i=0,j=0;i<size;i+=4,j++)
        {
           data[i]=data[i+1]=data[i+2]=aData[j];

        }
        return data;
      }


