/*                               -*- Mode: C -*- 
 * game.h -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Mon Dec 30 00:11:37 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 19:19:39 1997
 * Update Count    : 6
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

#ifndef __GAME__

#define __GAME__

/*
 * Global includes.
 */

#include "qhack.h"



/*
 * Global functions.
 */

void play(void);
void modify_dungeon_level(byte);
void redraw(void);

#endif
