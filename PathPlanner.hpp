#include <vector>
#include <cstdlib>
#include <iostream>
#include "Point.hpp"

static void permutationsR(int pos, std::vector<Point> points, int *tags, bool *taken, double *bestDistance, int *bestTags)
{
	//Termination condition
	if(pos >= points.size())
	{
		//Check wether the sequence of points just computed is a newly found shortest path.
		double totalDistance = 0.0;
		
		for(int i = 1; i < points.size(); i++)
		{
			Point current = Point::getByTag(points, tags[i]);
			Point previous = Point::getByTag(points, tags[i-1]);
			totalDistance += current.getDistanceTo(previous);
		}
		
		//The path must look like a loop!
		Point first = Point::getByTag(points, tags[0]);
		Point last = Point::getByTag(points, tags[points.size() - 1]);
		
		totalDistance += first.getDistanceTo(last);
				
		if(totalDistance < (*bestDistance))
		{
			//Yay, we found a shorter path.
			//Update "tags" and "bestDistance".
			for(int i = 0; i < points.size(); i++)
				bestTags[i] = tags[i];
			
			*bestDistance = totalDistance;
		}
		
		return;
	}
	
	for(int i = 0; i < points.size(); i++)
	{
		if(taken[i] == false)
		{
			taken[i] = true;
			tags[pos] = points.at(i).getTag();
			permutationsR(pos + 1, points, tags, taken, bestDistance, bestTags);
			taken[i] = false;
		}
	}
}

void findShortestPath(std::vector<Point> points, double *bestDistance, int *bestTags)
{
	
	int *tags = (int*) malloc(sizeof(int) * points.size());
	bool *taken = (bool*) malloc(sizeof(bool) * points.size());
	
	for(int i = 0; i < points.size(); i++)
		taken[i] = false;
	
	permutationsR(0, points, tags, taken, bestDistance, bestTags);
	
}