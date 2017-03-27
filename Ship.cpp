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
 * @version		9.2.0
 * @brief 	
 * =============================================================================
**/
#include "info.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <stdint.h>
#include <iostream>

#include ".\h\Ship.h"
#include ".\h\main.h"
#include ".\h\Window.h"
#include ".\h\Bullet.h"
#include ".\h\Powerup.h"
#include ".\h\Resources.h"

#include ".\h\debug.h"

///@todo Add friendly type to avaoid hitting
Ship::Ship(ShipTypes ship, Sizes size, Speeds speed) {
	this->speed = (int)speed; // Set the ships speed
	this->type = ship; // Set the ships type
	
	// Set all the flags to false and set all the counters to 0;
	this->exCounter     = 0;
	this->hasExploded   = false;
	this->hasRapidFire  = false;
	this->RFcounter     = 0;
	this->hasDoubleFire = false;
	this->DFcounter     = 0;
	this->hasShield     = false;
	this->shieldCounter = 0;
	this->hasBeam       = false;
	this->beamCounter   = 0;
	
	this->explosionTexture = Master_Resource->explosion; // set the explosion texture
	
	int subType = 0; // stores the subtype of the ship
	switch(ship) {
		case SHIP_ENEMY:
			subType = rand() % 5;

			this->texture = Master_Resource->spriteSheets[0];
			this->clipping = Master_Resource->clip_enemyShips[subType];
			
			this->position = SDL_Rect { // Set the ships position
				rand() % (SCRN_W - 60), ///< x position is a random value on the screen 
				-size - 50, ///< y position is a bit above the viewport so it seems to slowly drop down on the screen
				(int)size, // w
				(int)size // h
			};
		break;
		
		/// ==== **ASTROID** ====
		case SHIP_ASTROID:
			subType = rand() % 4 + A_SMALL;
			
			switch(subType) { // sets the clipping depending on the subtype
				case A_XLARGE: // first 8 are extra large astroids
					this->clipping = Master_Resource->clip_astroids[rand() % 8];
				break;
				
				case A_LARGE: // next 4 are large astroidx
					this->clipping = Master_Resource->clip_astroids[rand() % 4 + 8];
				break;
				
				case A_MEDIUM: // next 4 are medium astroidx
					this->clipping = Master_Resource->clip_astroids[rand() % 4 + 12];
				break;
				
				case A_SMALL: // last 4 are small astroidx
					this->clipping = Master_Resource->clip_astroids[rand() % 4 + 16];
				break;
			}
			
			this->texture = Master_Resource->spriteSheets[0]; // set the texture
			
			this->position = SDL_Rect { // Set the ships position
				rand() % (SCRN_W - 60), /// x position is a random value on the screen 
				-size - 50, /// y position is a bit above the viewport so it seems to slowly drop down on the screen
				this->clipping.w, // w
				this->clipping.h // h
			};
		break;
		
		case SHIP_BOSS:
			/*int subType = rand() % 3 + E_BOSS_1;

			texture = Master_Resource->spriteSheet[0];
			position = Master_Resource->clip_enemyShips[0];
			
			position = SDL_Rect { // Set the Boss's position
				SCRN_W / 2, ///< x position is middle of the screen
				0, ///< y position is 0
				(int)size, // w
				(int)size // h
			};*/
		break;
		
		case SHIP_PLAYER:
			this->texture = Master_Resource->spriteSheets[1];
			this->shieldTexture = Master_Resource->spriteSheets[0]; // set the shield Texture
			
			this->clipping = Master_Resource->clip_playerShips[0];
			this->shieldClipping = Master_Resource->clip_shield;
			
			this->speed = 7; // Set the ship's speed
			
			this->position = SDL_Rect {
				SCRN_W / 2, // x position is middle of the screen
				SCRN_H - 50, // y position is at the edge of the screen
				50, // w
				50 // h
			};
		break;
	}
}


Ship::~Ship() {
	/// Don't destroy renderer because it is a shared resource
	this->texture = NULL;
	this->explosionTexture = NULL;
}


