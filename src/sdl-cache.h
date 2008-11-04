#ifndef SDL_CACHE_H
#define SDL_CACHE_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p2
 *
 * Copyright (c) 2007 Samuel A. Falvo II
 * All Rights Reserved
 */

#include <lib65816/cpu.h>

int cache_initialize( void );
void cache_restart( void );
void cache_expunge( void );
void cache_nextLine( word32 origin );
void cache_colorExpand( byte b );

#endif

