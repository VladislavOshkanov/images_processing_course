#ifndef FILTER_H
#define FILTER_H
#include "image.h"

class Filter
{
public:
    Filter();
    static Image gauss(Image img, float sigma);
    static Image gabour(Image img, float l, float t, float p, float s, float g);
    static Image sobel (Image img);
    static float ** gaussMatrix(float sigma);
    static float ** gabourMatrix(float l, float t, float p, float s, float g);
private:
    static void gabourMul(float *** array, float *** lab, int width, int height, float ** m, int skip);
    static float gaussDistr(int x, int y, float sigma);
    static float gabourFunc(float x,float y, float l, float t, float p, float s, float g);

};


#endif // FILTER_H
