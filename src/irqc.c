/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * IRQ controller
 */

#include <lib65816/cpu.h>
#include "io.h"
#include "SDL.h"



IO_IRQC irqc;

int
irqc_initialize( void )
{
    irqc.iqea = 0;
    irqc.iqpn = 0;

    return 1;
}

void
irqc_expunge( void )
{
    /* does nothing for the time being */
}

byte
irqc_read( word32 address, word32 timestamp )
{
    /* default: used instead of case 3: to satisfy compiler warning. */
    switch( address & 0x0003 )
    {
        case  0: return LOBYTE(irqc.iqea);
        case  1: return HIBYTE(irqc.iqea);
        case  2: return LOBYTE(irqc.iqpn);
        default: return HIBYTE(irqc.iqpn);
    }
}

void
irqc_write( word32 address, byte b, word32 timestamp )
{
    switch( address & 0x0003 )
    {
        case  0: LOBYTE(irqc.iqea) = b; break;
        case  1: HIBYTE(irqc.iqea) = b; break;
        case  2: LOBYTE(irqc.iqpn) = b; break;
        case  3: HIBYTE(irqc.iqpn) = b; break;
    }

    irqc_update( timestamp );
}

void
irqc_update( word32 timestamp )
{
    if( irqc.iqpn & irqc.iqea )     CPU_addIRQ( 0x0001 );
    else                            CPU_clearIRQ( 0x0001 );
}

void
irqc_raster( void )
{
    irqc.iqpn |= IQVRLI;
    irqc_update( 0 );
}

void
irqc_kimo(void)
{
    irqc.iqpn |= IQKIMO;
    irqc_update(0);
}

