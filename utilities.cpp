#include "utilities.h"
void calcGridCoordinates(int x, int y, int* gridX, int *gridY, double *coefficientX, double *coefficientY)
{
    if(x<=-5){*gridX=0;*coefficientX=0;}
     else if(x>-5 && x<-4){*gridX=0;*coefficientX=x+5;}
      else if(x>=-4 && x<=-1){*gridX=1;*coefficientX=0;}
       else if(x>-1 && x<1){*gridX=1;*coefficientX=(x+1)/2;}
        else if(x>=1 && x<=4){*gridX=2;*coefficientX=0;}
         else if(x>4 && x<5){*gridX=2;*coefficientX=x-4;}
          else if(x>=5){*gridX=3;*coefficientX=0;}

    if(y<=-5){*gridY=0;*coefficientY=0;}
     else if(y>-5 && y<-4){*gridY=0;*coefficientY=y+5;}
      else if(y>=-4 && y<=-1){*gridY=1;*coefficientY=0;}
       else if(y>-1 && y<1){*gridY=1;*coefficientY=(y+1)/2;}
        else if(y>=1 && y<=4){*gridY=2;*coefficientY=0;}
         else if(y>4 && y<5){*gridY=2;*coefficientY=y-4;}
          else if(y>=5){*gridY=3;*coefficientY=0;}

}
