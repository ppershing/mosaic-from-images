// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "ColorDistance.h"
#include <cmath>

double ColorDistance::distance(float r1, float g1, float b1,
             float r2, float g2, float b2){
    double rmean = (r1 + r2) / 2;
    double r = r1 - r2;
    double g = g1 - g2;
    double b = b1 - b2;

    return sqrt((2+rmean)*r*r + 4*g*g + (3-rmean)*b*b);
}
