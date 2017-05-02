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
 * @version		4.1.0
 * @brief 	
 * =============================================================================
**/
#include "info.h"

#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>
#include <stdint.h>
#include <string>
using std::to_string;

#include ".\h\Window.h"
#include ".\h\main.h"
#include ".\h\Ship.h"
#include ".\h\Bullet.h"
#include ".\h\Powerup.h"
#include ".\h\Resources.h"

#include ".\h\debug.h"

const int BUTTON_SPACING = 100;

Window::Window() {
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode); // Get the properties of the screen

	SCRN_H = mode.h - 100;  // Set Window width
	SCRN_W = 580; //mode.w; // Set Window width	
	
	if( NULL ==
		(this->window = SDL_CreateWindow(  // Create our Window
			R_PRGMNAME,  // Window name
			SDL_WINDOWPOS_UNDEFINED,  // Window position (x)
			SDL_WINDOWPOS_UNDEFINED,  // Window position (y)
			SCRN_W, SCRN_H,  // Screen size
			SDL_WINDOW_SHOWN // Window flags
		))
	) EXIT("SDL Create Window Failed! " << SDL_GetError(), -0x101);
	
	if( NULL ==
		(this->renderer = SDL_CreateRenderer( // Create our main renderer
			window, -1,
			SDL_RENDERER_ACCELERATED // Renderer flags
		))
	) EXIT("SDL Create Renderer Failed! " << SDL_GetError(), -0x101);
	
	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
}


Window::~Window() {	
	SDL_DestroyRenderer(renderer); renderer = NULL; // Destroy our main renderer
	SDL_DestroyWindow(window); window = NULL; // Destroy our main window
	
	SDL_DestroyTexture(titleTexture);
	
	for(auto& i: enemyShips)    delete i; // Destroy all of our Enemy Ships
	for(auto& i: playerBullets) delete i; // Destroy all of the players bullets
	for(auto& i: enemyBullets)  delete i; // Destroy all of enemys bullets
	for(auto& i: powerups)      delete i; // Destroy all of the powerups
	
	delete playerShip; // Destroy our players ship
}


void Window::init() {
	lives = LIVES; // Set up total lives
	
	int w, h;
	SDL_QueryTexture(Master_Resource->background, NULL, NULL, &w, &h); // Get the size of our background
	this->backgroundClip = SDL_Rect { // Clip our background
		(w / 2) - (SCRN_W / 2), // Center the clipping on the picture
		0, // y position
		SCRN_W * h/SCRN_H, // Preserve the aspect ratio
		h // height
	};
	
	enemyShips.reserve(MAX_SHIPS + 5);
	playerBullets.reserve(50);
	enemyBullets.reserve(30);
	powerups.reserve(5);
	
	this->playerShip = new Ship(); // Instantize player's Ship
	
	hasShield = false;
	shieldCounter = 0;
	
	SDL_Color textColor = {0xFF, 0xFF, 0xFF}; // White Font
	SDL_Surface* tmpSurface = TTF_RenderText_Blended( // Create temp Surface for title
		Master_Resource->titleFont,
		R_PRGMNAME, 
		textColor
	);

	titleTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		tmpSurface
	);
	
	SDL_FreeSurface(tmpSurface);
	
	lost = false;
}


void Window::renderFrame() {
	this->clear(); // Clear the screen
	
	this->drawBackground(); // Draw the background
	
	this->drawBullets(); // Draw the bullets
	this->drawShips(); // Draw our ships
	this->drawPowerups();
	
	this->drawPoints(); // Draw our point
	this->drawHealth(); // Draw our health bar

	if(hasShield) { // If we have a sheild, then render
		if(shieldCounter >= FPS * POWERUP_TIME)
			hasShield = false;
		this->drawShield();
	}
	
	if(lost) // If we have lost then render the lost text
		displayLose();
	
	SDL_RenderPresent(renderer); // Present our renderer
}


