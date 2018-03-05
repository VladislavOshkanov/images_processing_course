#ifndef ALLOCATOR_H
#define ALLOCATOR_H


class Allocator
{
public:
    Allocator();
    static void del(float *** arr,int  width,int  height);
};

#endif // ALLOCATOR_H
