#include <iostream> 
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 
#include "ray_casting.h"
#include <stdio.h>
#include "Sprites_1.h"


void ray_casting::setPoint(double x, double y, double a) {
	rayX = x; rayY = y; rayAngle = a;
}

double ray_casting::getX() {
	return rayX;
}

double ray_casting::getY() {
	return rayY;
}

void ray_casting::setDepth(int d) {
	depth = d;
}

void ray_casting::setMap(std::vector<std::vector <char>>& m) {
	map = m;
	mapWidth = map[0].size();
	mapHeight = map.size();
}


double ray_casting::rayCasting_floor(double x, double y, double z, double angle, double angle_z) {
	x *= 64; y *= 64; z *= 64;
	double step = 0.5;
	double ray_len_floor = -1;
	
	//ray_len_floor = sqrt(pow(sqrt(pow(x, 2) + pow(y, 2)),2) + pow(32, 2));
	return ray_len_floor;
}

double ray_casting::rayCasting() {
	//checking vertical intersection
	double firstIntersectV_Y, firstIntersectV_X = ((int)this->rayX) * 64.0;
	//V - vertical
	double rayX_V = this->rayX * 64.0, rayY_V = this->rayY * 64.0;
	double addX_V = 64.0, addY_V = 64.0 / tan(this->rayAngle * radToDegr);
	//finding the intersection coordinate X with the vertical
	//if the ray is directed to the right
	if (this->rayAngle < 180.0 && this->rayAngle > 0.0) {
		firstIntersectV_X += 64.0;
	}
	//if the ray is directed to the left
	else {
		firstIntersectV_X = ((int)firstIntersectV_X / 64) * 64;
		addX_V = -64.0;
		addY_V *= -1;
	}
	//finding the intersection coordinate Y with the vertical 
	firstIntersectV_Y = rayY_V + (firstIntersectV_X - rayX_V) / tan(this->rayAngle * radToDegr);
	//finding the length of the ray going up to the first vertical intersection
	double rayLenV;
	//checking if the ray touches the wall vertically
	bool hitWallV = checkIntersection(rayLenV, rayX_V, rayY_V, firstIntersectV_X, firstIntersectV_Y, addX_V, addY_V, true);


	//checking horizontal intersection
	//H - hotizontal
	double rayX_H = this->rayX * 64.0, rayY_H = this->rayY * 64.0, rayLenH;
	double firstIntersectH_Y = ((int)this->rayY) * 64.0, firstIntersectH_X = rayX_H;
	double addX_H, addY_H = 64.0;
	//if the ray is directed to the down
	if (this->rayAngle <= 90.0 || this->rayAngle >= 270) {
		//finding the intersection coordinate with the horizontal
		firstIntersectH_Y += 64.0;
		//finding the intersection coordinate with the vertical
		firstIntersectH_X += (firstIntersectH_Y - rayY_H) * tan((this->rayAngle - 180.0) * radToDegr);
		addX_H = 64.0 * tan((this->rayAngle - 180.0) * radToDegr);
	}
	//if the ray is directed to the up
	else {
		firstIntersectH_Y = ((int)(firstIntersectH_Y) / 64) * 64;
		firstIntersectH_X += (firstIntersectH_Y - rayY_H) * tan(this->rayAngle * radToDegr);
		addX_H = -64.0 * tan(this->rayAngle * radToDegr);
		addY_H *= -1.0;
	}
	//finding the length of the ray going up to the first horizontal intersection
	//checking if the ray touches the wall horizontally

	bool hitWallH = checkIntersection(rayLenH, rayX_H, rayY_H, firstIntersectH_X, firstIntersectH_Y, addX_H, addY_H, false);

	if (hitWallH && hitWallV) {
		if (rayLenH < rayLenV) {
			rayX = rayX_H, rayY = rayY_H; 
			isVert = false;
		}
		else {
			rayX = rayX_V,rayY = rayY_V;
			isVert = true;
		}
		
		return std::min(rayLenH, rayLenV);
	}
	else if (hitWallH) {
		rayX = rayX_H,rayY = rayY_H;
		isVert = false;
		
		return rayLenH; 
	}
	else if (hitWallV) {
		rayX = rayX_V,rayY = rayY_V;
		isVert = true;
		
		return rayLenV;
	}
	else {
		return -1.0;
	}
}




bool ray_casting::checkIntersection(double& rayLen, double& ray_X, double& ray_Y, double firstIntersect_X, double firstIntersect_Y, double addX, double addY, bool isVert) {
	//coordinate ray
	ray_X = firstIntersect_X, ray_Y = firstIntersect_Y;
	double predX = 0, predY = 0;
	if (this->rayAngle > 90.0 && this->rayAngle < 270 && !isVert) {
		predY = 64.0;
		if (ray_Y < 64.0) ray_Y = 64.0;
	}
	else if (this->rayAngle > 180.0 && this->rayAngle < 360.0 && isVert) {
		predX = 64.0;
		if (ray_X < 64.0) ray_X = 64.0;
	}

	double rayLenCH = sqrt(pow(ray_Y - this->rayY * 64, 2) + pow(ray_X - this->rayX * 64, 2));
	double addRayLen = sqrt(pow(addX, 2) + pow(addY, 2));
	if (rayLenCH > depth * 64.0 || rayLenCH <= 0.0 || ray_X / 64.0 > mapWidth || ray_Y / 64.0 > mapHeight || ray_X < 0 || ray_Y < 0)
		return false;
	
	while (rayLenCH >= 0.0 && rayLenCH <= depth * 64.0 && ray_X >= predX && ray_X < mapHeight * 64.0 && ray_Y >= predY && ray_Y < mapWidth * 64.0 ) 
	{
		if (map[(int)(ray_Y - predY) / 64][(int)(ray_X - predX) / 64] == '#') {
			rayLen = sqrt(pow(ray_Y - this->rayY * 64, 2) + pow(ray_X - this->rayX * 64, 2));
			return true;
		}
		ray_X += addX;
		ray_Y += addY;
		rayLenCH += addRayLen;
	}
	return false;
}

