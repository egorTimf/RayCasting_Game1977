#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath> 
#include <SFML/Graphics.hpp>
#include <stdio.h>


class  sprite_object {
public:
	sprite_object(sf::Texture sprt_txt, double x, double y, double h, double w); 
	void createSprite(double plX, double plY, double plA);
	
	sf::Sprite sprt;
	double x,y;
	int i_beg, i_end;
	double length;
	int width, height;
private:
	sf::Texture txtr;
	int screenHeight = 480;
	double sprite_width = 1;
	double sprite_height = 1;
};