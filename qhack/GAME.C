/*                               -*- Mode: C -*- 
 * game.c -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Mon Dec 30 00:11:06 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:49:58 1997
 * Update Count    : 100
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


#include "qhack.h"


/*
 * Local variables.
 */

_BOOL walk_mode;
_BOOL walk_in_room;
int16 walk_steps;


/*
 * Local prototypes.
 */

void update_screen(coord, coord);
void try(byte);
void descend_level(void);
void ascend_level(void);
void open_door(void);
void activate_walk_mode(void);


/*
 * The main function.
 */

void play(void)
{
  coord opx, opy;
  char c;
  
  /*
   * Build the current level.
   *
   * XXXX: Once it is possible to save/restore the map this no longer
   *       must be done if the game was started by restoring a save file.
   */
  d.dl = 0;
  build_map();
  create_population();
  build_monster_map();
  d.visited[0] = TRUE;
  
  /* Initial player position. */
  d.px = d.opx = d.stxu[0];
  d.py = d.opy = d.styu[0];

  /* Initial panel position. */
  d.psx = d.psy = 0;

  /*
   * Standard stuff.
   */
  
  /* Setup the screen. */
  clear_screen();

  do
  {
    /* Print all the new things. */
    update_screen(d.px, d.py);
    update_player_status();

    /* Display the player and center the cursor. */
    map_cursor(d.px, d.py);
    set_color(C_WHITE);
    prtchar('@');
    map_cursor(d.px, d.py);

    /* Refresh the IO stuff. */
    update();

    /* Continue to walk or read a key. */
    if (!walk_mode)
    {
      walk_steps = 0;
      c = getkey();
    }
    
    /* The message line should be cleared in any case. */
    clear_messages();
    
    /* Memorize the old PC position. */
    opx = d.px;
    opy = d.py;
    
    /* Act depending on the last key received. */
    switch (c)
    {
      case 'T':
	adjust_training();
	break;

      case 'o':
	open_door();
	break;

      case '>':
	descend_level();
	break;

      case '<':
	ascend_level();
	/* Quit if necessary. */
	if (d.dl == -1)
	  c = 'Q';
	break;

      case 'R':
	redraw();
	break;

      case 'J':
	activate_walk_mode();
	try(W);
	break;
	
      case 'K':
	activate_walk_mode();
	try(S);
	break;
	
      case 'L':
	activate_walk_mode();
	try(E);
	break;
	
      case 'I':
	activate_walk_mode();
	try(N);
	break;
	
      case 'j':
	if (is_open(d.px - 1, d.py))
	  d.px--;
	break;
	
      case 'l':
	if (is_open(d.px + 1, d.py))
	  d.px++;
	break;

      case 'i':
	if (is_open(d.px, d.py - 1))
	  d.py--;
	break;

      case 'k':
	if (is_open(d.px, d.py + 1))
	  d.py++;
	break;
	
      default:
	break;
    }

    d.opx = opx;
    d.opy = opy;

    /* Remove the player character from the screen. */
    print_tile(opx, opy);
  }
  while (c != 'Q');
}


/*
 * Update the screen based upon the current player position.  Panel scrolling
 * is also handled in this function.
 */

void update_screen(coord x, coord y)
{
  coord sx, sy, px, py, opsx, opsy;

  /* Find the current general section. */
  get_current_section_coordinates(d.px, d.py, &sx, &sy);

  /* Memorize the old panel view. */
  opsx = d.psx;
  opsy = d.psy;
  
  /* Adjust the panel view. */
  while (sx < d.psx)
    d.psx--;
  while (d.psx + 4 < sx)
    d.psx++;
  while (sy < d.psy)
    d.psy--;
  while (d.psy + 1 < sy)
    d.psy++;

  /* Repaint the whole screen map if necessary. */
  if (opsx != d.psx || opsy != d.psy)
    paint_map();

  /* Make the immediate surroundings known. */
  for (px = x - 1; px <= x + 1; px++)
    for (py = y - 1; py <= y + 1; py++)
      know(px, py);

  /* Check whether the PC is in a room or not. */
  get_current_section(d.px, d.py, &sx, &sy);

  /* Make rooms known. */
  if (sx != -1 && sy != -1)
    know_section(sx, sy);
}



/*
 * Try to walk into a given direction.  This is used by the walk-mode when
 * the player tries to run into a given direction.
 *
 * Walk-mode will be deactivated if...
 * ...the surroundings change in a major way (more or less obstacles around
 *    the player compared to the last step).
 * ...an intersection is reached in a tunnel.
 * ...a room is entered or left.
 */