void Ship::render() {
	if(this->hasExploded) {
		SDL_Rect explosionClipping = {
			(exCounter % 9) * 100,
			(exCounter / 9) * 100,
			100,
			100
		};
		
		SDL_RenderCopy(
			Main_Window->renderer,
			this->explosionTexture,
			&explosionClipping,
			&this->position
		);
		
		return;
	}
	
	switch(this->type) {
		case SHIP_PLAYER: // If the ship is the player's ship
			SDL_RenderCopyEx( // Flip the ship 180 so its pointing down then draw it
				Main_Window->renderer,
				this->texture,
				&this->clipping,
				&this->position,
				0,
				NULL,
				SDL_FLIP_VERTICAL
			);
			
			if(hasShield) {
				SDL_Rect pos = SDL_Rect{
					this->position.x - (this->shieldClipping.w + this->position.w) / 2,
					this->position.y - (this->shieldClipping.h + this->position.h) / 2,
					this->shieldClipping.w,
					this->shieldClipping.h
				};
				
				SDL_RenderCopy( // Draw the ship on the renderer
					Main_Window->renderer,
					this->shieldTexture,
					&this->shieldClipping,
					&pos
				);	
			}
			
		return;
	
		default: // If the ship is any other ship
			SDL_RenderCopy( // Draw the ship on the renderer
				Main_Window->renderer,
				this->texture,
				&this->clipping,
				&this->position
			);	
		return;
	}
}


void Ship::move() {	
	if(this->hasExploded) {
		this->exCounter++;
		if(this->exCounter >= 80)
			this->destroy();
		return;
	}
	
	switch(this->type) {
		case SHIP_PLAYER: // If this is a player's ship
			if(this->direction == LEFT_DIRECTION)
				this->position.x -= speed; // Move left 
			if(this->direction == RIGHT_DIRECTION)
				this->position.x += speed; // Move right 
			
			if(this->position.x < 0) // If the ship is at the left edge of the map
				this->position.x += this->speed; // Go back right
			if(this->position.x + this->position.w > SCRN_W) // If the ship is at the right edge
				this->position.x -= this->speed; // Go left
				
			if(this->checkBulletHit()) {
				if(!this->hasShield)
					Main_Window->removeLife(1);
			}
			
			Powerup* tmpPowerup;
			if(NULL != (tmpPowerup = this->getPowerup())) {
				tmpPowerup->doPowerup();
				tmpPowerup->isDestroyed = true;
			}
			tmpPowerup = NULL;
			
			if(this->hasRapidFire) {
				this->RFcounter++;
				
				if(this->RFcounter % 2 == 0)
					this->fire();
				
				if(this->RFcounter >= (FPS * POWERUP_TIME) / 2) {
					this->hasRapidFire = false;
					this->RFcounter = 0;
				}
			}
			
			if(this->hasDoubleFire) {
				this->DFcounter++;
				
				if(this->DFcounter >= FPS * POWERUP_TIME) {
					this->hasDoubleFire = false;
					this->DFcounter = 0;
				}
			}
			
			if(this->hasShield) {
				this->shieldCounter++;
				
				if(this->shieldCounter >= FPS * POWERUP_TIME) {
					this->hasShield = false;
					this->shieldCounter = 0;
				}
			}
			
			if(this->hasBeam) {
				this->beamCounter++;
				
				if(this->beamCounter >= FPS * POWERUP_TIME) {
					this->hasBeam = false;
					this->beamCounter = 0;
				}
			}
		return;
	
		case SHIP_ASTROID: // If the ship is an enemy
			this->position.y += speed; // Move downward
			
			if(this->checkBulletHit()) { // If we are hit by a bullet then die
				this->hasExploded = true;
				return;
			}
			
			if(this->atEndOfMap()) { // If we are at the end of the map, then ...
				this->destroy(); // ... suicide and ...
				return;
			}		
			///@todo Add 5px random movement on xPos
			
		return;
		
		case SHIP_BOSS:
		case SHIP_ENEMY:
			this->position.y += speed; // Move downward
			
			if(this->checkBulletHit()) { // If we are hit by a bullet then die
				this->hasExploded = true;
				return;
			}
			
			if(this->atEndOfMap()) { // If we are at the end of the map, then ...
				Main_Window->removeLife(1);
				this->destroy(); // ... suicide and ...
				return;
			}
			
			if(Main_Window->hasShield && atShield()) {
				this->hasExploded = true;
				return;
			}
		return;
	}
}


void Ship::turnOnRapidFire() {
	this->hasRapidFire = true;
	this->RFcounter = 0;
}


void Ship::turnOnDoubleFire() {
	this->hasDoubleFire = true;
	this->DFcounter = 0;
}


void Ship::turnOnShield() {
	this->hasShield = true;
	this->shieldCounter = 0;
}


void Ship::turnOnBeam() {
	this->hasBeam = true;
	this->beamCounter = 0;
}


void Ship::changeShip(ShipSubtypes ship) {
	/*SDL_DestroyTexture(texture); // Destroy this texture
	texture = NULL;
	
	texture = SDL_CreateTextureFromSurface( // Create new texture
		Main_Window->renderer,
		Master_Resource->SHIP_TEXTURES[ship]
	);
	
	type = ship;*/
}