void Window::createEnemy() {
	// Create random enemy
	ShipTypes tmpShipType = (ShipTypes)(rand() % 4 == 0)? // SHIP TYPE
		SHIP_ASTROID : SHIP_ENEMY;
		
	int tempNum = rand();
	Sizes tmpSize = (Sizes) //SIZES
		  (tempNum % 6 == 0)? SMALL_SIZE
		: (tempNum % 6 <= 3)? MEDIUM_SIZE
		: LARGE_SIZE;
	
	tempNum = rand();
	Speeds tmpSpeed = (Speeds) //SPEED
		  (tempNum % TOTAL_SPEED == 1)? SLOW_SPEED
		: (tempNum % TOTAL_SPEED == 2)? MEDIUM_SPEED
		: FAST_SPEED;
	
	this->enemyShips.push_back(new Ship(tmpShipType, tmpSize, tmpSpeed));
	//this->enemyShips.push_back(new Ship(tmpShipType, tmpSize, tmpSpeed));
}


void Window::playFireSound() {
	Mix_PlayChannel( -1, Master_Resource->fire, 0 );
}


void Window::removeLife(int numOfLives) {
	lives -= numOfLives;
	
	if(lives == -1) {
		this->points = (int)((SDL_GetTicks() - Main_Timer) / 250);
		lost = true;
	}
}


void Window::moveAll() {
	shieldCounter++;
	
	for(auto& i: enemyShips)    i->move(); // Move each enemy ship
	for(auto& i: playerBullets) i->move(); // Move each bullet
	for(auto& i: enemyBullets)  i->move(); // Move each bullet
	for(auto& i: powerups)      i->move(); // Move each powerup
	
	playerShip->move(); // Move player
}


void Window::enemyFire() {
	enemyShips[rand() % enemyShips.size()]->fire();
}


void Window::createPowerup() {
	powerups.push_back(new Powerup((PowerupType)(rand() % POWERUP_TOTAL)));
}


void Window::turnOnShield() {
	hasShield = true;
	shieldCounter = 0;
}


void Window::clear() {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black
	SDL_RenderClear(renderer);
}


void Window::playMusic() {
	//@todo CODE THIS
}


void Window::drawBackground() {
	SDL_RenderCopy(renderer, Master_Resource->background, &backgroundClip, NULL); // draw the background
}


void Window::drawShips() {
	for(auto &tmpShip: enemyShips) {
		tmpShip->render(); // Go through all the ships and draw it
	}
	this->playerShip->render(); // Draw player's ship
}


void Window::drawBullets() {
	for(auto &tmpBullet: playerBullets) { // Go through all the player's bullets and draw it
		tmpBullet->render();
	}
	
	for(auto &tmpBullet: enemyBullets) { // Go through all the enemy's bullets and draw it
		tmpBullet->render();
	}
}


void Window::drawPowerups() {
	for(auto &tmpPowerup: powerups) { // Go through all the player's bullets and draw it
		tmpPowerup->render();
	}
}


void Window::drawPoints() {
	SDL_Color textColor = {0xFF, 0xFF, 0xFF}; // White Font
	
	if(!lost)
		points = (int)((SDL_GetTicks() - Main_Timer) / 250);
	
	SDL_Surface* textSurface = TTF_RenderText_Blended( // Create temp Surface for text
		Master_Resource->mainFont,
		to_string(points).c_str(), 
		textColor
	);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		textSurface
	);
	
	SDL_Rect textPos {10, 0, textSurface->w, textSurface->h}; // Text position
	
	SDL_RenderCopy(Main_Window->renderer, textTexture, NULL, &textPos); // Draw
	
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}


void Window::drawHealth() {
	if(lives <= 3) {
		for(int i = 0; i <= lives; ++i) {
			SDL_Rect healthBar = {
				SCRN_W - (45 * i),
				12,
				30,
				30
			};
			SDL_RenderCopy(Main_Window->renderer, Master_Resource->healthHeart, NULL, &healthBar);
		}
		return;
	}
	
	SDL_Rect healthBar = {
		SCRN_W - 90,
		12,
		30,
		30
	};
	SDL_RenderCopy(Main_Window->renderer, Master_Resource->healthHeart, NULL, &healthBar);
	
	SDL_Color textColor = {0xFF, 0xFF, 0xFF}; // White Font
	SDL_Surface* textSurface = TTF_RenderText_Blended( // Create temp Surface for text
		Master_Resource->mainFont,
		(std::string(" x ") + std::to_string(lives)).c_str(), 
		textColor
	);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		textSurface
	);
	
	SDL_Rect textPos {SCRN_W - 55, 5, textSurface->w, textSurface->h}; // Text position
	SDL_RenderCopy(Main_Window->renderer, textTexture, NULL, &textPos); // Draw
	
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}


