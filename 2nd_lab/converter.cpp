#include "converter.h"
#include <iostream>
#include <cmath>

Converter::Converter()
{

}
Image Converter::LABtoRGB (Image img)
{
    float *** lab = img.LAB;
    float *** rgb = img.RGB;
    for (int i = 0; i < img.width; i++){
        for (int j = 0; j < img.height; j++){
            float L = lab[i][j][0];
            float A = lab[i][j][1];
            float B = lab[i][j][2];
            float x = pow((L + 16)/116 + A/500, 3);
            float y = pow((L + 16)/116, 3);
            float z = pow ((L + 16) / 116 - B / 200, 3);
            float r = (2.370 * x - 0.9 * y - 0.471 * z);
            float g = (-0.513 * x + 1.425 * y + 0.088 * z);
            float b = (0.005 * x - 0.014 * y + 1.009 * z);
            rgb[i][j][0] = r;
            rgb[i][j][1] = g;
            rgb[i][j][2] = b;
        }
    }
    return img;
}

void Converter::normalizeLAB(Image img){
   float maxL = 0;
   float minL = 100;
   float minA = 0;
   float maxA = 0;
   float minB = 0;
   float maxB = 0;
   for (int i = 0; i < img.height; i++){
       for (int j = 0; j < img.width; j++){
           if (img.LAB[i][j][0] < minL) minL = img.LAB[i][j][0];
           if (img.LAB[i][j][0] > maxL) maxL = img.LAB[i][j][0];
           if (img.LAB[i][j][1] < minA) minA = img.LAB[i][j][1];
           if (img.LAB[i][j][1] > maxA) maxA = img.LAB[i][j][1];
           if (img.LAB[i][j][2] < minB) minB = img.LAB[i][j][2];
           if (img.LAB[i][j][2] > maxB) maxB = img.LAB[i][j][2];
       }
   }
   std::cout << "min" << minL << "max " << maxL;
   std::cout << "min" << minA << "max " << maxA;
   std::cout << "min" << minB << "max " << maxB;
}

void Converter::normalizeRGB(Image img){
   float max = 0;
   for (int i = 0; i < img.width; i++){
       for (int j = 0; j < img.height; j++){
//           if (img.RGB[i][j][0] < minL) minL = img.LAB[i][j][0];
           if (img.RGB[i][j][0] > max) max = img.RGB[i][j][0];
//           if (img.RGB[i][j][1] < minA) minA = img.RGB[i][j][1];
           if (img.RGB[i][j][1] > max) max = img.RGB[i][j][1];
//           if (img.RGB[i][j][2] < minB) minB = img.RGB[i][j][2];
           if (img.RGB[i][j][2] > max) max = img.RGB[i][j][2];
       }
   }
   if (max > 1){
       for (int i = 0; i < img.width; i++){
           for (int j = 0; j < img.height; j++){
            img.RGB[i][j][0] /= (max);
            img.RGB[i][j][1] /= (max);
            img.RGB[i][j][2] /= (max);


           }
       }
   }

//   std::cout << "min" << minL << "max " << maxL;
//   std::cout << "min" << minA << "max " << maxA;
//   std::cout << "min" << minB << "max " << maxB;
}
Image Converter::RGBtoLAB(Image img)
{
    float *** rgb = img.RGB;

    int i, j;
    float *** array = (float ***) malloc(img.width*sizeof(float**));

            for (i = 0; i < img.width; i++) {

             array[i] = (float **) malloc(img.height*sizeof(float *));

              for (j = 0; j < img.height; j++) {

                  array[i][j] = (float *)malloc(3*sizeof(float));
              }

            }

     for (int i = 0; i < img.width; i++)
          for (int j = 0; j < img.height; j++){
              float r =  rgb[i][j][0];
              float g =  rgb[i][j][1];
              float b =  rgb[i][j][2];
              float fx = pow((0.488 * r + 0.310 * g + 0.200 * b), 0.333);
              float fy = pow((0.176 * r + 0.812 * g + 0.010 * b), 0.333);
              float fz = pow((0.01 * g + 0.989 * b), 0.333);
              float L = 116 * fx - 16;
              float A = 500 * (fx - fy);
              float B = 200 * (fy - fz);
              array[i][j][0] = L;
              array[i][j][1] = A;
              array[i][j][2] = B;

          }

std::cout << "done" << std::endl;
           img.LAB = array;
    return img;
}


