/*                               -*- Mode: C -*- 
 * player.h -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Mon Jan  6 11:42:10 1997
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:20:45 1997
 * Update Count    : 19
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

#ifndef __PLAYER__

#define __PLAYER__

/*
 * Global includes.
 */

#include "config.h"



/*
 * Some more constants.
 */

/* For the attributes used by QHack. */
enum attributes
{
  STRENGTH, INTELLIGENCE, DEXTERITY, TOUGHNESS, MANA, MAX_ATTRIBUTE
};

/*
 * Constants for the training skills used by QHack.
 *
 * Note: The first five ones always should be the attribute training skills.
 *       Changing this could break several code parts in the game (especially
 *       in 'init_player').
 */

enum tskills
{
  T_STRENGTH, T_INTELLIGENCE, T_DEXTERITY, T_TOUGHNESS, T_MANA,
  T_HITS, T_POWER, T_2HIT, T_2DAMAGE, T_SEARCHING, MAX_T_SKILL
};



/*
 * The global data structure for the player character.
 */

struct player
{
  /* Name. */
  char name[MAX_PC_NAME_LENGTH + 1];

  /* Attribute scores and maximum attribute scores ever reached. */
  byte attribute[MAX_ATTRIBUTE];
  byte max_attribute[MAX_ATTRIBUTE];

  /* Hitpoints (current and maximum). */
  int16 hits, max_hits;
  
  /* Magical power. */
  int16 power, max_power;

  /* Experience scores. */
  int32 experience;
  int32 tskill_exp[MAX_T_SKILL];

  /* Training adjustment. */
  byte tskill_training[MAX_T_SKILL];
  
  /* Searching skill. */
  byte searching;

  /* Combat related stuff. */
  int16 to_hit, to_damage;
};



/*
 * Global variables.
 */

extern _BOOL update_necessary;



/*
 * Global functions.
 */

byte get_attribute(byte);

void init_player(void);
void update_player_status(void);
void set_attribute(byte, byte);
void adjust_training(void);
void score_exp(int32);

#endif
