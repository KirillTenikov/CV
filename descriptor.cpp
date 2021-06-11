#include "descriptor.h"
#include <math.h>


double calcDistanceDescriptors(Descriptor dsc1,Descriptor dsc2 )
{
    double superDistance=0;
    vector<double>distance(dsc1.size());
    for (int i=0; i<dsc1.size();i++){
       auto dsc1It=dsc1[i].begin();
       auto dsc2It=dsc2[i].begin();
       distance[i]=0;
       for(int j=0;j<dsc1[i].size();j++){
           distance[i]+=pow(dsc1It->second-dsc2It->second,2);
           ++dsc1It; ++dsc2It;
       }
       superDistance+=distance[i];
    }

    return sqrt(superDistance);
}
