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
	this->direction = direction; // set direction
	
	if(direction == UP_DIRECTION) // set clipping based on what direction its going
		clipping = Master_Resource->clip_bullets[0];
	else if(direction == DOWN_DIRECTION)
		clipping = Master_Resource->clip_bullets[1];
	
	bulletSheets = Master_Resource->spriteSheets[1]; // set sprite sheet
	
	speed = 10;
	
	if(direction == UP_DIRECTION) // set the position of the bullet based off the shooter
		position = SDL_Rect{x - 7, SCRN_H - 95, 14, 40};
	else if(direction == DOWN_DIRECTION)
		position = SDL_Rect{x - 7, y, 14, 40};
	
	isExploding = false;
	exCounter = 0;
	
	this->explosionSheet = Master_Resource->explosion; // set the explosion sheet
	
	Main_Window->playFireSound(); // play the fire sound
}


Bullet::~Bullet() {
	/// Dont destroy the texture because its a shared resource
	bulletSheets = NULL;
}


void Bullet::render() {
	if(isExploding) { // if it is exploding then render the explosion animation
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
	
	if(direction == UP_DIRECTION) { // if its going up then render it pointing up
		SDL_RenderCopy(
			Main_Window->renderer,
			bulletSheets,
			&clipping,
			&position
		);
	}
	
	if(direction == DOWN_DIRECTION)	{ // If bullet is going down then rotate it so it points down
		SDL_RenderCopyEx( // Flip the bullet 180 so its pointing down then draw it
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
	if(isExploding) { // If we are exploding then dont move, just increment counter
		exCounter++;
		if(exCounter >= 80)
			this->destroy();
		return;
	}
	
	if(this->checkBulletHit()) // If another bullet has hit us then explode
		this->explode();
	
	if     (direction == UP_DIRECTION) // change the position depending on what the direction is
		this->position.y -= speed;
	else if(direction == DOWN_DIRECTION)
		this->position.y += speed;
	
	if(atEndOfMap()) // if its at the end of the map then destroy
		this->destroy();
}


void Bullet::destroy() {
	for(unsigned i = 0; i < Main_Window->playerBullets.size(); ++i) { // go through playerBullet
		if(this == Main_Window->playerBullets[i]) // and see if we are in there
			Main_Window->playerBullets.erase(Main_Window->playerBullets.begin() + i); // if we are erase ourself
	}
	
	for(unsigned i = 0; i < Main_Window->enemyBullets.size(); ++i) { // also go through enemyBullets
		if(this == Main_Window->enemyBullets[i])
			Main_Window->enemyBullets.erase(Main_Window->enemyBullets.begin() + i);
	}
	
	delete this; // and delete ourself
}


bool Bullet::atEndOfMap() {
	if(direction == UP_DIRECTION) // if bullet is going up, check if at top of map
		return this->position.y + position.h < 0;
	if(direction == DOWN_DIRECTION)
		return this->position.y > SCRN_H; // Bottom is at the end of the map
	return false;
}


void Bullet::explode() {
	exCounter = 0;
	isExploding = true;
	
	if(direction == UP_DIRECTION) // if bullet is going up then move explosion a bit up, for astetic reasons
		position.y -= 30;
	
	position.w = position.h; // make square explosion and preserve aspect ratio of explosion
}


bool Bullet::checkBulletHit() {
	switch(this->direction) {
		case UP_DIRECTION:
			for(auto tmpBullet: Main_Window->enemyBullets) { // check collisions with enemy bullets
				if(checkCollision(tmpBullet->position, this->position)) {
					if(tmpBullet->isExploding)
						continue;
						
					tmpBullet->explode(); // Destroy the bullet
					return true;
				}
			}
		break;
		
		case DOWN_DIRECTION:
			for(auto tmpBullet: Main_Window->playerBullets) { // check collisions with player bullets
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
