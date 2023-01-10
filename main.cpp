#include <SFML/Graphics.hpp>

#include "ray_casting.h"

#include "Sprites_1.h"

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 

#include <stdio.h>
#include <Windows.h>

int screenWidth = 640;			// Monitor Screen Size X (columns)
int screenHeight = 480;			// Monitor Screen Size Y (rows)


double PI = 3.14159265;
double radToDegr = PI / 180.0;
double distProjectPlane =(int)( screenWidth / 2.0 / tan(30.0*radToDegr));
double playerX = 1.01;			// Player Start Position
double playerY = 1.01;
double playerZ = 0.5;
double playerA = 0;			// Player Start Rotation


double depth = 16.0;			// Maximum rendering distance
double speed = 15.0;
double angleBetweenRays = 60.0/screenWidth; 
double ABR_Z = 60.0 / (screenHeight/2);
std::vector <sprite_object> sprite_stack;
auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();
/*	    
        180
    225  |  135
	   \ | /
  270<---|--->90
	   / | \
    315  |  45
	     0
*/

std::vector<std::vector <char>> map = { 
		{'#','#','#','#','#','#','#','.','.','#','#','#','#','#','#','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'}, 
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','#','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','.','#','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		{'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

std::vector<std::vector <char>> floor_map = {
		{'.','.','.','.','.','.','.','#','#','.','.','.','.','.','.','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','.','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','.','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','#','.','#','#','#','#','#','#','#','#','#','#','#','#','.'},
		{'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'}
};

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting");//create window
sf::Sprite wall_sprite;
sf::Sprite sprite_floor;
sf::Texture texture_Wall;
sf::Texture texture_Floor;
sf::Texture barrel;
sf::Texture texture_Floor2;

void drawWall(double rayLen, double angle, int i, int i_wall_sprite, bool isVert);
void playerDisplacement();
int duckDown();

int main()
{
	texture_Wall.loadFromFile("textures/brick.png");
	texture_Floor.loadFromFile("textures/wood_floor.png");
	barrel.loadFromFile("textures/barrel.png");

	sprite_floor.setTexture(texture_Floor);
	
	sprite_object enemy(barrel, 3.0, 14.0, 0.6, 0.6);

	ray_casting rc;
	rc.setMap(map);
	rc.setDepth(depth);

	std::vector <double> rayA_stack(screenWidth);
	std::vector <double> rayLen_stack(screenWidth);
	std::vector <bool> isVert_stack(screenWidth);
	std::vector <int> i_wall_stack(screenWidth);

    while (window.isOpen())
    {
		playerDisplacement();
		window.clear();
		sprite_floor.setPosition(sf::Vector2f(0, screenHeight - texture_Floor.getSize().y));
		//window.draw(sprite_floor);
		
		enemy.createSprite(playerX, playerY, playerA);
		
 		for (int i = 0; i < screenWidth; i++) 
		{
			double rayA = std::fmod(playerA - 30.0 + (i * angleBetweenRays), 360.0), rayX = playerX, rayY = playerY;
			double rayA_Z = 0.0;
			



			
			if (rayA < 0.0) rayA += 360.0;
			rc.setPoint(rayX, rayY, rayA);
			rayA_stack[i] = rayA;
			double rayLen = rc.rayCasting();
			rayLen_stack[i] = rayLen;
			
			bool isVert = rc.isVert;
			isVert_stack[i] = isVert;
			rayX = std::fmod((int)rc.getX(), 64) , rayY = std::fmod((int)rc.getY(), 64);
			
			int i_wall_sprite = rayX;
			if (isVert)
				i_wall_sprite = rayY;

			i_wall_stack[i] = i_wall_sprite;

			if (rayLen != -1.0)
				drawWall(rayLen, rayA, i, i_wall_sprite, isVert);
			double ProjSliceHeight = 64.0 / (rayLen * cos((rayA - playerA) * radToDegr)) * distProjectPlane;
			double beta = abs(rayA - playerA);
			int centerRowY = screenHeight / 2 + ProjSliceHeight / 2;
			double ch = 32 * distProjectPlane;
			double dist = 32 * distProjectPlane / centerRowY;
			double d = dist * cos(beta * radToDegr);
			int floorPointX, floorPointY;
			double stepX = sin(rayA * radToDegr);
			double stepY = cos(rayA * radToDegr);
			double stp = cos(beta * radToDegr);
			if (rayLen == -1)
				continue;
			for (int j = centerRowY; j < centerRowY+64; j++) {
				//floor texturing
				d = ch / j * stp;
				floorPointX = (int)(playerX * 64.0 + stepX * d)%64;
				floorPointY = (int)(playerY * 64.0 + stepY * d)%64;
				//sprite_floor.setPosition(sf::Vector2f(i, j));
				//sprite_floor.setTextureRect(sf::IntRect(floorPointX, 1, floorPointY, 1));
				//window.draw(sprite_floor);
			}
		}
		window.draw(enemy.sprt);
		for (int j = std::max(0, enemy.i_beg); j <= std::min(enemy.i_end, screenWidth-1); j++)
			if (rayLen_stack[j] < enemy.length && rayLen_stack[j] != -1.0) 
				drawWall(rayLen_stack[j], rayA_stack[j], j, i_wall_stack[j], isVert_stack[j]);
		

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
		
    }
	
}
void drawWall(double rayLen, double angle, int i, int i_wall_sprite, bool isVert)
{
	sf::Sprite part_wall_sprite; 
	
	part_wall_sprite.setTexture(texture_Wall);
	part_wall_sprite.setTextureRect(sf::IntRect(i_wall_sprite, 1, 1, 64));

	double ProjSliceHeight = 64.0 / (rayLen*cos((angle - playerA) * radToDegr)) * distProjectPlane;
	bool playerCheckVert = false;
	double multiplDarkening = 1;
	if ((playerA > 225 && playerA < 315) || (playerA > 45 && playerA < 135))
		playerCheckVert = true;
	

	part_wall_sprite.setScale(sf::Vector2f(1,ProjSliceHeight / 64));
	part_wall_sprite.setPosition(sf::Vector2f(i,(int)(screenHeight / 2 - (int)ProjSliceHeight / 2) ));
	int numOfPhases = 512;
	int phaseDarkening = rayLen / ((depth / numOfPhases) * 64);
	int r= (163 * multiplDarkening) / numOfPhases * (numOfPhases - phaseDarkening),
		g= (146 * multiplDarkening) / numOfPhases * (numOfPhases - phaseDarkening),
		b= (135 * multiplDarkening) / numOfPhases * (numOfPhases - phaseDarkening);
	part_wall_sprite.setColor(sf::Color(r, g, b));

	window.draw(part_wall_sprite);
}


int duckDown() {
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
		return screenHeight/4;
	}
	return 0;
}

//done
void playerDisplacement()
{
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
