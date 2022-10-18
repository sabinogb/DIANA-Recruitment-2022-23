#include <cstdio>
#include <iostream>
#include <string>
#include <cfloat>
#include "PathPlanner.hpp"

int main(int argc, char **argv)
{
	FILE *fp;
	int count;
	std::string line;
	std::string x, y;
	char temp[20];
	std::vector<Point> points;
	
	/* Open the file. */
	fp = fopen("points.txt", "r");
	if(fp == NULL)
	{
		std::cout << "Could not open points.txt" << std::endl;
		return 0;
	}

	/* Read the number of the points. */
	fscanf(fp, "%d\n", &count);
	
	/* Read line by line... */
	int commaPosition;
	
	for(int i = 0; i < count; i++)
	{
		fgets(temp, 20, fp);
		
		/*... split the line in the x coordinate and y coordinate.*/
		line = temp;
		commaPosition = line.find(',');
		x = line.substr(0, commaPosition);
		y = line.substr(commaPosition + 1, line.length() - commaPosition);
		
		//... and initialize a new Point for each pair of coordinate.
		points.push_back(Point(std::stof(x), std::stof(y), i));
	}
	
	fclose(fp);
	
	/* Compute the shortest path. */
	int *tags = (int*) malloc(sizeof(int) * points.size());
	double totalDistance = DBL_MAX;
	
	findShortestPath(points, &totalDistance, tags);
	
	//Print result.
	std::cout << "[ ";
	for(int i = 0; i < points.size(); i++)
	{
		std::cout << tags[i]  << " ";
	}
	std::cout << "] ";
	std::cout << totalDistance << " meters" << std::endl;
	
	free(tags);
	return 0;
}