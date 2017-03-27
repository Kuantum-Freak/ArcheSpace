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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdint.h>
#include <SDL.h>
#include <SDL_Image.h>

class Window; ///< Forward declaration
struct Resources;

enum ShipTypes {
	SHIP_PLAYER,
	SHIP_ENEMY,
	SHIP_ASTROID,
	SHIP_BOSS
};

/// @brief The different types of ships
enum ShipSubtypes {
	P_SHIP_1,    ///< Player Ship type 1
	P_SHIP_2,    ///< Player Ship type 2
	P_SHIP_3,    ///< Player Ship type 3
	P_HELP_SHIP, ///< Player Help Ship

	
	E_SHIP_1, ///< Enemy Ship type 1
	E_SHIP_2, ///< Enemy Ship type 2
	E_SHIP_3, ///< Enemy Ship type 3
	E_BOSS_1, ///< Enemy Boss Ship type 1
	E_BOSS_2, ///< Enemy Boss Ship type 2
	E_BOSS_3, ///< Enemy Boss Ship type 3
	
	
	A_SMALL,  ///< Astroid Ship type small
	A_MEDIUM, ///< Astroid Ship type medium
	A_LARGE,  ///< Astroid Ship type large
	A_XLARGE, ///< Astroid Ship type extra large
	
	
	TOTAL_SHIPS ///< Total Ship types
};


/// @brief The different sizes of the ships
enum Sizes {
	SMALL_SIZE = 30,  ///< (30px) Small Size
	MEDIUM_SIZE = 50, ///< (50px) medium Size
	LARGE_SIZE = 70,  ///< (70px) large Size
	TOTAL_SIZE = 3,   ///< Total Sizes
	
	XLARGE_SIZE = 90 ///< Used only for the astroid ship
};


/// @brief The differnt speeds the ship can go at
enum Speeds {
	STATIONARY_SPEED, ///< (0px per frame) Stationary
	SLOW_SPEED,       ///< (1px per frame) Slow speed
	MEDIUM_SPEED,     ///< (2px per frame) Medium speed
	FAST_SPEED,       ///< (3px per frame) Fast speed
	
	TOTAL_SPEED ///< Total Speeds
};


/// @brief The possible direction on the board
enum Direction {
	UP_DIRECTION,    ///< Up Direction
	DOWN_DIRECTION,  ///< Down Direction
	LEFT_DIRECTION,  ///< Left Direction
	RIGHT_DIRECTION, ///< Right Direction
	
	TOTAL_DIRECTION, ///< Total Direction
	RESET_DIRECTION  ///< Reset Direction, phony
};


/// @brief The Types of powerups
enum PowerupType {
	FIRE_POWERUP,
	HEALTH_POWERUP,
	SHIELD_POWERUP,
	BEAM_POWERUP,
	
	POWERUP_TOTAL
};


enum PowerupSubtype {
	FIRE_POWERUP_1,   ///< Powerup to enable rapid fire
	FIRE_POWERUP_2,   ///< Powerup to enable rapid fire
	FIRE_POWERUP_3,   ///< Powerup to enable rapid fire
	HEALTH_POWERUP_1, ///< Powerup to enable extra health
	HEALTH_POWERUP_2, ///< Powerup to enable extra health
	HEALTH_POWERUP_3, ///< Powerup to enable extra health
	HEALTH_POWERUP_4, ///< Powerup to enable extra health
	SHIELD_POWERUP_1,
	SHIELD_POWERUP_2,
	SHIELD_POWERUP_3,
	BEAM_POWERUP_1,
	BEAM_POWERUP_2,
	BEAM_POWERUP_3,
	
	SUBPOWERUP_TOTAL
};


extern const int FPS; ///< Max frame rate
extern const int MAX_SHIPS; ///< Max amount of ships on the screen at one time
extern const int MIN_SHIPS; ///< Min amount of ships on the screen
extern const int SHIP_SEED; ///< Percent change of new ship
extern const int POWERUP_SPEED;
extern const int LIVES;              ///< How many lives we have
extern const int POWERUP_TIME;
extern const int POWERUP_SPEED;

extern int SCRN_W; ///< Screen width
extern int SCRN_H; ///< Screen Heigth

extern Window* Main_Window;        ///< Our main window
extern Resources* Master_Resource; ///< All of our resources
extern uint32_t Main_Timer;        ///< Main Timer


/// @brief  Program entry point
/// @param  argc \a `int` - Number of command line arguments
/// @param  argv \a `char**` - Command line arguments
/// @return \a `int` - Exit code
int main(int argc, char* argv[]);

void initGraphics(); ///< @brief Initializes graphics

bool checkCollision(SDL_Rect& box1, SDL_Rect& box2);

void quit(int code);

#endif // MAIN_H_INCLUDED