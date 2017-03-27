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
 * @date			2017-03-20 (YYYY-MM-DD)
 * @version			4.0.0
 * @brief 		
 * =============================================================================
**/
#include "info.h"
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>

#include ".\h\main.h"
#include ".\h\Powerup.h"
#include ".\h\Window.h"
#include ".\h\Resources.h"
#include ".\h\Ship.h"

#include ".\h\debug.h"

Powerup::Powerup(PowerupType type) {
	PowerupSubtype subtype;
	switch(type) {
		case FIRE_POWERUP:
			subtype = (PowerupSubtype)((rand() % 3) + FIRE_POWERUP_1);
		break;
		
		case HEALTH_POWERUP:
			subtype = (PowerupSubtype)((rand() % 4) + HEALTH_POWERUP_1);
		break;
		
		case SHIELD_POWERUP:
			subtype = (PowerupSubtype)((rand() % 3) + SHIELD_POWERUP_1);
		break;
		
		case BEAM_POWERUP:
			subtype = (PowerupSubtype)((rand() % 3) + FIRE_POWERUP_1);
			//subtype = (PowerupSubtype)((rand() % 3) + BEAM_POWERUP_1);
		break;
		
		case POWERUP_TOTAL:
		default:
		break;
	}
	
	powerup = Master_Resource->spriteSheets[0];
	clipping = Master_Resource->clip_powerup[subtype];
	
	position = SDL_Rect{
		rand() % (SCRN_W - 60), /// x position is a random value on the screen 
		-clipping.h - 5, /// y position is a bit above the viewport so it seems to slowly drop down on the screen
		clipping.w,
		clipping.h
	};
	
	this->type = subtype;
	
	isDestroyed = false;
	destCounter = -1;
}


Powerup::~Powerup() {
	powerup = NULL;
}


void Powerup::move() {
	if(isDestroyed) {
		destCounter += 25;
		if(destCounter >= 254)
			this->destroy();
	} else {
		this->position.y += POWERUP_SPEED;
	}
	
	if(atEndofMap())
		destroy();
}


void Powerup::render() {	
	if(isDestroyed) {
		SDL_SetTextureAlphaMod(powerup, 255 - destCounter);
		SDL_RenderCopy(Main_Window->renderer, powerup, &clipping, &position);
		SDL_SetTextureAlphaMod(powerup, 255);
		return;
	}
	
	SDL_RenderCopy(Main_Window->renderer, powerup, &clipping, &position);
	
}


void Powerup::destroy() {
	for(unsigned i = 0; i < Main_Window->powerups.size(); ++i) {
		if(this == Main_Window->powerups[i])
			Main_Window->powerups.erase(Main_Window->powerups.begin() + i);
	}
	
	delete this;
}


void Powerup::doPowerup() {
	switch(type) {
		case FIRE_POWERUP_1:
			Main_Window->playerShip->turnOnDoubleFire();
		break;
		
		case FIRE_POWERUP_2:
			Main_Window->playerShip->turnOnRapidFire();
		break;
		
		case FIRE_POWERUP_3:
			Main_Window->playerShip->turnOnDoubleFire();
			Main_Window->playerShip->turnOnRapidFire();
		break;
		
		case HEALTH_POWERUP_1:
			Main_Window->removeLife(-1);
		break;
		
		case HEALTH_POWERUP_2:
			Main_Window->removeLife(-2);
		break;
		
		case HEALTH_POWERUP_3:
			Main_Window->removeLife(-3);
		break;
		
		case HEALTH_POWERUP_4:
			Main_Window->removeLife(-4);
		break;
		
		case SHIELD_POWERUP_1:
			Main_Window->turnOnShield();
		break;
		
		case SHIELD_POWERUP_2:
			Main_Window->playerShip->turnOnShield();
		break;
		
		case SHIELD_POWERUP_3:
			Main_Window->turnOnShield();
			Main_Window->playerShip->turnOnShield();
		break;
		
		case BEAM_POWERUP_1:
			Main_Window->playerShip->turnOnBeam();
		break;
		
		case BEAM_POWERUP_2:
		
		break;
		
		case BEAM_POWERUP_3:
		
		break;
		
		default: break;
	}
}


bool Powerup::atEndofMap() {
	return this->position.y > SCRN_H; // Bottom is at the end of the map
}