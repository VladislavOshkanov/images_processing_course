#include "filter.h"
#include "allocator.h"
#include <cmath>
#include <iostream>
#include <thread>
#define GAB_SIZE 5

Filter::Filter()
{

}

Image Filter::gauss (Image img, float sigma){
    float ** m = Filter::gaussMatrix(sigma);
    float *** lab = img.LAB;
    std::cout << lab[2][2][0] << std::endl;
    for (int i = 2; i < img.width - 2; i++)
        for (int j = 2; j < img.height - 2; j++){
            float sum = 0;
            for (int k = 0; k < 5; k++){
                for (int l = 0; l < 5; l++){

                    sum += (m[k][l] * lab[i + k - 2][j + l - 2][0]);
                }
            }
            lab[i][j][0] = sum;
        }
    return img;
}

Image Filter::sobel (Image img) {
    int i, j;
    float *** array = (float ***) malloc(img.width*sizeof(float**));

            for (i = 0; i < img.width; i++) {

             array[i] = (float **) malloc(img.height*sizeof(float *));

              for (j = 0; j < img.height; j++) {

                  array[i][j] = (float *)malloc(3*sizeof(float));
              }

            }
    for (int i = 1; i < img.width - 1; i++)
        for (int j = 1; j < img.height - 1; j++){
            float z1 = img.LAB[i-1][j-1][0];
            float z2 = img.LAB[i-1][j][0];
            float z3 = img.LAB[i-1][j+1][0];
            float z4 = img.LAB[i][j-1][0];
            float z5 = img.LAB[i][j][0];
            float z6 = img.LAB[i][j+1][0];
            float z7 = img.LAB[i+1][j-1][0];
            float z8 = img.LAB[i+1][j][0];
            float z9 = img.LAB[i+1][j+1][0];


            float gx = (z7 + 2*z8 + z9) - (z1 + 2 * z2 + z3);
            float gy = (z3 + 2 * z6 + z9) - (z1 + 2 * z4 + z7);

            if (gx < 0) gx = -gx;
            if (gy < 0) gy = -gy;
            array[i][j][0] = gx + gy;

        }
    img.LAB = array;
    return img;
}
Image Filter::gabour(Image img, float l, float t, float p, float s, float g){
    int i, j;
    float *** array = (float ***) malloc(img.width*sizeof(float**));

            for (i = 0; i < img.width; i++) {

             array[i] = (float **) malloc(img.height*sizeof(float *));

              for (j = 0; j < img.height; j++) {

                  array[i][j] = (float *)malloc(3*sizeof(float));
              }

            }


    float ** m = Filter::gabourMatrix(l, t, p, s, g);
    float *** lab = img.LAB;


    std::thread first (Filter::gabourMul, array, lab, img.width, img.height, m, 0);
    std::thread second (Filter::gabourMul, array, lab, img.width, img.height, m, 1);
    std::thread third (Filter::gabourMul, array, lab, img.width, img.height, m, 2);
    std::thread fourth (Filter::gabourMul, array, lab, img.width, img.height, m, 3);



    first.join();
    second.join();
    third.join();
    fourth.join();
//    for (int i = 2; i < img.width - 2; i++)
//        for (int j = 2; j < img.height - 2; j++){
//            float sum = 0;
//            for (int k = 0; k < GAB_SIZE; k++){
//                for (int l = 0; l < GAB_SIZE; l++){

//                    sum += (m[k][l] * lab[i + k - 2][j + l - 2][0]);
//                }
//            }
//            array[i][j][0] = sum;
//        }
    float *** old = img.LAB;
    Allocator::del(old, img.width, img.height);
    img.LAB = array;
    return img;
}

void Filter::gabourMul(float *** array, float *** lab, int width, int height,float ** m, int skip) {
    for (int i = 2; i < width - 2; i++)
        for (int j = 2; j < height - 2; j++){
//            std::cout << "imhere" << std::endl << skip << std::endl;
            float sum = 0;
            if ((i + skip) % 4 == 0){
                for (int k = 0; k < GAB_SIZE; k++){
                    for (int l = 0; l < GAB_SIZE; l++){
                        sum += (m[k][l] * lab[i + k - 2][j + l - 2][0]);
                    }
                }


              array[i][j][0] = sum;
            }
        }

}

float ** Filter::gabourMatrix(float l, float t, float p, float s, float g){
    float ** arr = new float*[5];
    int i, j;
    for (i = 0; i < GAB_SIZE; ++i)
        arr[i] = new float[5];
    for (i = 0; i < GAB_SIZE; i++)
        for (j = 0; j < GAB_SIZE; j++)
            arr[i][j] = gabourFunc(i -  2, j - 2, l, t, p, s, g);
    float sum = 0;
    for (i = 0; i < GAB_SIZE; i++)
        for (j = 0; j < GAB_SIZE; j++)
            sum += arr[i][j];
    for (i = 0; i < GAB_SIZE; i++){
        for (j = 0; j < GAB_SIZE; j++){
//          arr[i][j] /= sum;

        }
    }
    return arr;

}
float ** Filter::gaussMatrix(float sigma){
    float** arr = new float*[5];
    int i, j;

    for (i = 0; i < 5; ++i)
        arr[i] = new float[5];
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            arr[i][j] = gaussDistr(i - 2, j - 2, sigma);
    float sum = 0;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            sum += arr[i][j];
    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
          arr[i][j] /= sum;
        }
    }
    return arr;


}


float Filter::gabourFunc(float x,float y, float l, float t, float p, float s, float g) {
    float x_s = x * cos (t * 3.1415 / 180) + y * sin (t * 3.1415 / 180);
    float y_s = - x * sin (t * 3.1415 / 180) + y * cos (t * 3.1415 / 180);
    float G = exp((x_s * x_s + y_s * y_s * g * g) / (2 * s * s)) * cos (2 * 3.1415 * x_s / l /*+ p * 3.1415 / 180*/);

    return G;

}
float Filter::gaussDistr(int x, int y, float sigma)
{
    return (1/(2 * 3.1415 * sigma))* exp(-(x*x + y*y)/(2*sigma*sigma));

}
