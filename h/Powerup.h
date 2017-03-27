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

#ifndef POWERUP_H_INCLUDED
#define POWERUP_H_INCLUDED

#include ".\main.h"

/// @brief	A powerup
class Powerup {
public: // =====================================================================
	SDL_Rect position; ///< Stores the position fo the powerup
	
	bool isDestroyed; ///< Is the powerup Destroyed (for destruction animation)
	
	/// @brief	Default constructor
	/// @param	type \a \ref `PowerupType` - The type of powerup we want to create
	Powerup(PowerupType type);
	~Powerup(); ///< Default deconstructor
	
	void move(); 	  ///< Move the poweup down the board
	void render(); 	  ///< Draw the powerup on the board 
	void doPowerup(); ///< does the powerup function
private: // ====================================================================

	SDL_Texture* powerup; ///< Stores the tecture for our powerup
	SDL_Rect clipping;    ///< stores the clipping for our powerup
	
	PowerupSubtype type; ///< The Type of powerup
	
	int destCounter;
	void destroy();   ///< Destroy the powerup
	bool atEndofMap();
};

#endif