void Window::drawShield() {
	SDL_Rect shieldLoc = SDL_Rect{0, SCRN_H - 30, SCRN_W, 30};
	//	8b8989
	SDL_SetRenderDrawColor(renderer, 0x8B, 0x89, 0x89, 0x50);
	SDL_RenderFillRect(renderer, &shieldLoc);
}


void Window::displayLose() {	
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xC0);
	SDL_RenderFillRect(renderer, NULL);
	
	SDL_Color textColor = SDL_Color{0xFF, 0xFF, 0xFF};
	SDL_Surface* loseSurf = TTF_RenderText_Blended( // Create temp Surface for text
		Master_Resource->mainFont,
		"You Lose", 
		textColor
	);
	
	SDL_Texture* loseTex = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		loseSurf
	);
	
	SDL_Rect losePos = SDL_Rect{
		SCRN_W / 2 - loseSurf->w / 2,
		SCRN_H / 2 - loseSurf->h / 2, 
		loseSurf->w, 
		loseSurf->h
	};
	
	SDL_Surface* finalPointsSurface = TTF_RenderText_Blended( // Create temp Surface for text
		Master_Resource->mainFont,
		to_string(points).c_str(), 
		textColor
	);

	SDL_Texture* finalPointsTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		finalPointsSurface
	);
	
	SDL_Rect finalPointsPos = {
		SCRN_W / 2 - finalPointsSurface->w / 2,
		losePos.y + loseSurf->h + 10, 
		finalPointsSurface->w, 
		finalPointsSurface->h
	}; // Text position
	
	SDL_RenderCopy(Main_Window->renderer, finalPointsTexture, NULL, &finalPointsPos); // Draw
	SDL_RenderCopy(renderer, loseTex, NULL, &losePos);
	
	SDL_FreeSurface(loseSurf); loseSurf = NULL;
	SDL_DestroyTexture(loseTex); loseTex = NULL;

	SDL_FreeSurface(finalPointsSurface); finalPointsSurface = NULL;
	SDL_DestroyTexture(finalPointsTexture); finalPointsTexture = NULL;
}


void Window::showMenu() {
	SDL_Event event;
	uint32_t FPS_Timer; // Stores the time the current frame took to render
	
	while(true) {
		FPS_Timer = SDL_GetTicks(); // Get the time at the start of the frame
		
		this->clear();
		this->drawBackground();
		this->drawTitle();
		SDL_Rect* buttons = this->drawButtons();
		
		SDL_RenderPresent(renderer);
		
		while(SDL_PollEvent(&event) != 0){
		
			if(event.type == SDL_QUIT) {
				delete buttons;
				quit(0x1);
			}
			
			if(event.type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				
				if(
					   x > buttons[0].x
					&& x < buttons[0].x + buttons[0].w
					&& y > buttons[0].y
					&& y < buttons[0].y + buttons[0].h
				) {
					delete buttons;
					return;
				}
				
				if(
					   x > buttons[1].x
					&& x < buttons[1].x + buttons[1].w
					&& y > buttons[1].y
					&& y < buttons[1].y + buttons[1].h
				) showHowTo();
				
				if(
					   x > buttons[2].x
					&& x < buttons[2].x + buttons[2].w
					&& y > buttons[2].y
					&& y < buttons[2].y + buttons[2].h
				) showCredits();
			}
		}
		
		delete buttons;
		
		int frameTicks = SDL_GetTicks() - FPS_Timer; // Get time it took to render frame
		if( frameTicks < 1000/FPS ) // If this was too big
			SDL_Delay( 1000/FPS - frameTicks ); // Wait remainging time
	}
	
}


void Window::drawTitle() {
	int w, h;
	SDL_QueryTexture(titleTexture, NULL, NULL, &w, &h); // Get the size of our background
	
	SDL_Rect textPos {
		(SCRN_W / 2) - (w / 2),
		SCRN_H * 25/100, 
		w, 
		h
	}; // Text position
	
	SDL_RenderCopy(Main_Window->renderer, titleTexture, NULL, &textPos); // Draw
}


