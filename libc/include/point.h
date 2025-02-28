#ifndef POINT_H
#define POINT_H

#include "bignum.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT Point {
public:
    int x;
    int y;
public:
    explicit Point();
    explicit Point(int x, int y);
    ~Point();
    bool operator == (Point p){
        return x == p.x && y == p.y;
    }
};

#endif // POINT_H
