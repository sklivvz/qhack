/*                               -*- Mode: C -*- 
 * sysdep.h -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Sun Dec 29 21:52:36 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:01:09 1997
 * Update Count    : 42
 * Status          : Unknown, Use with caution!
 *
 * (C) Copyright 1996, 1997 by Thomas Biskup.
 * All Rights Reserved.
 *
 * This software may be distributed only for educational, research and
 * other non-proft purposes provided that this copyright notice is left
 * intact.  If you derive a new game from these sources you also are
 * required to give credit to Thomas Biskup for creating them in the first
 * place.  These sources must not be distributed for any fees in excess of
 * $3 (as of January, 1997).
 */

#ifndef __SYSDEP__

#define __SYSDEP__

/*
 * Includes.
 */

#include "config.h"


/*
 * Standard types.
 */

typedef signed char    byte;
typedef unsigned char  ubyte;
typedef short          int16;
typedef unsigned short uint16;
typedef long           int32;
typedef unsigned long  uint32;



/*
 * Special types.
 */

typedef unsigned int   rand_type;
typedef byte           _BOOL;

/* Guess the size for coordinate variables. */
#if (MAP_W > 127) || (MAP_H > 127)
typedef int16          coord;
#else
typedef byte           coord;
#endif

/*
 * Special constants.
 */

#ifndef TRUE
# define TRUE  ((_BOOL) (1 > 0))
#endif

#ifndef FALSE
# define FALSE ((_BOOL) (0 > 1))
#endif



/*
 * Color constants.
 */


enum color_types
{
  C_BLACK, C_BLUE, C_GREEN, C_CYAN, C_RED, C_MAGENTA,
  C_BROWN, C_LIGHT_GRAY, C_DARK_GRAY, C_LIGHT_BLUE,
  C_LIGHT_GREEN, C_LIGHT_CYAN, C_LIGHT_RED, C_LIGHT_MAGENTA,
  C_YELLOW, C_WHITE, MAX_COLOR
};


/*
 * Global functions.
 */

byte rand_byte(byte);
uint16 rand_int(uint16);
uint32 rand_long(uint32);

char getkey(void);

void init_rand(void);
void init_io();
void clean_up_io();
void clear_screen();
void cursor(byte, byte);
void prtchar(byte);
void prtstr(char *, ...);
void stdprtstr(char *, ...);
void update(void);
void clear_to_eol(void);
void set_color(byte);

#endif