SDL_Rect* Window::drawButtons() {
	SDL_Color textColor{0xFF, 0xFF, 0xFF}; // White Font
	int numbuttons = 3;
	
	SDL_Rect* buttons = new SDL_Rect[numbuttons];
	
	const char* texts[] = {
		"Play",
		"How To Play",
		"Credits"
	};
	
	for(int i = 0; i < numbuttons; ++i) {
		SDL_Surface* textSurface = TTF_RenderText_Blended( // Create temp Surface for text
			Master_Resource->mainFont,
			texts[i], 
			textColor
		);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
			Main_Window->renderer,
			textSurface
		);
		SDL_Rect textPos = SDL_Rect{
			(SCRN_W / 2) - (textSurface->w / 2),
			(SCRN_H * 2/4) + (i * BUTTON_SPACING),// - (textSurface->h / 2),
			textSurface->w, 
			textSurface->h
		}; // Text position
		
		buttons[i] = SDL_Rect{
			SCRN_W / 2 - ((textSurface->w + 75) / 2),
			SCRN_H * 2/4 - ((textSurface->h + 15) / 8) + (i * BUTTON_SPACING),
			textSurface->w + 75,
			textSurface->h + 15
		};
		
		drawAButton(&buttons[i]);
		
		SDL_RenderCopy(Main_Window->renderer, textTexture, NULL, &textPos); // Draw
		
		SDL_FreeSurface(textSurface); textSurface = NULL;
		SDL_DestroyTexture(textTexture); textTexture = NULL;
	}
	
	return buttons;
}


void Window::drawAButton(SDL_Rect* button) {
	SDL_Texture* buttonTex = Master_Resource->spriteSheets[2];
	int x_i, y_i, w, h, h_p, w_p, x_f, y_f;
	
	x_i = 200;
	y_i = 300;
	w = 100;
	h = 100;
	
	h_p = 10;
	w_p = 6;
	
	x_f = x_i + w;
	y_f = y_i + h;
	
	SDL_Rect clip_topleft     = SDL_Rect{x_i, y_i, w_p, h_p};
	SDL_Rect clip_topright    = SDL_Rect{x_f - w_p, y_i, w_p, h_p};
	SDL_Rect clip_bottomleft  = SDL_Rect{x_i, y_f - h_p, w_p, h_p};
	SDL_Rect clip_bottomright = SDL_Rect{x_f - w_p, y_f - h_p, w_p, h_p};
	
	SDL_Rect clip_top    = SDL_Rect{x_i + w_p, y_i, w - 2*w_p, h_p};
	SDL_Rect clip_left   = SDL_Rect{x_i, y_i + h_p, w_p, h - 2*h_p};
	SDL_Rect clip_right  = SDL_Rect{x_f - w_p, y_i + h_p, w_p, h - 2*h_p};
	SDL_Rect clip_bottom = SDL_Rect{x_i + w_p, y_f - h_p, w - 2*w_p, h_p};
	
	SDL_Rect clip_center = SDL_Rect{x_i + w_p, y_i + h_p, w - 2*w_p, h - 2*h_p};

	x_i = button->x;
	y_i = button->y;
	w = button->w;
	h = button->h;
	
	h_p = 10;
	w_p = 6;
	
	x_f = x_i + w;
	y_f = y_i + h;
	
	SDL_Rect pos_topleft     = SDL_Rect{x_i, y_i, w_p, h_p};
	SDL_Rect pos_topright    = SDL_Rect{x_f - w_p, y_i, w_p, h_p};
	SDL_Rect pos_bottomleft  = SDL_Rect{x_i, y_f - h_p, w_p, h_p};
	SDL_Rect pos_bottomright = SDL_Rect{x_f - w_p, y_f - h_p, w_p, h_p};
	
	SDL_Rect pos_top    = SDL_Rect{x_i + w_p, y_i, w - 2*w_p, h_p};
	SDL_Rect pos_left   = SDL_Rect{x_i, y_i + h_p, w_p, h - 2*h_p};
	SDL_Rect pos_right  = SDL_Rect{x_f - w_p, y_i + h_p, w_p, h - 2*h_p};
	SDL_Rect pos_bottom = SDL_Rect{x_i + w_p, y_f - h_p, w - 2*w_p, h_p};
	
	SDL_Rect pos_center = SDL_Rect{x_i + w_p, y_i + h_p, w - 2*w_p, h - 2*h_p};
	
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_topleft, &pos_topleft);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_topright, &pos_topright);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_bottomleft, &pos_bottomleft);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_bottomright, &pos_bottomright);
	
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_top, &pos_top);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_left, &pos_left);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_right, &pos_right);
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_bottom, &pos_bottom);
	
	SDL_RenderCopy(Main_Window->renderer, buttonTex, &clip_center, &pos_center);
}


