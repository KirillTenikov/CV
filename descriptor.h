#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include <map>
#include <vector>
using namespace std;
typedef vector<map<double, double>> Descriptor;


double calcDistanceDescriptors(Descriptor dsc1,Descriptor dsc2 );

#endif // DESCRIPTOR_H
