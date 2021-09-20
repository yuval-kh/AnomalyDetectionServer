//Yuval Khanimov 318970902 Ilan Bogomolnik ---------

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <assert.h>
#include <vector>
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------


//calculate the distance of two points by the regular formula.
float distTwoPoints(Point x1, Point x2) ;

//finds the minimum circle between two points.
Circle minForTwoPoints(Point p,Point r) ;

//checks if a specific point is in the circle or not.
bool isPointInCircle(Circle c,Point p) ;

//calculate the circle that is defined by 3 points.
Circle circleDefinedBy3Points(Point x ,Point y ,Point z) ;

//calculate the min circle for three points.
Circle minForREqual3(vector<Point> r) ;

//calculate the case of the points vector equals zero.
Circle noPointsLeft(vector<Point> r) ;

//the function for the minimum point. can't use the main function because of the recursion.
Circle minimalCircleHelper(Point** points,size_t size, vector<Point> r) ;

//the main function that calculates the needed minimum circle.
Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
