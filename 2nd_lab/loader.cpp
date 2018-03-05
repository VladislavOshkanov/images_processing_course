#include "loader.h"


loader::loader()
{

}
Image loader::load (QString fname)
{
    QImage qimg( fname );
    int i, j;
    float *** array = (float ***)malloc(qimg.width()*sizeof(float**));

            for (i = 0; i< qimg.width(); i++) {

             array[i] = (float **) malloc(qimg.height()*sizeof(float *));

              for (j = 0; j < qimg.height(); j++) {

                  array[i][j] = (float *)malloc(3*sizeof(float));
              }

            }
    if ( false == qimg.isNull() )
    {
        for ( int row = 0; row < qimg.width(); row++ )
            for ( int col = 0; col < qimg.height(); col++ )
            {
                QColor clrCurrent( qimg.pixel( row, col ) );
//                qimg.setPixelColor(row,col, QColor(100,100,100));
                array[row][col][0] = clrCurrent.red() / 255.0;
                array[row][col][1] = clrCurrent.green() / 255.0;
                array[row][col][2] = clrCurrent.blue() / 255.0;
            }
    }



    return Image(array, qimg.width(), qimg.height());
}

void loader::saveTmpImage(Image img, QString path){
    float *** rgb = img.RGB;
    QImage tmp(QSize(img.width, img.height), QImage::Format_RGB888);
    for ( int row = 0; row < img.width; row++ )
        for ( int col = 0; col < img.height; col++ )
        {
            float r = static_cast<int>(rgb[row][col][0] * 255);
            float g = static_cast<int>(rgb[row][col][1] * 255);
            float b = static_cast<int>(rgb[row][col][2] * 255);
//            std::cout << r << " " << g << " " << b << std::endl;
//            std::cout << r << " " << g << " " << b << std::endl;
            tmp.setPixelColor(row,col, QColor(r,g,b));

        }
tmp.save(path);
}