void try(byte dir)
{
  coord sx1, sy1, sx2, sy2;
  byte cn, cw, ce, cs;
  static byte old_cn, old_cw, old_ce, old_cs;
  
  get_current_section(d.px, d.py, &sx1, &sy1);
  
  /* 
   * Check whether running should be stopped.
   */

  if (walk_steps || walk_in_room)
  {
    /* Count the possible ways. */
    cn = (might_be_open(d.px, d.py - 1) && (d.py - 1 != d.opy)) ? 1 : 0;
    cs = (might_be_open(d.px, d.py + 1) && (d.py + 1 != d.opy)) ? 1 : 0;
    cw = (might_be_open(d.px - 1, d.py) && (d.px - 1 != d.opx)) ? 1 : 0;
    ce = (might_be_open(d.px + 1, d.py) && (d.px + 1 != d.opx)) ? 1 : 0;
    
    /* Check... */
    if (walk_in_room)
    {
      /* In rooms we simply check the general look of the surroundings. */
      switch (walk_steps)
      {
	case 0:
	  /* One free step in any case. */
	  break;

	default:
	  /* Check the surroundings. */
	  walk_mode &= (cn == old_cn && cs == old_cs &&
			cw == old_cw && ce == old_ce);
	  
	case 1:
	  /* Memorize the surroundings. */
	  old_cn = cn;
	  old_cs = cs;
	  old_cw = cw;
	  old_ce = ce;
	  break;
      }

      /* Check whether we are still in a room. */
      walk_mode &= (sx1 != -1 && sy1 != -1);

      /* Check for special features. */
      if (walk_steps)
	walk_mode &= (tile_at(d.px, d.py) == FLOOR);
    }
    else
      /* Check for intersections. */
      switch (dir)
      {
	case N:
	  walk_mode &= (cw + ce < 2 && cn + cw < 2 && cn + ce < 2);
	  break;
	case S:
	  walk_mode &= (cw + ce < 2 && cs + cw < 2 && cs + ce < 2);
	  break;
	case W:
	  walk_mode &= (cn + cs < 2 && cw + cs < 2 && cw + cn < 2);
	  break;
	case E:
	  walk_mode &= (cn + cs < 2 && ce + cs < 2 && ce + cn < 2);
	  break;
	  
	default:
	  break;
      }
    
    if (!walk_mode)
      return;
  }

  /*
   * Walk.  This function also manages to walk around corners in a tunnel.
   */

  switch (dir)
  {
    case N:
      if (is_open(d.px, d.py - 1))
	d.py--;
      else if (is_open(d.px - 1, d.py) && d.px - 1 != d.opx)
	d.px--;
      else if (is_open(d.px + 1, d.py) && d.px + 1 != d.opx)
	d.px++;
      else
	walk_mode = FALSE;
      break;
      
    case S:
      if (is_open(d.px, d.py + 1))
	d.py++;
      else if (is_open(d.px - 1, d.py) && d.px - 1 != d.opx)
	d.px--;
      else if (is_open(d.px + 1, d.py) && d.px + 1 != d.opx)
	d.px++;
      else
	walk_mode = FALSE;
      break;
      
    case E:
      if (is_open(d.px + 1, d.py))
	d.px++;
      else if (is_open(d.px, d.py + 1) && d.py + 1 != d.opy)
	d.py++;
      else if (is_open(d.px, d.py - 1) && d.py - 1 != d.opy)
	d.py--;
      else
	walk_mode = FALSE;
      break;
      
    case W:
      if (is_open(d.px - 1, d.py))
	d.px--;
      else if (is_open(d.px, d.py + 1) && d.py + 1 != d.opy)
	d.py++;
      else if (is_open(d.px, d.py - 1) && d.py - 1 != d.opy)
	d.py--;
      else
	walk_mode = FALSE;
      break;
      
    default:
      break;
  }

  /* Find the new section. */
  get_current_section(d.px, d.py, &sx2, &sy2);

  /* Entering/leaving a room will deactivate walk-mode. */
  if (walk_steps)
    walk_mode &= (sx1 == sx2 && sy1 == sy2);

  /* Increase the number of steps actually walked. */
  walk_steps++;
}




/*
 * Redraw the whole screen.
 */

void redraw(void)
{
  clear_messages();
  paint_map();
  update_necessary = TRUE;
  update_player_status();
  update();
}



/*
 * Switch between dungeon levels.
 */

void modify_dungeon_level(byte mod)
{
  /* Modify the actual dungeon level. */
  d.dl += mod;

  /* Build the current dungeon map from the general description. */
  build_map();

  /* Determine monster frequencies for the current dungeon level. */
  initialize_monsters();

  /*
   * If a level is entered for the first time a new monster population
   * will be generated and the player receives a little bit of experience
   * for going where nobody went before (or at least managed to come back).
   */
  if (!d.visited[d.dl])
  {
    create_population();
    d.visited[d.dl] = TRUE;

    /* Score some experience for exploring unknown depths. */
    score_exp(d.dl);
  }

  /* Place monsters in the appropriate positions. */
  build_monster_map();

  /* Paint the new map. */
  paint_map();
}


/*
 * Continue one level downwards.
 */

void descend_level(void)
{
  if (tile_at(d.px, d.py) != STAIR_DOWN)
    you("don't see any stairs leading downwards.");
  else
  {
    modify_dungeon_level(+1);
    d.px = d.stxu[d.dl];
    d.py = d.styu[d.dl];
  }
}



/*
 * Continue one level upwards.
 */

void ascend_level(void)
{
  if (tile_at(d.px, d.py) != STAIR_UP)
    you("don't see any stairs leading upwards.");
  else
  {
    if (d.dl)
    {
      modify_dungeon_level(-1);
      d.px = d.stxd[d.dl];
      d.py = d.styd[d.dl];
    }
    else
      /* Leave the dungeon. */
      d.dl = -1;
  }
}



/*
 * Handle doors.
 */

void open_door(void)
{
  coord tx, ty;

  /* Find the door. */
  get_target(d.px, d.py, &tx, &ty);

  /* Command aborted? */
  if (tx == -1 || ty == -1)
    return;

  /* Check the door. */
  switch (tile_at(tx, ty))
  {
    case OPEN_DOOR:
      message("This door is already open.");
      break;

    case CLOSED_DOOR:
      you("open the door.");
      change_door(tx, ty, OPEN_DOOR);
      break;

    case LOCKED_DOOR:
      message("This door seems to be locked.");
      break;

    default:
      message("Which door?");
      break;
  }
}


/*
 * Activate the walk mode and determine whether we are walking through a
 * room.
 */

void activate_walk_mode(void)
{
  coord x, y;

  /* Activate walking. */
  walk_mode = TRUE;

  /* Check for a room. */
  get_current_section(d.px, d.py, &x, &y);
  walk_in_room = (x != -1 && y != -1);
}
