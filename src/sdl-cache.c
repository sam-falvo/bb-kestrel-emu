/*
 * Kestrel 2 Baseline Emulator
 * Release 1p2
 *
 * Copyright (c) 2007 Samuel A. Falvo II
 * All Rights Reserved
 *
 * SDL scanline cache for fixed-frequency VGA emulation.
 */

#include <lib65816/cpu.h>
#include "sdl-cache.h"

#include "SDL.h"

enum
{
    screenWidthInPixels = 800,
    screenHeightInLines = 525,
    bytesPerPixel = 1,
    bytesPerLine = screenWidthInPixels * bytesPerPixel,
    bytesPerScreen = bytesPerLine * screenHeightInLines
};

static word32       line;
static SDL_Surface *screen;
static byte   *     pixels;

/* OPTIMIZATION: mgia.c uses these to "inline" cache_colorExpand() into
 * mgia_update().  This makes video emulation a bit faster, but increases
 * coupling significantly.
 */
       byte         BLACK, WHITE;
       byte   *     beam;

/* For profiling purposes only */
void flip( void ) { SDL_Flip( screen ); }

int
cache_initialize( void )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )    return 0;
    atexit( SDL_Quit );

    screen = SDL_SetVideoMode( 800, 600, 8, SDL_HWSURFACE|SDL_SWSURFACE);
    if( screen == 0 )  return 0;

    BLACK = SDL_MapRGB( screen -> format, 0, 0, 0 );
    WHITE = SDL_MapRGB( screen -> format, 255, 255, 255 );
    pixels = (byte *)screen -> pixels;

    cache_restart();

    return 1;
}

void
cache_restart( void )
{
    extern word32 frames;       /* from mgia.c */
    static word32 reference = 0;
           word32 now;

    flip();
    line = 0;
    beam = pixels;
    memset( beam, 0, bytesPerScreen );

    /* FPS counter */

    if( reference == 0 )
        reference = SDL_GetTicks();

    now = SDL_GetTicks();
    if( ( now - reference ) > 1000 )
    {   
        fprintf( stderr, "\r  FPS: %d   ", frames );
        fprintf( stderr, "CPU Performance: %d%%   ", (10*frames)/6);

        frames = 0; reference = now;
    }
}



void
cache_expunge( void )
{
    /* does nothing for the time being; SDL expunge is performed via the
     * atexit() handler.
     */
}

void
cache_nextLine( word32 origin )
{
    word32 *p, *pe;

    line++;
    beam = pixels + (line*screenWidthInPixels + origin);
}

void
cache_colorExpand( byte b )
{
    if( b & 0x01 ) beam[7] = WHITE;
    if( b & 0x02 ) beam[6] = WHITE;
    if( b & 0x04 ) beam[5] = WHITE;
    if( b & 0x08 ) beam[4] = WHITE;
    if( b & 0x10 ) beam[3] = WHITE;
    if( b & 0x20 ) beam[2] = WHITE;
    if( b & 0x40 ) beam[1] = WHITE;
    if( b & 0x80 ) beam[0] = WHITE;

    beam += 8;      /* pointer arithmetic takes care of scaling */
}

