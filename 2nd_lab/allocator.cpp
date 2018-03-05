#include "allocator.h"

Allocator::Allocator()
{

}
void Allocator::del(float *** arr, int width, int height){
    int i,j;
    for (i = 0; i < width; i++) {

      for (j = 0; j < height; j++) {

          delete arr[i][j];
      }
      delete arr[i];
    }
    delete arr;
}