void Window::showHowTo() {
	///@todo function clean up
	int lines = 27; // the number of lines in the help text
	const char* helpText[lines] = {
		"Welcome to Archespace,",
		"A next generation top-down space shooter.",
		" ",
		"To play the game, you can use your \"A\" and \"D\" arrow", 
		"keys to move the ship left and right respectivly. You", 
		"can also use the spacebar to fire. ",
		" ",
		"There are many powerups that you can collect: ",
		" ",
		"Health Powerups: ",
		"This powerup gives you more health.",
		" ",
		" ",
		"Fire Powerups:",
		"This Powerup gives you better firing abilities.",
		" ",
		" ",
		"Shield Powerups: ",
		"This powerup grants you a sheild",
		" ",
		" ",
		"There are 3 tiers to each powerup: Gold, Silver, and", 
		"Bronze. Gold powerups are cooler than silver and so", 
		"on an so forth...",
		" ",
		"Thank you for playing my game and as always...", 
		"Have Fun!"
	};
	
	SDL_Color textColor = {0xFF, 0xFF, 0xFF}; // White Font
	
	int titleW, titleH; // the height and width of the title
	SDL_QueryTexture(titleTexture, NULL, NULL, &titleW, &titleH); // Get the size of our background
	
	SDL_Rect titlePos { // current title position
		(SCRN_W / 2) - (titleW / 2),
		SCRN_H * 1/4,
		titleW, 
		titleH
	};
	
	
	SDL_Texture* powerups = Master_Resource->spriteSheets[0]; // Powerups sprite sheet
	SDL_Rect PUclipping[] = { // Clipping for the powerups
		Master_Resource->clip_powerup[ 2], // Lighting
		Master_Resource->clip_powerup[ 3], // Pill
		Master_Resource->clip_powerup[ 9], // Shield
		Master_Resource->clip_powerup[12]  // Star
	};
	
	SDL_Rect PUpos[] = {
		SDL_Rect{215, 336, PUclipping[0].w, PUclipping[0].h}, // Lightning
		SDL_Rect{215, 265, PUclipping[1].w, PUclipping[1].h}, // Pill
		SDL_Rect{215, 417, PUclipping[2].w, PUclipping[2].h}, // Shield
		SDL_Rect{215, 90, PUclipping[3].w, PUclipping[3].h}   // Star
	};
	
	
	SDL_Surface* buttonTextSurface = TTF_RenderText_Blended( // Create temp Surface for button text
		Master_Resource->mainFont,
		"Back", // Text
		textColor
	);
	
	SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		buttonTextSurface
	);
	
	SDL_Rect buttonTextPos = SDL_Rect{ // position of the button
		(SCRN_W / 2) - (buttonTextSurface->w / 2),
		SCRN_H * 1/8 + ((lines + 1) * 20),
		buttonTextSurface->w, 
		buttonTextSurface->h
	};
	
	SDL_Rect button = SDL_Rect{
		buttonTextPos.x -  75/2,
		buttonTextPos.y - ((buttonTextSurface->h + 15) / 8),
		buttonTextSurface->w + 75,
		buttonTextSurface->h + 15
	};
	
	
	SDL_Surface** helpTextSurf = new SDL_Surface*[lines];
	SDL_Texture** helpTextTex  = new SDL_Texture*[lines];
	SDL_Rect*     helpTextPos  = new SDL_Rect[lines];
	
	for(int j = 0; j < lines; ++j) {
		helpTextSurf[j] = TTF_RenderText_Blended( // Create temp Surface for text
			Master_Resource->briefFont,
			helpText[j], 
			textColor
		);
		
		helpTextTex[j] = SDL_CreateTextureFromSurface( // Convert it to a texture
			Main_Window->renderer,
			helpTextSurf[j]
		);
		
		helpTextPos[j] = SDL_Rect{
			10,
			SCRN_H * 1/8 + (j * 20), 
			helpTextSurf[j]->w, 
			helpTextSurf[j]->h
		};
	}
	
	
	uint32_t FPS_Timer = 0;
	uint8_t alpha = 0;
	while(titlePos.y >= 15) {
		FPS_Timer = SDL_GetTicks();
		this->clear();
		this->drawBackground();
		
		titlePos.y -= 2;
		
		SDL_RenderCopy(renderer, titleTexture, NULL, &titlePos); // Draw
		
		///@todo prerender this
		for(int j = 0; j < lines; ++j) {
			SDL_SetTextureAlphaMod(helpTextTex[j], alpha);
			SDL_RenderCopy(renderer, helpTextTex[j], NULL, &helpTextPos[j]);
		}
		
		for(int j = 0; j < 3; ++j) {
			SDL_SetTextureAlphaMod(powerups, alpha); /// @warning reset alpha after use
			SDL_RenderCopy(renderer, powerups, &PUclipping[j], &PUpos[j]);
		}
		
		SDL_SetTextureAlphaMod(buttonTextTexture, alpha);
		SDL_SetTextureAlphaMod(Master_Resource->spriteSheets[2], alpha); /// @warning reset alpha after use
		drawAButton(&button);
		SDL_RenderCopy(Main_Window->renderer, buttonTextTexture, NULL, &buttonTextPos); // Draw
		
		SDL_RenderPresent(renderer);
		
		if(alpha >= 0xFB)
			alpha = 0xFB;
		else
			alpha += 3;
		
		int frameTicks = SDL_GetTicks() - FPS_Timer; // Get time it took to render frame
		if( frameTicks < 1000/FPS ) // If this was too big
			SDL_Delay( 1000/FPS - frameTicks ); // Wait remainging time
	}
	SDL_RenderPresent(renderer);
	
	SDL_Event event;
	while(true) {
		SDL_PollEvent(&event);
		
		if(event.type == SDL_QUIT) {
			SDL_SetTextureAlphaMod(powerups, 0xFF);
			SDL_SetTextureAlphaMod(Master_Resource->spriteSheets[2], 0xFF);
			
			SDL_FreeSurface(buttonTextSurface);
			SDL_DestroyTexture(buttonTextTexture);
			
			for(int i = 0; i < lines; ++i) {
				SDL_FreeSurface(helpTextSurf[i]);
				SDL_DestroyTexture(helpTextTex[i]);
			}
			
			delete[] helpTextSurf;
			delete[] helpTextTex;
			delete[] helpTextPos;
			
			quit(0x1);
		}
		
		if(event.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(
				   x > button.x
				&& x < button.x + button.w
				&& y > button.y
				&& y < button.y + button.h
			) {
				SDL_SetTextureAlphaMod(powerups, 0xFF);
				SDL_SetTextureAlphaMod(Master_Resource->spriteSheets[2], 0xFF);
				
				SDL_FreeSurface(buttonTextSurface);
				SDL_DestroyTexture(buttonTextTexture);
				
				for(int i = 0; i < lines; ++i) {
					SDL_FreeSurface(helpTextSurf[i]);
					SDL_DestroyTexture(helpTextTex[i]);
				}
				
				delete[] helpTextSurf;
				delete[] helpTextTex;
				delete[] helpTextPos;
				
				return;
			}
		}
	}
}


