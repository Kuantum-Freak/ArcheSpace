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

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include <SDL.h>
#include <stdint.h>

#include ".\main.h"
class Powerup;

/// @brief A Ship
class Ship {
public:
	/// @brief Default Constructor with defaults
	/// @param ship  \a      `ShipTypes` - Ship type
	/// @param size  \a \ref `Sizes` - Ship size
	/// @param speed \a \ref `Speeds` - Ship speed
	/** @note
		\p ship defaults to \ref SHIP_PLAYER \n
		\p size defaults to \ref LARGE_SIZE \n
		\p speed defaults to \ref FAST_SPEED \n
	*/
	Ship(
		ShipTypes ship = SHIP_PLAYER,
		Sizes size   = LARGE_SIZE,
		Speeds speed = FAST_SPEED
	);
	
	~Ship(); ///< @brief Default destructor
	
	void render(); 	///< @brief Draw the ship on to the board
	void move();    ///< @brief Move the ship one frame
	void fire();    ///< @brief Changes the direction
	
	void turnOnRapidFire();
	void turnOnDoubleFire();
	void turnOnShield();
	void turnOnBeam();
	
	/// @brief Changes the direction
	/// @param direction \a \ref `Direction` - The direction to change to
	void changeDirection(Direction direction);
	
	
	/// @brief Changes the ship texture
	/// @param ship \a \ref `ShipSubtypes` - The ship to change to
	void changeShip(ShipSubtypes ship);
	
private:
	bool hasRapidFire;
	int RFcounter;
	
	bool hasDoubleFire;
	int DFcounter;
	
	SDL_Texture* shieldTexture;
	SDL_Rect shieldClipping;
	bool hasShield;
	int shieldCounter;
	
	bool hasBeam;
	int beamCounter;
	
	SDL_Texture* texture; ///< The ship's image/texture
	ShipTypes type;      ///< Type of ship
	
	int speed; ///< Ships speed

	SDL_Rect position; ///< Ship's position
	SDL_Rect clipping;
	
	SDL_Texture* explosionTexture; ///< The explosion sprite sheet
	int exCounter;    ///< counter for the explosion animation
	bool hasExploded; ///< has the ship been (for explosion animation)
	
	Direction direction; ///< Direction (for playerShip)
	
	

	Powerup* getPowerup(); ///< @brief  Detects if a power-up has hit us
	
	void destroy(); ///< @brief Destroys this ship
	
	/// @brief  Detects a if a bullet hit this ship
	/// @return \a `bool` - if a bullet has hit us or not
	/// @retval TRUE  - if a bullet has hit us
	/// @retval FALSE  - if a bullet hasn't hit it
	bool checkBulletHit();
	
	/// @brief  Checks if its at the end of the map
	/// @return \a `bool` - at the end of the map of not
	/// @retval TRUE if at the end of map
	/// @retval FALSE if not at the end of the map
	bool atEndOfMap();
	
	bool atShield();
};

#endif // SHIP_H_INCLUDED