/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Keyboard Input and MOuse controller
 */

#include <lib65816/cpu.h>
#include <lib65816/cpuevent.h>
#include "io.h"
#include "SDL.h"

static IO_KIMO kimo;
extern IO_IRQC irqc;

static CPUEvent kimoCheckEvent;

int
kimo_initialize( void )
{
    memset( &kimo, 0, sizeof( IO_KIMO ) );
    CPUEvent_schedule( &kimoCheckEvent, 400, &kimo_update );
    return 1;
}

void
kimo_expunge( void )
{
    /* does nothing for the time being */
}

byte
kimo_read( word32 address, word32 timestamp )
{
    switch( address & 0x0007 )
    {
        case  0: kimo.kbmost &= 0x7FFF; return LOBYTE(kimo.kbda);
        case  1: kimo.kbmost &= 0x7FFF; return HIBYTE(kimo.kbda);
        case  2: return 0;
        case  3: return 0;
        case  4: return 0;
        case  5: return 0;
        case  6: return LOBYTE(kimo.kbmost);
        case  7: return HIBYTE(kimo.kbmost);
    }
}

void
kimo_write( word32 address, byte b, word32 timestamp )
{
    /* All registers are read-only. */
}

void
kimo_update( word32 timestamp )
{
    SDL_Event evt;

    while( SDL_PollEvent( &evt ) )
    {
        switch( evt.type )
        {
            case SDL_KEYDOWN:
                kimo.kbda = evt.key.keysym.scancode;
                kimo.kbmost |= 0xC000;
                irqc_kimo();
                break;

            case SDL_KEYUP:
                kimo.kbda = evt.key.keysym.scancode;
                kimo.kbmost |= 0x8000;
                kimo.kbmost &= ~0x4000;
                irqc_kimo();
                break;

            case SDL_QUIT:
                /* There simply has to be a better way... */
                exit(0);
                break;
        }
    }

    CPUEvent_schedule( &kimoCheckEvent, 400, &kimo_update );
}

