#include <cmath>
#include <iostream>
#include "Point.hpp"

/* Euclidean distance */
double Point::getDistanceTo(Point other)
{
	double dx = fabs(this->x - other.x);
	double dy = fabs(this->y - other.y);
	return hypot(dx, dy);
}

int Point::getTag()
{
	return this->tag;
}

Point Point::getByTag(std::vector<Point> points, int tag)
{
	for(Point point : points)
	{
		if(point.tag == tag)
			return point;
	}
	
	return Point(0, 0, 0); //this line will never be reached.
}