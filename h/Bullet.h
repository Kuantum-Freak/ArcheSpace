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

#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL.h>

#include ".\main.h"
#include ".\Window.h"

/// @brief A bullet
class Bullet {
public:
	SDL_Rect position; ///< Position of our bullet
	bool isExploding;
	
	/// @brief Creates a Bullet
	/// @param direction \a `Direction` - The direction the bullet will be going
	/// @param x \a `int` - The x cordinate of the bullet to create
	/// @param y \a `int` - The y cordinate of the bullet to create
	/// @note  \p y defaults to 95px above the bottom of the screen (for the players bullet)
	Bullet(Direction direction, int x, int y = SCRN_H - 95);
	~Bullet(); ///< @brief Deconstructor
	
	void render(); 	///< @brief Render our bullet
	void move(); 	///< @brief Moves our bullet
	void explode();
private:
	int exCounter;
	
	Direction direction; ///< Direction of the ship, @note Used only when ship is \ref SHIP_PLAYER
	int speed;           ///< Speed of the bullet
	
	SDL_Texture* bulletSheets; ///< The sprite sheet for our bullets
	SDL_Rect clipping;       ///< Main bullet clipping
	
	SDL_Texture* explosionSheet;
	
	void destroy(); ///< @brief Destroys the current bullet and removes it from the game
	
	/// @brief  Cheks f the ship is at the end of the map
	/// @return \a `bool` - if the ship is at the end of the map
	/// @retval TRUE - Ship is at the end of the map
	/// @retval FALSE - Ship is not at the end of the map
	bool atEndOfMap();
	
	bool checkBulletHit();
};

#endif // BULLET_H_INCLUDED