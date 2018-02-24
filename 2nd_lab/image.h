#ifndef IMAGE_H
#define IMAGE_H


class Image
{
public:
    Image (float *** RGB, int width, int height);
    float *** RGB;
    float *** LAB;
    int width;
    int height;
};

#endif // IMAGE_H
