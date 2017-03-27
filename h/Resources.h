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

#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED_

#include <SDL_Mixer.h>

extern const char* SPRITE_RES[];
extern const char* EXPLOSION_RES;
extern const char* BACKGROUND_IMAGE; ///< Background location
extern const char* MAIN_FONT;        ///< Font location
extern const char* LIFE_IMAGE;


/// @brief Stores all of our resources
/**
	This Struct was implemented so the program doesnt have to
		- load the image, then ...
		- convert to surface, then ...
		- convert to renderer
	
	everytime a object is created. Which was causeing major lags. \n
	After this struct is created, this stuct contains all of the textures we
	will need. So there is only one bullet texture, one ship texture. However,
	whenever we create the class, it uses the texture as a <em>Shared Resource</em>. 
	That is why we never destroy our textures in our class destructors. Doing so
	would delete the image and we cant access the texture when creating another
	object.
 */
struct Resources {
	///@brief Default Constructor
	/// Loads all the images and resources
	Resources();
	
	/// @brief Default Destructor
	/// Frees all of the textures and surfaces used in the game
	~Resources();
	
	SDL_Texture** spriteSheets;
	SDL_Texture*  explosion;
	
	SDL_Texture*  background;  ///< Stores our background texture
	SDL_Texture*  healthHeart; ///< Stores out texture for our heart
	TTF_Font*     mainFont;    ///< Stores the main font we will be using
	TTF_Font*     briefFont;
	TTF_Font*     titleFont;
	
	/// SPRITE SHEET 0
	SDL_Rect* clip_enemyShips;
	SDL_Rect* clip_astroids;
	SDL_Rect* clip_powerup;
	SDL_Rect  clip_shield;
	
	/// SPRITE SHEET 1
	SDL_Rect* clip_playerShips;
	SDL_Rect* clip_bullets;
	
	Mix_Music* music;
	Mix_Chunk* fire;
private:
	SDL_Texture* load(const char* file);
};

#endif