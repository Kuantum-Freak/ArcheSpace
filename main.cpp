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
 * @version		3.1.0
 * @brief 	
 * =============================================================================
**/
#include "info.h"

#include <iostream>
#include <SDL.h>
#include <SDL_Mixer.h>
#include <stdint.h>
#include <time.h>
#include <Windows.h>

#include ".\h\main.h"
#include ".\h\Window.h"
#include ".\h\Ship.h"
#include ".\h\Bullet.h"
#include ".\h\Powerup.h"
#include ".\h\Resources.h"

long long MEM_LEAK_COUNTER = 0;

void* operator new(size_t ptr, const char *file, int line) {
	MEM_LEAK_COUNTER++;
	//printf("Allocating in %s in line %u\n", file, line);
	//printf("counter: %ld\n", MEM_LEAK_COUNTER);
	return operator new(ptr);
}
void* operator new[](size_t ptr, const char *file, int line) {
	MEM_LEAK_COUNTER++;
	//printf("Allocating array in %s in line %u\n", file, line);
	//printf("counter: %ld\n", MEM_LEAK_COUNTER);
	return operator new[](ptr);
}
void pre_delete(const char *file, int line) {
	MEM_LEAK_COUNTER--;
	//printf("Dealocating in %s in line %u\n", file, line);
	//printf("counter: %ld\n", MEM_LEAK_COUNTER);
}

#include ".\h\debug.h"

const int FPS = 45;

const int MAX_SHIPS = 35;
const int MIN_SHIPS = 7;

const int SHIP_PROB = 350; // Default: 350
const int POWERUP_PROB = 7; // Default: 7
const int ENEMY_FIRE_PROB = 250; // Default: 250

const int LIVES = 7;

const int POWERUP_TIME = 15;
const int POWERUP_SPEED = 3;

int SCRN_W;
int SCRN_H;

Window* Main_Window;
Resources* Master_Resource;
uint32_t Main_Timer;

int main(int argc, char* argv[]) {
	PRINT_LEGAL_TERR; // Display Legal and Informational
	
	srand(time(NULL));
	
	initGraphics();
	
	Main_Window = new Window();
	Master_Resource = new Resources();
	Main_Window->init();
	
	Main_Window->showMenu();
	
	uint32_t FPS_Timer; // Stores the time the current frame took to render
	SDL_Event event; // Stores the current event
	
	Ship& playerShip = *Main_Window->playerShip; // Reference to the player's ship.  
	
	Main_Timer = SDL_GetTicks(); // Start the main timer
	// ==== MAIN GAME LOOP ====
	while(true) {
		FPS_Timer = SDL_GetTicks(); // Get the time at the start of the frame
		
		if(Main_Window->enemyShips.size() < MIN_SHIPS) { // If the amount of ships is less than MAX_SHIPS
			Main_Window->createEnemy();
		} else if(Main_Window->enemyShips.size() < MAX_SHIPS) { // If the amount of ships is less than the MIN_SHIPS
			if(rand() % 10000 < SHIP_PROB) // possiblity of creating a ship
				Main_Window->createEnemy();
		}

		if(rand() % 10000 < POWERUP_PROB)
			Main_Window->createPowerup(); // Create a powerup

		if(rand() % 10000 < ENEMY_FIRE_PROB)
			Main_Window->enemyFire(); // Enemy fire
		
		Main_Window->moveAll(); // Move all the ships on our screen
		Main_Window->renderFrame(); // Draw all the ships on our screen
		
		// ==== EVENT LOOP ====
		while(SDL_PollEvent(&event)) { // Get event
			if(event.type == SDL_QUIT) { // If event is QUIT, then exit
				quit(0x1);
			}
			
			if(!Main_Window->lost) {
				if(event.type == SDL_KEYUP && event.key.repeat == 0) { // If our event is KEYUP and is not being held down
					switch(event.key.keysym.sym) {
						case SDLK_d: // If our key is D or A then reset the direction
						case SDLK_a:
							playerShip.changeDirection(RESET_DIRECTION);
						break;
						
						case SDLK_SPACE: // If our key was space, fire a missle
							playerShip.fire();
						break;
					}
				}
				
				const Uint8* keystate = SDL_GetKeyboardState(NULL); // Get KeyBoard State
				if(keystate[SDL_SCANCODE_A]) // If player is holding down the A Key
					playerShip.changeDirection(LEFT_DIRECTION);
				if(keystate[SDL_SCANCODE_D]) // If player is holding down the D Key
					playerShip.changeDirection(RIGHT_DIRECTION);
			}
		}
		// ==== END EVENT LOOP ====
		
		int frameTicks = SDL_GetTicks() - FPS_Timer; // Get time it took to render frame
		if( frameTicks < 1000/FPS ) // If this was too big
			SDL_Delay( 1000/FPS - frameTicks ); // Wait remainging time
	}
	// ==== END MAIN GAME LOOP ====
	return 0;
}


void initGraphics() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Init SDL with Video
		EXIT("SDL Init Failed! " << SDL_GetError(), -0x101);
	
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) // Set render scaling quality
		EXIT("SDL Scaling Failed! " << SDL_GetError(), -0x101);
		
	int flags = IMG_INIT_PNG | IMG_INIT_JPG; // Init png and jpg/jpeg loading
	if(!(IMG_Init(flags) & flags))
		EXIT("IMG SDL Init Failed! " << IMG_GetError(), -0x101);
	
	if(TTF_Init() < 0) // Init TTF and text rendering
		EXIT("TTF SDL Init Failed! " << TTF_GetError(), -0x101);
	
	if(Mix_Init(MIX_INIT_OGG) < 0)
		EXIT("Mixer Init Failed! " << Mix_GetError(), -0x101);
	
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) //Initialize SDL_mixer
		EXIT("Mixer Init Failed! " << Mix_GetError(), -0x101);
}

bool checkCollision(SDL_Rect& box1, SDL_Rect& box2) {
	/** 
		@note this was moed from a different function, This algoritm is the same
		but bullet is box 1 abd ship is box 2
	*/
	/**
		*Algorithm* \n
		Checks 4 things:
			- If the bullet's left most part is greater than the ship's right most part.
			- If the bullet's top most part is greater than the ship's bottom most part.
			- If the bullet's right most part is less than the ship's left most part.
			- If the bullet's bottom most part is less than the ship's top most part.
		If any of these are true, then we have a miss and we continue checking the next bullet.
		Else, we have a hit.
		
		@note Actual code goes like this: If any of the points aren't true then we have a hit.\n
		(inverses the condition because we are looking for hits not misses)
	*/
	if(!(
		   box1.x > (box2.x + box2.w)
		|| box1.y > (box2.y + box2.h)
		
		|| box1.x + box1.w < box2.x
		|| box1.y + box1.h < box2.y
	)) return true;
	
	return false;
}

void quit(int code) {
	delete Main_Window;
	delete Master_Resource;
	
	IMG_Quit(); // Quit our subsystems
	TTF_Quit();
	SDL_Quit();
	
	exit(code);
}