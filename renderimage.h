#ifndef RENDERIMAGE_H
#define RENDERIMAGE_H
#include <vector>




using namespace std;

enum Format {PAL, NTVD};

class RenderImage
{
public:
    RenderImage(unsigned char *aData, int aSize);
    vector<double> processRawData(Format format);
    unsigned char* render(vector<double> data);

    int getSize(){return size;}
    unsigned char* getData(){return data;}

    ~RenderImage(){delete[]data;}


private:
    unsigned char *data;
    int size;

    void turnGreyPAL();
    void turnGreyNTVD();
    void linearNormalization(vector<double> &data);
    void linearReNormalization(vector<double> &data);
};

#endif // RENDERIMAGE_H
