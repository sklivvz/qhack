/*                               -*- Mode: C -*- 
 * qhack.h -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Mon Dec 30 00:25:24 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 20:50:10 1997
 * Update Count    : 30
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

#ifndef __QHACK__

#define __QHACK__

/*
 * Includes.
 */

#include "config.h"
#include "dungeon.h"
#include "error.h"
#include "game.h"
#include "misc.h"
#include "monster.h"
#include "player.h"
#include "sysdep.h"



/*
 * One section of a level: each dungeon map consists of NSECT_W * NSECT_H
 * sections (see config.g).  A section either contains one room with up
 * to four doors or a tunnel intersection.
 *
 * NOTE: The use of a sectioning approach prevents things like digging, etc.
 */

struct section
{
  /* Room available? */
  _BOOL exists;

  /* Room coordinates. */
  coord rx1, rx2, ry1, ry2;

  /* Door positions. */
  coord dx[4], dy[4];

  /* Door types. */
  byte dt[4];
};


/*
 * QHack uses one large structure for the complete dungeon.  There are
 * no pointers or other fancy stuff involved since this game should be
 * simple and easy to use.
 *
 * Naturally this prevents some useful things and is not the way a big
 * roguelike game should be written (you sacrifice too much in flexibility),
 * but since it's easy to use I headed into this direction.
 */

struct dungeon_complex
{
  /* The current level number. */
  byte dl;

  /* NSECT_W * NSECT_H sections for each level. */
  struct section s[MAX_DUNGEON_LEVEL][NSECT_W][NSECT_H];

  /* Coordinates for stairways. */
  coord stxu[MAX_DUNGEON_LEVEL];
  coord styu[MAX_DUNGEON_LEVEL];
  coord stxd[MAX_DUNGEON_LEVEL - 1];
  coord styd[MAX_DUNGEON_LEVEL - 1];

  /* Player coordinates. */
  coord px, py;

  /* Last player coordinates. */
  coord opx, opy;

  /* The knowledge map. */
  byte known[MAX_DUNGEON_LEVEL][MAP_BIT_W][MAP_H];

  /* The panel positions. */
  coord psx, psy;

  /* Level was already visited? */
  _BOOL visited[MAX_DUNGEON_LEVEL];

  /* The player data. */
  struct player pc;
};


/* The global dungeon structure. */
extern struct dungeon_complex d;

#endif


