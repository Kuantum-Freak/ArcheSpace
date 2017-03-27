/** 
 * Copyright (c) 2017 The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * =============================================================================
 * @author 			Aryan Gupta
 * @project 		
 * @file 			
 * @date			2017-03-12 (YYYY-MM-DD)
 * @version		2.2.0
 * @brief 	
 * =============================================================================
**/
#include "info.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

#include ".\h\Bullet.h"
#include ".\h\main.h"
#include ".\h\Window.h"
#include ".\h\Resources.h"

#include ".\h\debug.h"

Bullet::Bullet(Direction direction, int x, int y) {
	this->direction = direction;
	
	if(direction == UP_DIRECTION)
		clipping = Master_Resource->clip_bullets[0];
	else if(direction == DOWN_DIRECTION)
		clipping = Master_Resource->clip_bullets[1];
	
	bulletSheets = Master_Resource->spriteSheets[1];
	
	speed = 10;
	
	if(direction == UP_DIRECTION)
		position = SDL_Rect{x - 7, SCRN_H - 95, 14, 40};
	else if(direction == DOWN_DIRECTION)
		position = SDL_Rect{x - 7, y, 14, 40};
	
	isExploding = false;
	exCounter = 0;
	
	this->explosionSheet = Master_Resource->explosion;
	
	Main_Window->playFireSound();
}


Bullet::~Bullet() {
	/// Dont destroy the texture because its a shared resource
	bulletSheets = NULL;
}


void Bullet::render() {
	if(isExploding) {
		SDL_Rect explosionClipping = {
			(exCounter % 9) * 100,
			(exCounter / 9) * 100,
			100,
			100
		};
		
		SDL_RenderCopy(
			Main_Window->renderer,
			this->explosionSheet,
			&(explosionClipping),
			&(this->position)
		);
		
		return;
	}
	
	if(direction == UP_DIRECTION) {
		SDL_RenderCopy(
			Main_Window->renderer,
			bulletSheets,
			&clipping,
			&position
		);
	}
	
	if(direction == DOWN_DIRECTION)	{	
		SDL_RenderCopyEx( // Flip the ship 180 so its pointing down then draw it
			Main_Window->renderer,
			this->bulletSheets,
			&this->clipping,
			&this->position,
			0,
			NULL,
			SDL_FLIP_VERTICAL
		);
	}
}


void Bullet::move() {
	if(isExploding) {
		exCounter++;
		if(exCounter >= 80)
			this->destroy();
		return;
	}
	
	if(this->checkBulletHit())
		this->explode();
	
	if     (direction == UP_DIRECTION)
		this->position.y -= speed;
	else if(direction == DOWN_DIRECTION)
		this->position.y += speed;
	
	if(atEndOfMap())
		this->destroy();
}


void Bullet::destroy() {
	for(unsigned i = 0; i < Main_Window->playerBullets.size(); ++i) {
		if(this == Main_Window->playerBullets[i])
			Main_Window->playerBullets.erase(Main_Window->playerBullets.begin() + i);
	}
	
	for(unsigned i = 0; i < Main_Window->enemyBullets.size(); ++i) {
		if(this == Main_Window->enemyBullets[i])
			Main_Window->enemyBullets.erase(Main_Window->enemyBullets.begin() + i);
	}
	
	delete this;
}


bool Bullet::atEndOfMap() {
	if(direction == UP_DIRECTION)
		return this->position.y + position.h < 0;
	if(direction == DOWN_DIRECTION)
		return this->position.y > SCRN_H; // Bottom is at the end of the map
	return false;
}


void Bullet::explode() {
	exCounter = 0;
	isExploding = true;
	
	if(direction == UP_DIRECTION)
		position.y -= 30;
	
	position.w = position.h;
}


bool Bullet::checkBulletHit() {
	switch(this->direction) {
		case UP_DIRECTION:
			for(auto tmpBullet: Main_Window->enemyBullets) {
				if(checkCollision(tmpBullet->position, this->position)) {
					if(tmpBullet->isExploding)
						continue;
						
					tmpBullet->explode(); // Destroy the bullet
					return true;
				}
			}
		break;
		
		case DOWN_DIRECTION:
			for(auto tmpBullet: Main_Window->playerBullets) {
				if(checkCollision(tmpBullet->position, this->position)) {
					if(tmpBullet->isExploding)
						continue;
						
					tmpBullet->explode(); // Destroy the bullet
					return true;
				}
			}
		break;
		
		default:
		break;
	}
	
	return false;
}
