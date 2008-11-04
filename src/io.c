/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 */

#include <stdio.h>

#include <lib65816/cpu.h>
#include "io.h"

int
io_initialize( void )
{
    int success;

    success = mgia_initialize();
    if( !success ) fprintf( stderr, "MGIA failed to initialize\n" );

    success = kimo_initialize();
    if( !success ) fprintf( stderr, "KIMO failed to initialize\n" );

    success = timers_initialize();
    if( !success ) fprintf( stderr, "TIMERS failed to initialize\n" );

    success = serbus_initialize();
    if( !success ) fprintf( stderr, "SERBUS failed to initialize\n" );

    return success;
}


void
io_expunge( void )
{
    mgia_expunge();
}


byte
io_read( word32 address, word32 timestamp )
{
    if( ( address & IOMASK_MGIA ) == IOBASE_MGIA )
        return mgia_read( address, timestamp );

    if( ( address & IOMASK_IRQC ) == IOBASE_IRQC )
        return irqc_read( address, timestamp );

    if( ( address & IOMASK_KIMO ) == IOBASE_KIMO )
        return kimo_read( address, timestamp );

    if( ( address & IOMASK_TIMERS ) == IOBASE_TIMERS )
        return timers_read( address, timestamp );

    if( ( address & IOMASK_SERBUS ) == IOBASE_SERBUS )
        return serbus_read( address, timestamp );

    return 0xFF;
}


void
io_write( word32 address, byte b, word32 timestamp )
{
    if( ( address & IOMASK_MGIA ) == IOBASE_MGIA )
        mgia_write( address, b, timestamp );

    else if( ( address & IOMASK_IRQC ) == IOBASE_IRQC )
        irqc_write( address, b, timestamp );

    else if( ( address & IOMASK_KIMO ) == IOBASE_KIMO )
        kimo_write( address, b, timestamp );

    else if( ( address & IOMASK_TIMERS ) == IOBASE_TIMERS )
        timers_write( address, b, timestamp );

    else if( ( address & IOMASK_SERBUS ) == IOBASE_SERBUS )
        serbus_write( address, b, timestamp );
}


    static void
    fps_update( void )
    {
        extern word32 frames;       /* from mgia.c */
        static word32 reference = 0;
               word32 now;

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
EMUL_hardwareUpdate( word32 timestamp )
{
    fps_update();

    mgia_update( timestamp );
    kimo_update( timestamp );
    irqc_update( timestamp );   /* update this last, to make sure we get all IRQs */
}

