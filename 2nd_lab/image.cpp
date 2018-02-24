#include "image.h"

Image::Image(float *** RGB, int width, int height)
{
    this->RGB = RGB;
    this->LAB = 0;
    this->width = width;
    this->height = height;
}
