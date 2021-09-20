/*
 * anomaly_detection_util.cpp
 * Author: Yuval Khanimov
 * ID:318970902
 */

#include <cmath>
#include "anomaly_detection_util.h"

float avg(float* x, int size) {
    float sum=0;
    for (int i=0;i<size;i++) {
        sum+=x[i];
    }
    return sum/size;
}

// returns the variance of X and Y
float var(float* x,int size ) {
    float average = avg(x, size);
    float sum=0;
    for (int i=0;i<size;i++) {
        sum+=x[i]*x[i];
    }
    sum=sum/size;
    return sum-average*average;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float aveXY=0;
    for (int i=0;i<size;i++) {
        aveXY+=x[i]*y[i];
    }
    aveXY =aveXY/size;
    return aveXY - (avg(x, size) * avg(y, size));
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float  deviationX=sqrt(var(x,size));
    float  deviationY=sqrt(var(y,size));
    return cov(x,y,size)/(deviationX*deviationY);
}


// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size){
    //creates array of X and Y numbers from given array of points.
    float* xarray = new float [size];
    float* yarray = new float [size];
    for (int i=0;i<size;i++) {
        xarray[i] = points[i]->x;
        yarray[i] = points[i]->y;
    }
    float a = cov(xarray,yarray,size) /var(xarray,size);
    float b = avg(yarray, size) - a * avg(xarray, size);
    delete [] xarray;
    delete [] yarray;
    return Line(a,b);
}
// performs a linear regression and return s the line equation
Line linear_reg(float* x, float* y, int size){
    float a = cov(x,y,size) /var(x,size);
    float b = avg(y, size) - a * avg(x, size);
    return Line(a,b);
}


// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    return dev(p,linear_reg(points,size));
}
// returns the deviation between point p and the line
float dev(Point p,Line l){
    return  std::abs(p.y - l.f(p.x));
}

