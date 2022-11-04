#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 

#include <stdio.h>

class ray_casting {
public:
	void setPoint(double x, double y, double a);
	double getX();
	double getY();
	void setDepth(int d);
	void setMap(std::vector<std::vector <char>>& m);
	double rayCasting();
private:
	bool checkIntersection(double& rayLen, double& ray_X, double& ray_Y, double firstIntersect_X, double firstIntersect_Y, double addX, double addY, bool isVert);
	double rayX, rayY;

	double rayAngle;
	double PI = 3.14159265;
	double radToDegr = PI / 180.0;
	int mapWidth, mapHeight;
	
	int depth;
	std::vector<std::vector <char>> map;
};