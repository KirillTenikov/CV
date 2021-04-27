#ifndef RENDERIMAGE_H
#define RENDERIMAGE_H
#include <vector>




using namespace std;



namespace RenderImage {

   enum Format {PAL, NTVD};

   vector<double> processRawData(unsigned char* data, int size, Format format);
   unsigned char* render(vector<double> data);

};

#endif // RENDERIMAGE_H
