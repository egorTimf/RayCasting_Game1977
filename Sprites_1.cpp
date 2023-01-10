#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Sprites_1.h"

#define PI 3.14159265
#define screenWidth 640

sprite_object::sprite_object
(sf::Texture sprt_txt, double x1, double y1, double h, double w)
: txtr(sprt_txt), x(x1), y(y1), sprite_height(h), sprite_width(w)
{}



void sprite_object::createSprite(double playerX, double playerY, double playerA) {
	double theta = playerA;
	double dy = y  - playerY;
	double dx = x - playerX;
	length = sqrt(pow(dx, 2) + pow(dy, 2)) * 64.0;
	
	double sprite_dir = atan2 (dx*64.0 ,dy*64.0) * 180.0 / PI;
	while (sprite_dir - playerA > 180.0) sprite_dir -= 360.0;
	while (sprite_dir - playerA < -180.0) sprite_dir += 360.0;
	int thetaY = (sprite_dir - playerA)/(60.0/ screenWidth);

	double DPP = (int)(screenWidth  / 2.0 / tan(30.0 *PI / 180.0));
	double DPPW = (int)(64.0 / 2.0 / tan(30.0 * PI / 180.0));
	double PSH = (64.0) / (length * cos((sprite_dir - playerA) * PI / 180.0)) * DPP;
	double PSW = (64.0) / (length * cos((sprite_dir - playerA) * PI / 180.0)) * DPPW;
	width = 64.0 * PSW / 4;
	height =  PSH;
	i_beg = 320 + thetaY;
	i_end = i_beg + 64.0 * PSW/4;
	
	sprt.setTexture(txtr);
	sprt.setScale(sf::Vector2f(PSW*sprite_height/4, PSH*sprite_width / 64));
	sprt.setPosition(sf::Vector2f(320 + thetaY, (int)screenHeight / 2  - ((int)((PSH)*pow(sprite_width, 5)) / 2) ));
}