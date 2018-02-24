#ifndef CONVERTER_H
#define CONVERTER_H
#include "image.h"

class Converter
{
public:
    Converter();
    static Image RGBtoLAB(Image img);
    static Image LABtoRGB (Image img);
    static void normalizeLAB(Image img);
    static void normalizeRGB(Image img);
};

#endif // CONVERTER_H
