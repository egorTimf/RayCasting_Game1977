#include <SFML/Graphics.hpp>

#include "ray_casting.h"
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 

#include <stdio.h>
#include <Windows.h>

int screenWidth = 1024;			// Monitor Screen Size X (columns)
int screenHeight = 768;			// Monitor Screen Size Y (rows)


double PI = 3.14159265;
double radToDegr = PI / 180.0;
double distProjectPlane =(int)( screenWidth / 2.0 / tan(30.0*radToDegr));
double playerX = 1;			// Player Start Position
double playerY = 1;
double playerA = 0;			// Player Start Rotation



double depth = 16.0;			// Maximum rendering distance
double speed = 15.0;
double angleBetweenRays = 60.0/screenWidth; 

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

std::vector<std::vector <char>> map = { 
		{'#','#','#','#','#','#','#','.','.','#','#','#','#','#','#','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','#','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','#','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','#','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','#','#','#','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','#','#','#','.','.','.','.','.','.','#'},
		{'#','#','.','.','.','#','.','#','.','.','.','.','.','.','.','#'},
		{'#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','#','.','.','.','.','.','.','.','.','#','.','.','#'},
		{'#','#','.','.','#','.','.','.','.','#','.','.','#','.','.','#'},
		{'#','.','#','.','.','#','.','.','.','.','.','.','#','.','.','#'},
		{'#','.','.','#','.','.','.','.','.','.','.','.','#','.','.','#'},
		{'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting");//create window
sf::Sprite wall;


void drawWall(double rayLen, double angle, int i);
void playerDisplacement();
int duckDown();
void drawFloor();
void darkeningtheWalls(int& r, int& g, int& b, int num, double lenRay);

int main()
{
	sf::Texture texture_Wall;
	texture_Wall.loadFromFile("textures/BrickWall_texture.png");
	
	
	wall.setTexture(texture_Wall);
	
	ray_casting rc;
	rc.setMap(map);
	rc.setDepth(depth);

    while (window.isOpen())
    {
		playerDisplacement();
		window.clear();
		
		std::vector<std::vector<double>> pointWall;
		for (int i = 0; i < screenWidth; i++) {
			double rayA = std::fmod(playerA - 30.0 + (i * angleBetweenRays), 360.0), rayX = playerX, rayY = playerY;
			if (rayA < 0.0) rayA += 360.0;
			rc.setPoint(rayX, rayY, rayA);
			double rayLen = rc.rayCasting();
			if (rayLen == -1) {
				continue;
			}

			rayX = (int)rc.getX() , rayY = (int)rc.getY();
			
			drawWall(rayLen, rayA, i);
				
			//rc.getPointWall(pointWall);
			//std::cout << pointWall[0][0];
			
			
			//drawFloor();
			//std::cout << rayLen << std::endl;
		}
		
		
		
		//window.draw(wall);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		
        window.display();
    }
	
}

void drawFloor() {
	sf::ConvexShape convex;

	// изменяем размер на 5 вершин
	convex.setPointCount(4);

	// определяем вершины
	convex.setPoint(0, sf::Vector2f(0, screenHeight));
	convex.setPoint(1, sf::Vector2f(0, screenHeight/2));
	convex.setPoint(3, sf::Vector2f(screenWidth, screenHeight/2));
	convex.setPoint(2, sf::Vector2f(screenWidth, screenHeight));
	window.draw(convex);

}
void drawWall(double rayLen, double angle, int i){
	double ProjSliceHeight = 64.0 / (rayLen*cos((angle - playerA) * radToDegr)) * distProjectPlane;
	if (ProjSliceHeight >= screenHeight) 
		ProjSliceHeight = screenHeight;

	int r = 176, g = 176, b = 176;
	
	ProjSliceHeight = (int)ProjSliceHeight / 2;
	
	if (rayLen <= 0.0) return;
	sf::VertexArray lines(sf::Lines, 2);
	int stR = (int)(screenHeight / 2 - ProjSliceHeight);
	int stR2 = (int)(screenHeight / 2 + ProjSliceHeight );
	lines[0].position = sf::Vector2f((int)i, stR);
	lines[1].position = sf::Vector2f((int)i, stR2);

	
	darkeningtheWalls(r, g, b, 16, rayLen);
	
	lines[0].color = sf::Color(r, g, b);
	lines[1].color = sf::Color(r, g, b);
	
	window.draw(lines);
}

void darkeningtheWalls(int& r, int& g, int& b, int num, double lenRay) {
	int stepR = r / num;
	int ansR = 0;
	int stepLen = 0;
	while ((depth / num) * 64 * stepLen < lenRay) {
		stepLen++;
	}
	
	r = r / num *(num-stepLen+1);
	
	g = g / num * (num - stepLen);
	b = b / num * (num - stepLen);
}

int duckDown() {
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
		return screenHeight/4;
	}
	return 0;
}
void playerDisplacement() {
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<double> tlapsedTime = tp2 - tp1;
	tp1 = tp2;
	double elapsedTime = tlapsedTime.count();

	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		playerA -= (speed * 2) * elapsedTime;

	// Handle CW Rotation
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		playerA += (speed * 2) * elapsedTime;

	// Handle Forwards movement & collision
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		playerX += sin(playerA * radToDegr) * speed * elapsedTime;
		playerY += cos(playerA * radToDegr) * speed * elapsedTime;
		if (map[(int)playerY][(int)playerX] == '#' || (int)playerY<=0.0 || (int)playerX <= 0.0)
		{
			playerX -= sin(playerA * radToDegr) * speed * elapsedTime;
			playerY -= cos(playerA * radToDegr) * speed * elapsedTime;
		}
	}
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		playerX -= sin(playerA * radToDegr) * speed * elapsedTime;
		playerY -= cos(playerA * radToDegr) * speed * elapsedTime;
		if (map[(int)playerY][(int)playerX] == '#' || (int)playerY <= 0.0 || (int)playerX <= 0.0)
		{
			playerX += sin(playerA * radToDegr) * speed * elapsedTime;
			playerY += cos(playerA * radToDegr) * speed * elapsedTime;
		}
	}
	playerA = std::fmod(playerA, 360.0);
	if (playerA < 0.0)
		playerA += 360.0;
	
}
