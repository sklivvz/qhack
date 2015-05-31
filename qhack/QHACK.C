/*                               -*- Mode: C -*- 
 * qhack.c -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Sun Dec 29 22:55:15 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:25:59 1997
 * Update Count    : 61
 * Status          : Unknown, Use with caution!
 *
 * On the following times I worked on this game:
 *  12/29/96, 12/30/96 [9:45pm-1:30am, 10:35am-11:11am, 1:25pm-2:55pm]
 *                     [5:18pm-6:55pm]
 *  12/31/96           [1:25pm-3:16pm]
 *  01/06/97           [11:28am-11:55am, 10:37pm-10:56pm]
 *  01/08/97           [11:25pm-1:04am]
 *  09/01/97           [6:40pm-10:55pm]
 *
 * Total time so far: 959 minutes = 15 hours, 59 minutes
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

/*
 * Includes.
 */

#include <stdio.h>

#include "qhack.h"



/*
 * Local prototypes.
 */

void init_screen(void);


/*
 * Local functions.
 */


/*
 * Draw a title screen.
 */

void init_screen(void)
{
  char s[80];
  
  clear_screen();
  strcpy(s, string("-----====<<<<< QHack %d.%d >>>>>====-----"
		   , MAJOR_VERSION, MINOR_VERSION));
  cursor((80 - strlen(s)) >> 1, 3);
  prtstr("%s", s);
  cursor(16, 5);
  prtstr("(The Quickest Roguelike Gaming Hack on the Net)");
  cursor(19, 8);
  prtstr("(C) Copyright 1996, 1997 by Thomas Biskup.");
  cursor(30, 9);
  prtstr("All Rights Reserved.");
  cursor(0, 24);
  prtstr("Email comments/suggestions/bug reports to ............... rpg@saranxis.ruhr.de");
  getkey();
}


/*
 * The main function.
 */

int main(int argc, char **argv)
{
  /* Print startup message. */
  printf("\nQuickHack Version 0.1\n");
  printf("(C) Copyright 1996, 1997 by Thomas Biskup.\n\n");
  printf("Current dungeon size: %ld.\n"
	 , (long int) sizeof(struct dungeon_complex));
  printf("Current monster size: %ld.\n"
	 , (long int) sizeof(struct monster_struct));
  printf("Current section size: %ld.\n"
	 , (long int) sizeof(struct section));
  printf("\n");
  
  if (argc > 1)
    return 0;
  
  stdprtstr("Setting up the game...");
  
  /* Initialize everything. */
  stdprtstr(".");
  init_rand();
  stdprtstr(".");
  init_player();
  stdprtstr(".");
  init_monsters();
  stdprtstr(".");
  init_dungeon();
  stdprtstr(".");
  init_io();
  init_screen();
  
  /* Play the game. */
  play();

  /* Clean up. */
  clean_up_io();
  
  /* Be done. */
  return 0;
}




