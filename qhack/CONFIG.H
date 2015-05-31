/*                               -*- Mode: C -*- 
 * config.h -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Sun Dec 29 21:57:08 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:18:57 1997
 * Update Count    : 38
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

#ifndef __CONFIG__

#define __CONFIG__

/*
 * Section 0: Guess the target system.
 *
 * This section tries to do a little magic and attempts to guess your
 * operating system through checking a number of usually defined compiler
 * constants.
 *
 * The goal is to be able to compile the complete game without having to
 * change any part of the sources -- no matter what the target system is
 * (as long as it is supported naturally :-)
 *
 * If this doesn't work for you you need to define the required constant by
 * hand and will have to remove all the other constants.
 *
 * For DOS we support DJGPP (it sets the '__GO32__' flag).
 * For Linux we support the GCC (it sets the 'linux' flag).
 */

#if defined(__GO32__)
# define __TS_DOS__
#elif defined(linux)
# define __TS_LINUX__
#else
# define __WINDOWS__
#endif


/*
 * Section I: General constants for meta information.
 *
 * A note on version information: the major version number is only increased
 * if a specific milestone in the game developement is reached.  The minor
 * version is increased very time I (Thomas Biskup) release a new QHack source
 * distribution.
 */

#define MAJOR_VERSION 0
#define MINOR_VERSION 1


/*
 * Section II: Constants governing the dungeon size.
 *
 * Many things are limited in scope because QHack uses 'byte' variables
 * in many places.  Changing this would be some work.  This limit was
 * arbitrarly introduced to keep the game simple.
 *
 * Note that there can't be more than 127 sections in either direction
 * right now -- this should nonetheless be sufficient.
 */

/* Screen width. */
#define SCREEN_W 80

/* Screen height. */
#define SCREEN_H 25

/* Section width. */
#define SECT_W 15

/* Section height. */
#define SECT_H 12

/* Number of sections in E/W direction. */
#define NSECT_W 7

/* Number of sections in N/S direction. */
#define NSECT_H 4

/* Total number of sections per level. */
#define SECT_NUMBER (NSECT_H * NSECT_W)

/* Map width. */
#define MAP_W (SECT_W * NSECT_W)

/* Map height (must be divisable by NSECT_H). */
#define MAP_H (SECT_H * NSECT_H)

/* Visible map width. */
#define VMAP_W 80

/* Visible map height. */
#define VMAP_H 22

/* Maximum width for a room. */
#define ROOM_W (SECT_W - 2)

/* Maximum height for a room. */
#define ROOM_H (SECT_H - 2)

/* Bitmap width for the knowledge map. */
#define MAP_BIT_W ((MAP_W >> 3) + 1)

/* Maximum number of monsters per level. */
#define MONSTERS_PER_LEVEL 64

/* The initial number of monsters on a new level. */
#define INITIAL_MONSTER_NUMBER 24

/*
 * Dungeon depth.
 *
 * Note: changing this variable is not a good idea since many parts of this
 * code depend on a dungeon with 26 levels.
 */

#define MAX_DUNGEON_LEVEL 26



/*
 * Section III: Some constants for the player character.
 */

/* The length of the name of your character. */
#define MAX_PC_NAME_LENGTH 20

/* The number of training units you can distribute. */
#define TUNITS 100

#endif
