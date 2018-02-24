#ifndef FILTER_H
#define FILTER_H
#include "image.h"

class Filter
{
public:
    Filter();
    static Image gauss(Image img, float sigma);
    static float ** gaussMatrix(float sigma);
private:

    static float gaussDistr(int x, int y, float sigma);
};


#endif // FILTER_H
