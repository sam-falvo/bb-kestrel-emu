/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 */

#include <stdio.h>
#include <stdlib.h>

#include <lib65816/cpu.h>
#include "ram.h"
#include "io.h"

FILE *hardfile;  /* Used in serbus.c */

static int
k2_initialize( void )
{
    int success;

    if( !ram_initialize() )     goto no_ram;
    if( !rom_initialize() )     goto no_rom;
    if( !io_initialize() )      goto no_io;

    MEM_initialize();
    return 1;

no_io:
    rom_expunge();

no_rom:
    ram_expunge();

no_ram:
    return 0;
}

static void
k2_expunge( void )
{
    io_expunge();
    rom_expunge();
    ram_expunge();
}

    static int
    peekw( int a )
    {
        return (256*ram_read(a+1,0))+ram_read(a,0);
    }

void
EMUL_handleWDM( byte opcode, word32 timestamp )
{
    int i,j;

    switch( opcode )
    {
        case 0:
            fprintf( stderr, "WDM Emulator Exit Requested\n" );
            k2_expunge();
            exit(0);
            break;

        case 1:
        case 2:
            CPU_setTrace( opcode-1 );
            break;

        case 3: /* simple stack trace */
            i = S.W & 0xFFF0;

            for( j = 0; j < 64; j++ )
            {
                if( ( j & 15 ) == 0 ) fprintf( stderr, "\n%04X - ", i );
                if( i == S.W ) fprintf( stderr, "*%02X", MEM_readMem(i,timestamp) );
                else           fprintf( stderr, " %02X", MEM_readMem(i,timestamp) );
                i+=1;
            }
            fprintf( stderr, "\n" );
            break;

        default:
            fprintf( stderr, "Unknown WDM opcode $%02X at $%06X\n", opcode, PC.A );
            break;
    }
}

int
main( int argc, char *argv[] )
{
    CPUEvent_initialize();
    hardfile = fopen("blocks.fb", "rb+");

    if( k2_initialize() )
    {
        CPU_reset();
        CPU_run();
    }

    k2_expunge();

    if(hardfile) fclose(hardfile);
    return 0;
}

