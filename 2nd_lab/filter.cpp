#include "filter.h"
#include <cmath>
#include <iostream>

Filter::Filter()
{

}

Image Filter::gauss (Image img, float sigma){
    float ** m = Filter::gaussMatrix(sigma);
    float *** lab = img.LAB;
    std::cout << lab[2][2][0] << std::endl;
    for (int i = 2; i < img.width - 2; i++)
        for (int j = 2; j < img.height - 2; j++){
//            std::cout << i << " " << j << std::endl;
            float sum = 0;
            for (int k = 0; k < 5; k++){
                for (int l = 0; l < 5; l++){

                    sum += (m[k][l] * lab[i + k - 2][j + l - 2][0]);
                }
            }
//            std::cout << sum << "";
            lab[i][j][0] = sum;
        }
    std::cout << "gauss" << std::endl;
    return img;
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
          std::cout<< arr [i][j] << " ";
        }
        std::cout << std::endl;
    }
    return arr;


}
float Filter::gaussDistr(int x, int y, float sigma)
{
    return (1/(2 * 3.1415 * sigma))* exp(-(x*x + y*y)/(2*sigma*sigma));

}