void Window::showCredits() {
	int lines = 12;
	
	const char* helpText[lines] = {
		"Legal Stuffs: ",
		"ArcheSpace v1.0.0 by Aryan Gupta",
		"Copyright (c) The Gupta Empire - All Rights Reserved",
		"The Gupta Empire - http://theguptaempire.net",
		" ",
		"Thanks to: ",
		"All my friends that gave advice to create this game",
		" ",
		"Credits: ",
		"Sprites - Kenney.nl",
		"Explosion Animation - goocreate.com (maybe?)",
		"Alien Ships - www.gameart2d.com"
	};
	
	SDL_Color textColor = {0xFF, 0xFF, 0xFF}; // White Font
	
	int titleW, titleH; // the height and width of the title
	SDL_QueryTexture(titleTexture, NULL, NULL, &titleW, &titleH); // Get the size of our background
	
	SDL_Rect titlePos { // current title position
		(SCRN_W / 2) - (titleW / 2),
		SCRN_H * 1/4,
		titleW, 
		titleH
	};
	
	
	SDL_Surface* buttonTextSurface = TTF_RenderText_Blended( // Create temp Surface for button text
		Master_Resource->mainFont,
		"Back", // Text
		textColor
	);
	
	SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface( // Convert it to a texture
		Main_Window->renderer,
		buttonTextSurface
	);
	
	SDL_Rect buttonTextPos = SDL_Rect{ // position of the button
		(SCRN_W / 2) - (buttonTextSurface->w / 2),
		SCRN_H * 1/8 + ((lines + 1) * 20),
		buttonTextSurface->w, 
		buttonTextSurface->h
	};
	
	SDL_Rect button = SDL_Rect{
		buttonTextPos.x -  75/2,
		buttonTextPos.y - ((buttonTextSurface->h + 15) / 8),
		buttonTextSurface->w + 75,
		buttonTextSurface->h + 15
	};
	
	
	SDL_Surface** helpTextSurf = new SDL_Surface*[lines];
	SDL_Texture** helpTextTex  = new SDL_Texture*[lines];
	SDL_Rect*     helpTextPos  = new SDL_Rect[lines];
	
	for(int j = 0; j < lines; ++j) {
		helpTextSurf[j] = TTF_RenderText_Blended( // Create temp Surface for text
			Master_Resource->briefFont,
			helpText[j], 
			textColor
		);
		
		helpTextTex[j] = SDL_CreateTextureFromSurface( // Convert it to a texture
			Main_Window->renderer,
			helpTextSurf[j]
		);
		
		helpTextPos[j] = SDL_Rect{
			10,
			SCRN_H * 1/8 + (j * 20), 
			helpTextSurf[j]->w, 
			helpTextSurf[j]->h
		};
	}
	
	
	uint32_t FPS_Timer = 0;
	uint8_t alpha = 0;
	while(titlePos.y >= 15) {
		FPS_Timer = SDL_GetTicks();
		this->clear();
		this->drawBackground();
		
		titlePos.y -= 2;
		
		SDL_RenderCopy(renderer, titleTexture, NULL, &titlePos); // Draw
		
		///@todo prerender this
		for(int j = 0; j < lines; ++j) {
			SDL_SetTextureAlphaMod(helpTextTex[j], alpha);
			SDL_RenderCopy(renderer, helpTextTex[j], NULL, &helpTextPos[j]);
		}
		
		SDL_SetTextureAlphaMod(buttonTextTexture, alpha);
		SDL_SetTextureAlphaMod(Master_Resource->spriteSheets[2], alpha); /// @warning reset alpha after use
		drawAButton(&button);
		SDL_RenderCopy(Main_Window->renderer, buttonTextTexture, NULL, &buttonTextPos); // Draw
		
		SDL_RenderPresent(renderer);
		
		if(alpha >= 0xFB)
			alpha = 0xFB;
		else
			alpha += 3;
		
		int frameTicks = SDL_GetTicks() - FPS_Timer; // Get time it took to render frame
		if( frameTicks < 1000/FPS ) // If this was too big
			SDL_Delay( 1000/FPS - frameTicks ); // Wait remainging time
	}
	SDL_RenderPresent(renderer);
	
	SDL_Event event;
	while(true) {
		SDL_PollEvent(&event);
		
		if(event.type == SDL_QUIT)
			quit(0x1);
		
		if(event.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(
				   x > button.x
				&& x < button.x + button.w
				&& y > button.y
				&& y < button.y + button.h
			) {
				SDL_FreeSurface(buttonTextSurface);
				SDL_DestroyTexture(buttonTextTexture);
				
				for(int i = 0; i < lines; ++i) {
					SDL_FreeSurface(helpTextSurf[i]);
					SDL_DestroyTexture(helpTextTex[i]);
				}
				
				delete[] helpTextSurf;
				delete[] helpTextTex;
				delete[] helpTextPos;
				
				return;
			}
		}
	}
}