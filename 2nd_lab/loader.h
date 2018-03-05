#ifndef LOADER_H
#define LOADER_H

#include<QString>
#include<QImage>
#include<QSize>
#include <iostream>
#include "image.h"

class loader
{
public:
    loader();
    static Image load(QString fname);
    static void saveTmpImage(Image img, QString path);
};

#endif // LOADER_H
