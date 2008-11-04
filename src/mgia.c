/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Monochrome Graphics Interface Adapter (MGIA)
 */

#include <sys/time.h>

#include <lib65816/cpuevent.h>
#include <lib65816/cpu.h>
#include "io.h"
#include "sdl-cache.h"
#include "SDL.h"


enum
{
    frameSkipCount = 5,
    warpDrive = 0
};



       word32       frames;

static IO_MGIA      mgia;
static int          raster_compare;
static word32       time;

#define YPOS        mgia.CCVRLI
static int          XPOS;

static CPUEvent     scanlineEvent;



int
mgia_initialize( void )
{
    memset( &mgia, 0, sizeof( IO_MGIA ) );
    mgia.CCHZTL = 800;
    mgia.CCVRTL = 525;
    mgia.CCSYWD = 0x1010;
    mgia.VIHZST = 80;
    mgia.VIHZEN = 720;
    mgia.VIVRST = 20;
    mgia.VIVREN = 500;
    raster_compare = 0x3FF;

    cache_initialize();
    XPOS = 0;
    YPOS = 0;
    frames = 0;

    CPUEvent_schedule( &scanlineEvent, 400, &mgia_update );

//  CPU_setUpdatePeriod(400);   /* 400 cycles per scanline */

    return 1;
}

void
mgia_expunge( void )
{
    cache_expunge();
}

byte
mgia_read( word32 address, word32 timestamp )
{
    switch( address & 0x001F )
    {
        case  0: return LOBYTE(mgia.CCHZTL);
        case  1: return HIBYTE(mgia.CCHZTL);
        case  2: return LOBYTE(mgia.CCVRTL);
        case  3: return HIBYTE(mgia.CCVRTL);
        case  4: return LOBYTE(mgia.CCSYWD);
        case  5: return HIBYTE(mgia.CCSYWD);
        case  6: return LOBYTE(mgia.CCVRLI);
        case  7: return HIBYTE(mgia.CCVRLI);
        case  8: return LOBYTE(mgia.VIHZST);
        case  9: return HIBYTE(mgia.VIHZST);
        case 10: return LOBYTE(mgia.VIHZEN);
        case 11: return HIBYTE(mgia.VIHZEN);
        case 12: return LOBYTE(mgia.VIVRST);
        case 13: return HIBYTE(mgia.VIVRST);
        case 14: return LOBYTE(mgia.VIVREN);
        case 15: return HIBYTE(mgia.VIVREN);
        case 16: return LOBYTE(mgia.VIDAPT);
        case 17: return HIBYTE(mgia.VIDAPT);
        case 18: return LOBANK(mgia.VIDAPT);

        default: return 0;
    }
}

void
mgia_write( word32 address, byte b, word32 timestamp )
{
    switch( address & 0x001F )
    {
        case  0: LOBYTE(mgia.CCHZTL) = b; break;
        case  1: HIBYTE(mgia.CCHZTL) = b; break;
        case  2: LOBYTE(mgia.CCVRTL) = b; break;
        case  3: HIBYTE(mgia.CCVRTL) = b; break;
        case  4: LOBYTE(mgia.CCSYWD) = b; break;
        case  5: HIBYTE(mgia.CCSYWD) = b; break;
        case  6: LOBYTE(raster_compare) = b; break;
        case  7: HIBYTE(raster_compare) = b&3; break;
        case  8: LOBYTE(mgia.VIHZST) = b; break;
        case  9: HIBYTE(mgia.VIHZST) = b; break;
        case 10: LOBYTE(mgia.VIHZEN) = b; break;
        case 11: HIBYTE(mgia.VIHZEN) = b; break;
        case 12: LOBYTE(mgia.VIVRST) = b; break;
        case 13: HIBYTE(mgia.VIVRST) = b; break;
        case 14: LOBYTE(mgia.VIVREN) = b; break;
        case 15: HIBYTE(mgia.VIVREN) = b; break;
        case 16: LOBYTE(mgia.VIDAPT) = b; break;
        case 17: HIBYTE(mgia.VIDAPT) = b; break;
        case 18: LOBANK(mgia.VIDAPT) = b; break;

        default: return;
    }
}

void
mgia_update( word32 cpuTime )
{
    int kindOfPixel;
    byte b;
    static int skip = 0;
    struct timeval tv;
    static signed long lastRestart = 0, thisRestart, delta;

    cpuTime = time+400;

    while( cpuTime - time > 0 )
    {
        extern byte   *beam;    /* From sdl-cache.c */
        extern byte   WHITE;

        if( ( YPOS < mgia.VIVRST ) || ( YPOS > mgia.VIVREN ) )
            goto skip;

        if( ( XPOS < mgia.VIHZST ) || ( XPOS > mgia.VIHZEN ) )
            goto skip;

        /* The code herein is a hand-inlined version of
         * cache_colorExpand().
         */

//      cache_colorExpand( MEM_readMem( mgia.VIDAPT++, cpuTime ) );

        if( skip == 0 )
        {
            b = MEM_readMem( mgia.VIDAPT++, cpuTime );

            if( b & 0x01 ) beam[7] = WHITE;
            if( b & 0x02 ) beam[6] = WHITE;
            if( b & 0x04 ) beam[5] = WHITE;
            if( b & 0x08 ) beam[4] = WHITE;
            if( b & 0x10 ) beam[3] = WHITE;
            if( b & 0x20 ) beam[2] = WHITE;
            if( b & 0x40 ) beam[1] = WHITE;
            if( b & 0x80 ) beam[0] = WHITE;
        }

skip:
        beam += 8;      /* pointer arithmetic takes care of scaling */
        XPOS += 8;
        if( XPOS >= mgia.CCHZTL )
        {
            XPOS -= mgia.CCHZTL;
            cache_nextLine( XPOS );

            YPOS++;

            if( YPOS == mgia.CCVRTL )
            {
                frames++;
                YPOS = 0;

                if( skip == frameSkipCount )
                {
                    cache_restart();
                    skip = 0;
                }
                else
                    skip++;


                if( warpDrive == 0 )
                {
                    gettimeofday( &tv, 0 );
                    thisRestart = ( tv.tv_sec * 1000000 ) + tv.tv_usec;
                    if( lastRestart == 0 ) lastRestart = thisRestart;
                    delta = thisRestart - lastRestart;
                    if( delta < 16666 ) usleep( 16666-delta );
                    lastRestart = thisRestart;
                }
            }

            if( YPOS == raster_compare ) { irqc_raster(); }
        }

        time += 4;
    }

    CPUEvent_schedule( &scanlineEvent, 400, &mgia_update );
}

