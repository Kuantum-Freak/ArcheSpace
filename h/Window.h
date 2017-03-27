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
 * @date			(YYYY-MM-DD)
 * @version		1.0.0
 * @brief 	
 * =============================================================================
**/

#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <vector>
#include <SDL.h>
#include <SDL_TTF.h>
using std::vector;

#include ".\main.h"

class Ship;    ///< Forward Declaration of \a Ship
class Bullet;  ///< Forward Declaration of \a Bullet
class Powerup; ///< Forward Declaration of \a Powerup

/// @brief	Stores our Window
class Window {
public: // =====================================================================
	SDL_Window* window;     ///< Stores our main Window
	SDL_Renderer* renderer; ///< Stores our main renderer
	
	SDL_Rect backgroundClip; ///< Stores the clipping of the image
	
	vector<Ship*> enemyShips;      ///< Stores pointers to all the enemy Ships and Astroids
	vector<Bullet*> playerBullets; ///< Stores pointers to all the Bullets
	vector<Bullet*> enemyBullets;  ///< Stores pointes to all the enemy Bullets
	vector<Powerup*> powerups;     ///< Stores all the powerups
	
	Ship* playerShip; ///< Stores a pointer to the player's Ship
	
	int lives; ///< Stores the lives we have left
	
	bool hasShield;
	int shieldCounter;
	
	bool lost;
	
	Window();  ///< @brief Creates our Window, and instantized graphic properties
	~Window(); ///< @brief Destroys our Window safetly
	
	void init();          ///< @brief Sets up the variables for use (need seperation)
	void renderFrame();   ///< @brief Renders a frame
	void createEnemy();   ///< @brief Creates a enemy with random values
	void playFireSound();  ///< @brief Plays a hit sound
	void moveAll();       ///< @brief Moves all the ships by one frame
	void enemyFire();     ///< @brief Fire an enemy bullet
	void createPowerup(); ///< @brief Creates a powerup
	void turnOnShield();
	void showMenu();
	
	/// @brief Removes lives
	/// @param numOfLives \a `int` - Number of lives to remove. If negative, adds lives
	void removeLife(int numOfLives);
	
private: // ====================================================================

	SDL_Texture* titleTexture;
	
	int points;
	
	void clear();          ///< @brief Clear the screen
	void playMusic();      ///< @brief Play Music
	void drawBackground(); ///< @brief Draws the Background
	void drawShips();      ///< @brief Draws the ships
	void drawBullets();    ///< @brief Draws the bullets
	void drawPoints();     ///< @brief Draws the points
	void drawHealth();     ///< @brief Draws the health bar
	void drawPowerups();   ///< @brief Draws the powerups
	void drawShield();
	void displayLose();///< @brief Displays the lose screen then quits
	void drawTitle();
	void showHowTo();
	void showCredits();
	
	SDL_Rect* drawButtons();
	void drawAButton(SDL_Rect*);
};

#endif // WINDOW_H_INCLUDED