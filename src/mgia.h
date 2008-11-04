#ifndef K2_MGIA_H
#define K2_MGIA_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

typedef struct IO_MGIA IO_MGIA;

#define IOBASE_MGIA     0xFF0000
#define IOMASK_MGIA     0xFFFF00

struct IO_MGIA
{
    /* The CRTC section */

    word16  CCHZTL;     /* Horizontal Total; typically 800 for VGA */
    word16  CCVRTL;     /* Vertical Total; typically 525 for VGA */
    word16  CCSYWD;     /* H and V Sync Widths */
    word16  CCVRLI;     /* Vertical Line Compare (for raster interrupts) */

    /* The Display Engine section */

    word16  VIHZST;     /* Video Horizontal Start */
    word16  VIHZEN;     /* Video Horizontal End */
    word16  VIVRST;     /* Video Vertical Start */
    word16  VIVREN;     /* Video Vertical End */

    word32  VIDAPT;     /* Video Data Pointer */
    word16  unused0;
    word16  unused1;

    word16  unused2;
    word16  unused3;
    word16  unused4;
    word16  unused5;

    /* The following are *INTERNAL* emulator-specific state */

    /* vidast = Video Data Start.  Whenever an overt change to the
     * VIDAPT register occurs, this memento gets updated.  Whenever
     * vertical refresh occurs, this register gets updated.  Anything
     * else, however, does NOT update this register.  The result is that
     * vidast <= VIDAPT < vidast+38399 at all times.  Therefore, it's now
     * possible to detect when a write to "video RAM" occurs, and thus,
     * we can now start optimizing the MGIA emulation core performance!
     */
    word32  vidast;
};



int
mgia_initialize( void );

void
mgia_expunge( void );

byte
mgia_read( word32 address, word32 timestamp );

void
mgia_write( word32 address, byte b, word32 timestamp );

void
mgia_update( word32 timestamp );

void
mgia_trap_write( word32 address, byte b, word32 timestamp );

#endif

