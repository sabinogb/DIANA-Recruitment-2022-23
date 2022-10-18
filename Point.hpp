/* This class represent a point in a system of 2 coordinates (x and y).
   A "tag" is used to remember where the point's coordinates were in the points.txt file (first point has tag 0, second point has tag 1...). */
   
#include <vector>

class Point
{
	private:
		double x;
		double y;
		int tag;
	
	public:
		Point(double x, double y, int tag) : x(x), y(y), tag(tag) {}
		double getDistanceTo(Point other);
		static Point getByTag(std::vector<Point> points, int tag);
		int getTag();
};