void Ship::fire() {
	if(this->hasExploded) return;
	
	Bullet* tmpBullet = NULL;
	
	switch(type) {
		case SHIP_PLAYER: // If we are a player ship, then create bullet
			if(this->hasDoubleFire) {
				tmpBullet = new Bullet(
					UP_DIRECTION, // Bullet that goes up
					position.x // Position of the bullet
				);
				Main_Window->playerBullets.push_back(tmpBullet); // Store the pointer of the bullet in the vector
				
				tmpBullet = new Bullet(
					UP_DIRECTION, // Bullet that goes up
					position.x + position.w // Position of the bullet
				);
				Main_Window->playerBullets.push_back(tmpBullet); // Store the pointer of the bullet in the vector
				
				return;
			}
			
			if(this->hasBeam) {
				tmpBullet = new Bullet(
					UP_DIRECTION, // Bullet that goes up
					position.x + (position.w / 2) // Position of the bullet
				);
				Main_Window->playerBullets.push_back(tmpBullet); // Store the pointer of the bullet in the vector
				
				return;
			}
			
			tmpBullet = new Bullet(
				UP_DIRECTION, // Bullet that goes up
				position.x + (position.w / 2) // Position of the bullet
			);
			Main_Window->playerBullets.push_back(tmpBullet); // Store the pointer of the bullet in the vector
		return;
	
		case SHIP_ENEMY: // If we are an enemy ship
			tmpBullet = new Bullet(
				DOWN_DIRECTION, // Bullet that goes up
				position.x + (position.w / 2), // Position of the bullet
				position.y // Position of the bullet
			);
			Main_Window->enemyBullets.push_back(tmpBullet); // Store the pointer of the bullet in the vector
		return;
		
		default:
		break;
	}
}


void Ship::destroy() {
	for(unsigned i = 0; i < Main_Window->enemyShips.size(); ++i) { // Go through the ships ...
		if(this == Main_Window->enemyShips[i]) // ... and if it is us ...
			Main_Window->enemyShips.erase(Main_Window->enemyShips.begin() + i); // ... then erase us from the pool of ships
	}
	delete this; // Delete us
}


void Ship::changeDirection(Direction direction) {
	this->direction = direction;
}


bool Ship::checkBulletHit() {
	if(this->type == SHIP_ENEMY || this->type == SHIP_ASTROID) {
		/// @todo Change this to a range based for loop, so its shorter
		for(auto tmpBullet: Main_Window->playerBullets) {
			if(checkCollision(tmpBullet->position, this->position)) {
				if(tmpBullet->isExploding)
					continue;
					
				tmpBullet->explode(); // Destroy the bullet
				return true;
			}
		}
	}
	
	if(type == SHIP_PLAYER || type == SHIP_ASTROID) {
		if(hasShield) {
			SDL_Rect shieldPosition = SDL_Rect{
				this->position.x - 47,
				this->position.y - 47,
				this->shieldClipping.w,
				this->shieldClipping.h
			};
			for(auto tmpBullet: Main_Window->enemyBullets) {
				if(checkCollision(tmpBullet->position, shieldPosition)) {
					if(tmpBullet->isExploding)
						continue;
					
					tmpBullet->explode(); // Destroy the bullet
					return true;
				}
			}
		} else {
			/// @todo Change this to a range based for loop, so its shorter
			for(auto tmpBullet: Main_Window->enemyBullets) {
				if(checkCollision(tmpBullet->position, this->position)) {
					if(tmpBullet->isExploding)
						continue;
					
					tmpBullet->explode(); // Destroy the bullet
					return true;
				}
			}
		}
	}
	return false;
}


Powerup* Ship::getPowerup() {
	/// @todo Change this to a range based for loop, so its shorter
	for(unsigned i = 0; i < Main_Window->powerups.size(); ++i) {
		if(Main_Window->powerups[i]->isDestroyed == false && 
			!( Main_Window->powerups[i]->position.x > (this->position.x + this->position.w)
			|| Main_Window->powerups[i]->position.y > (this->position.y + this->position.h)
			
			|| Main_Window->powerups[i]->position.x + Main_Window->powerups[i]->position.w < this->position.x
			|| Main_Window->powerups[i]->position.y + Main_Window->powerups[i]->position.h < this->position.y
		)) {
			return Main_Window->powerups[i];
		}
	}
	return NULL;	
}


bool Ship::atEndOfMap() {
	return this->position.y > SCRN_H - 10; // Bottom is at the end of the map
	// @note this ads a 10px padding at the bottom of the "end of map"
}


bool Ship::atShield() {
	return (this->position.y + this->position.h) > SCRN_H - 35; // Bottom is at the end of the map
	// @note this ads a 10px padding at the bottom of the "end of map"
}
