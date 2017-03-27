/* 
 * Copyright (c) 2017 The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * =============================================================================
 * @author 			Aryan Gupta
 * @project 		
 * @title 			
 * @date			2017-03-25 (YYYY-MM-DD)
 * @fversion		1.0.0
 * @description 	
 * =============================================================================
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

extern long long MEM_LEAK_COUNTER;

void pre_delete(const char *file, int line);
void* operator new(size_t ptr, const char *file, int line);
void* operator new[](size_t ptr, const char *file, int line);

#define new 	new(__FILE__, __LINE__)
#define delete  pre_delete(__FILE__, __LINE__),delete

#endif