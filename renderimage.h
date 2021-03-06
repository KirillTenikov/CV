#ifndef RENDERIMAGE_H
#define RENDERIMAGE_H
#include <vector>




using namespace std;



namespace RenderImage {

   enum Format {PAL, NTVD};

   vector<double> processRawData(unsigned char* data, int size, Format format);
   unsigned char* render(vector<double> data);
   unsigned char* renderTwoImages(vector<double> data1, int width, int height1,vector<double> data2, int width2, int height2);
   unsigned char* renderWithPointsDetector(int size, vector<int>points);
   unsigned char* renderWithPoints(vector<double> data, vector<int>points);
   unsigned char* renderMaxMin(vector<double> aData);



};

#endif // RENDERIMAGE_H
