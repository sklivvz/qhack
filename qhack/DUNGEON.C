/*                               -*- Mode: C -*- 
 * dungeon.c -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Sun Dec 29 21:49:08 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:00:14 1997
 * Update Count    : 143
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

#include <stdio.h>

#include "qhack.h"


/*
 * Local variables.
 */

static byte existence_chance;
static byte map[MAP_W][MAP_H];



/*
 * The complete dungeon structure
 */

struct dungeon_complex d;


/*
 * Prototypes.
 */

void create_complete_dungeon(void);
void dig_level(void);
void dig_section(coord, coord);
void dig_stairs(void);
void connect_sections(coord, coord, coord, coord, byte);
void get_random_section(coord *, coord *);
void set_color_for_tile(char);

byte rand_door(void);
_BOOL dir_possible(coord, coord, byte);




/*
 * Define all the basic dungeon structures and create the complete dungeon
 * map.
 */

void init_dungeon(void)
{
  create_complete_dungeon();
}



/*
 * Create all the levels in the dungeon.
 */

void create_complete_dungeon(void)
{
  for (d.dl = 0; d.dl < MAX_DUNGEON_LEVEL; d.dl++)
  {
    coord x, y;

    /* Basic initialization. */

    /* Nothing is known about the dungeon at this point. */
    for (x = 0; x < MAP_W; x++)
      for (y = 0; y < MAP_H; y++)
	set_knowledge(x, y, 0);

    /* Create the current level map. */
    dig_level();

    /* Note the current level as unvisited. */
    d.visited[d.dl] = FALSE;
  }
}



/*
 * Create one single dungeon level.
 */

void dig_level(void)
{
  coord w, h, sectx[SECT_NUMBER], secty[SECT_NUMBER];
  int16 i, index[SECT_NUMBER];

  /*
   * Determine a random order for the section generation.
   */

  /* Initial order. */
  i = 0;
  for (w = 0; w < NSECT_W; w++)
    for (h = 0; h < NSECT_H; h++)
    {
      index[i] = i;
      sectx[i] = w;
      secty[i] = h;
      i++;
    }

  /* Randomly shuffle the initial order. */
  for (i = 0; i < SECT_NUMBER; i++)
  {
    int16 j, k, dummy;

    j = rand_int(SECT_NUMBER);
    k = rand_int(SECT_NUMBER);

    dummy = index[j];
    index[j] = index[k];
    index[k] = dummy;
  }
  
  /*
   * Create each section separately.
   */
  
  /* Initially there is a 30% chance for rooms to be non-existant. */
  existence_chance = 70;

  /* Dig each section. */
  for (i = 0; i < SECT_NUMBER; i++)
    dig_section(sectx[index[i]], secty[index[i]]);

  /* Build some stairs. */
  dig_stairs();
}



/*
 * Dig one section for the dungeon.
 *
 * The game assumes that one section is SECT_W * SECT_H tiles in size.
 * A section can contain a room with up to four doors or simply be an
 * intersction of several passages.
 *
 */

void dig_section(coord x, coord y)
{
  if (rand_byte(100) + 1 >= existence_chance)
  {
    /* No room here. */
    d.s[d.dl][x][y].exists = FALSE;

    /* Decrease the chance for further empty rooms. */
    existence_chance += 3;
  }
  else
  {
    byte dir;

    /* Yeah :-) ! */
    d.s[d.dl][x][y].exists = TRUE;

    /*
     * Dig a room.
     *
     * Rooms are at least 4x4 tiles in size.
     */
    
    do
    {
      d.s[d.dl][x][y].rx1 = x * SECT_W + rand_byte(3) + 1;
      d.s[d.dl][x][y].ry1 = y * SECT_H + rand_byte(3) + 1;
      d.s[d.dl][x][y].rx2 = (x + 1) * SECT_W - rand_byte(3) - 2;
      d.s[d.dl][x][y].ry2 = (y + 1) * SECT_H - rand_byte(3) - 2;
    }
    while (d.s[d.dl][x][y].rx2 - d.s[d.dl][x][y].rx1
	   < 3 ||
	   d.s[d.dl][x][y].ry2 - d.s[d.dl][x][y].ry1
	   < 3);

    /*
     * Create doors.
     *
     * XXX: At some point it would be nice to create doors only for
     *      some directions to make the dungeon less regular.
     */
    
    for (dir = N; dir <= E; dir++)
      if (dir_possible(x, y, dir))
      {
	switch (dir)
	{
	  case N:
	    d.s[d.dl][x][y].dx[dir] = d.s[d.dl][x][y].rx1 +
	      rand_byte(room_width(x, y) - 1) + 1;
	    d.s[d.dl][x][y].dy[dir] = d.s[d.dl][x][y].ry1;
	    break;

	  case S:
	    d.s[d.dl][x][y].dx[dir] = d.s[d.dl][x][y].rx1 +
	      rand_byte(room_width(x, y) - 1) + 1;
	    d.s[d.dl][x][y].dy[dir] = d.s[d.dl][x][y].ry2;
	    break;
	    
	  case E:
	    d.s[d.dl][x][y].dy[dir] = d.s[d.dl][x][y].ry1 +
	      rand_byte(room_height(x, y) - 1) + 1;
	    d.s[d.dl][x][y].dx[dir] = d.s[d.dl][x][y].rx2;
	    break;
	    
	  case W:
	    d.s[d.dl][x][y].dy[dir] = d.s[d.dl][x][y].ry1 +
	      rand_byte(room_height(x, y) - 1) + 1;
	    d.s[d.dl][x][y].dx[dir] = d.s[d.dl][x][y].rx1;
	    break;
	    
	  default:
	    break;
	}
	d.s[d.dl][x][y].dt[dir] = rand_door();
      }
      else
	d.s[d.dl][x][y].dt[dir] = NO_DOOR;
  }
}



/*
 * Calculate the room width for a specific room section at (x, y).
 */

int room_width(coord x, coord y)
{
  return (d.s[d.dl][x][y].rx2 - d.s[d.dl][x][y].rx1 - 1);
}



/*
 * Calculate the room height for a specific room section at (x, y).
 */

int room_height(coord x, coord y)
{
  return (d.s[d.dl][x][y].ry2 - d.s[d.dl][x][y].ry1 - 1);
}



/*
 * Determine a random door type.
 */

byte rand_door(void)
{
  byte roll = rand_byte(100);

  if (roll < 75)
    return OPEN_DOOR;
  else if (roll < 90)
    return CLOSED_DOOR;

  return LOCKED_DOOR;
}



/*
 * Build a map for the current dungeon level.
 *
 * This function is very important for QHack in general.  Levels are only
 * stored by their section descriptions.  The actual map is created when the
 * level is entered.  The positive thing about this is that it requires much
 * less space to save a level in this way (since you only need the outline
 * descriptions).  The negative thing is that tunneling and other additions
 * are not possible since the level desciptions have now way of recording
 * them.
 */

void build_map(void)
{
  coord x, y, sx, sy;
  byte dir;
  
  /* Basic initialization. */
  for (x = 0; x < MAP_W; x++)
    for (y = 0; y < MAP_H; y++)
      map[x][y] = ROCK;

  /* Build each section. */
  for (sx = 0; sx < NSECT_W; sx++)
    for (sy = 0; sy < NSECT_H; sy++)
    {
      /* Handle each section. */
      if (d.s[d.dl][sx][sy].exists)
      {
	/* Paint existing room. */
	for (x = d.s[d.dl][sx][sy].rx1 + 1;
	     x < d.s[d.dl][sx][sy].rx2;
	     x++)
	  for (y = d.s[d.dl][sx][sy].ry1 + 1;
	       y < d.s[d.dl][sx][sy].ry2;
	       y++)
	    map[x][y] = FLOOR;

	/* Paint doors. */
	for (dir = N; dir <= E; dir++)
	  if (d.s[d.dl][sx][sy].dt[dir] != NO_DOOR)
	    map[d.s[d.dl][sx][sy].dx[dir]][d.s[d.dl][sx][sy].dy[dir]]
	      = d.s[d.dl][sx][sy].dt[dir];
      }
    }


  /* Connect each section. */
  for (sx = 0; sx < NSECT_W; sx++)
    for (sy = 0; sy < NSECT_H; sy++)
    {
      if (dir_possible(sx, sy, E))
	connect_sections(sx, sy, sx + 1, sy, E);
      if (dir_possible(sx, sy, S))
	connect_sections(sx, sy, sx, sy + 1, S);
    }

  /* Place the stairways. */
  map[d.stxu[d.dl]][d.styu[d.dl]] = STAIR_UP;
  if (d.dl < MAX_DUNGEON_LEVEL - 1)
    map[d.stxd[d.dl]][d.styd[d.dl]] = STAIR_DOWN;
}




/*
 * Connect two sections of a level.
 */

void connect_sections(coord sx1, coord sy1, coord sx2, coord sy2, byte dir)
{
  coord cx1, cy1, cx2, cy2, mx, my, x, y;

  /* Get the start coordinates from section #1. */ 
  if (d.s[d.dl][sx1][sy1].exists)
  {
    if (dir == S)
    {
      cx1 = d.s[d.dl][sx1][sy1].dx[S];
      cy1 = d.s[d.dl][sx1][sy1].dy[S];
    }
    else
    {
      cx1 = d.s[d.dl][sx1][sy1].dx[E];
      cy1 = d.s[d.dl][sx1][sy1].dy[E];
    }
  }
  else
  {
    cx1 = sx1 * SECT_W + (SECT_W >> 1);
    cy1 = sy1 * SECT_H + (SECT_H >> 1);
  }

  /* Get the end coordinates from section #2. */ 
  if (d.s[d.dl][sx2][sy2].exists)
  {
    if (dir == S)
    {
      cx2 = d.s[d.dl][sx2][sy2].dx[N];
      cy2 = d.s[d.dl][sx2][sy2].dy[N];
    }
    else
    {
      cx2 = d.s[d.dl][sx2][sy2].dx[W];
      cy2 = d.s[d.dl][sx2][sy2].dy[W];
    }
  }
  else
  {
    cx2 = sx2 * SECT_W + (SECT_W >> 1);
    cy2 = sy2 * SECT_H + (SECT_H >> 1);
  }

  /* Get the middle of the section. */
  mx = (cx1 + cx2) >> 1;
  my = (cy1 + cy2) >> 1;

  /* Draw the tunnel. */
  x = cx1;
  y = cy1;
  if (dir == E)
  {
    /* Part #1. */
    while (x < mx)
    {
      if (map[x][y] == ROCK)
	map[x][y] = FLOOR;
      x++;
    }

    /* Part #2. */
    if (y < cy2)
      while (y < cy2)
      {
	if (map[x][y] == ROCK)
	  map[x][y] = FLOOR;
	y++;
      }
    else
      while (y > cy2)
      {
	if (map[x][y] == ROCK)
	  map[x][y] = FLOOR;
	y--;
      }
    
    /* Part #3. */
    while (x < cx2)
    {
      if (map[x][y] == ROCK)
	map[x][y] = FLOOR;
      x++;
    }
    if (map[x][y] == ROCK)
      map[x][y] = FLOOR;
  }
  else
  {
    /* Part #1. */
    while (y < my)
    {
      if (map[x][y] == ROCK)
	map[x][y] = FLOOR;
      y++;
    }
    if (map[x][y] == ROCK)
      map[x][y] = FLOOR;

    /* Part #2. */
    if (x < cx2)
      while (x < cx2)
      {
	if (map[x][y] == ROCK)
	  map[x][y] = FLOOR;
	x++;
      }
    else
      while (x > cx2)
      {
	if (map[x][y] == ROCK)
	  map[x][y] = FLOOR;
	x--;
      }

    /* Part #3. */
    while (y < cy2)
    {
      if (map[x][y] == ROCK)
	map[x][y] = FLOOR;
      y++;
    }
  }
  if (map[x][y] == ROCK)
    map[x][y] = FLOOR;
}



/*
 * Determine whether a given section is set on a border.
 */

_BOOL dir_possible(coord x, coord y, byte dir)
{
  return ((dir == N && y > 0) ||
	  (dir == S && y < NSECT_H - 1) ||
	  (dir == W && x > 0) ||
	  (dir == E && x < NSECT_W - 1));
}



/*
 * Each level requires at least one stair!
 */

void dig_stairs(void)
{
  coord sx, sy, x, y;
  
  /* Dig stairs upwards. */

  /* Find a section. */
  get_random_section(&sx, &sy);
  
  d.stxu[d.dl] = d.s[d.dl][sx][sy].rx1 +
    rand_byte(room_width(sx, sy) - 1) + 1;
  d.styu[d.dl] = d.s[d.dl][sx][sy].ry1 +
    rand_byte(room_height(sx, sy) - 1) + 1;

  /* Dig stairs downwards. */
  if (d.dl < MAX_DUNGEON_LEVEL - 1)
  {
    /* Find a section. */
    get_random_section(&sx, &sy);

    /* Find a good location. */
    do
    {
      x = d.s[d.dl][sx][sy].rx1 + rand_byte(room_width(sx, sy) - 1) + 1;
      y = d.s[d.dl][sx][sy].ry1 + rand_byte(room_height(sx, sy) - 1) + 1;
    }
    while (d.dl && x == d.stxu[d.dl] && y == d.styu[d.dl]);

    /* Place the stairway. */
    d.stxd[d.dl] = x;
    d.styd[d.dl] = y;
  }
}



/*
 * Find a random section on the current dungeon level.
 */

void get_random_section(coord *sx, coord *sy)
{
  do
  {
    *sx = rand_int(NSECT_W);
    *sy = rand_int(NSECT_H);
  }
  while (!d.s[d.dl][*sx][*sy].exists);
}



/*
 * Check whether a given position is accessible.
 */

_BOOL is_open(coord x, coord y)
{
  switch (map[x][y])
  {
    case ROCK:
    case LOCKED_DOOR:
    case CLOSED_DOOR:
      return FALSE;

    default:
      return TRUE;
  }
}



/*
 * Check whether a given position might be accessible.
 */

_BOOL might_be_open(coord x, coord y)
{
  switch (map[x][y])
  {
    case ROCK:
      return FALSE;

    default:
      return TRUE;
  }
}



/*
 * Memorize a new location.
 *
 * This has two effects: the position is known to you (and will be for
 * the rest of the game barring magical effects) and it will be displayed
 * on the screen.
 */

void know(coord x, coord y)
{
  if (is_known(x, y))
    return;

  set_knowledge(x, y, 1);
  print_tile(x, y);
}



/*
 * This function prints the tile at position (x, y) on the screen.
 * If necessary the map will be scrolled in 'map_cursor'.
 */

void print_tile(coord x, coord y)
{
  map_cursor(x, y);
  print_tile_at_position(x, y);
}



/*
 * Print the tile at position (x, y) to the current screen position.
 *
 * NOTE: Monsters and items also need to be considered in this function.
 */

void print_tile_at_position(coord x, coord y)
{
  if (x <  0 || y < 0 || x > MAP_W || y > MAP_H || !is_known(x, y))
  {
    set_color(C_BLACK);
    prtchar(' ');
  }
  else
  {
    if (is_monster_at(x, y) && los(x, y))
    {
      struct monster *m = get_monster_at(x, y);

      set_color(monster_color(m->midx));
      prtchar(monster_tile(m->midx));
    }
    else
    {
      set_color_for_tile(map[x][y]);
      prtchar(map[x][y]);
    }
  }
}



/*
 * Makes a complete scetion known.
 *
 * This function is usually called when a room is entered.
 */

void know_section(coord sx, coord sy)
{
  coord x, y;

  for (y = d.s[d.dl][sx][sy].ry1;
       y <= d.s[d.dl][sx][sy].ry2;
       y++)
    for (x = d.s[d.dl][sx][sy].rx1;
	 x <= d.s[d.dl][sx][sy].rx2;
	 x++)
      know(x, y);
}



/*
 * Calculate the current section coordinates.
 */

void get_current_section_coordinates(coord px, coord py, coord *sx, coord *sy)
{
  *sx = px / SECT_W;
  *sy = py / SECT_H;
}




/*
 * Calculate the current section coordinates *if* the current section
 * contains a room and the given position is in that room.
 */

void get_current_section(coord px, coord py, coord *sx, coord *sy)
{
  get_current_section_coordinates(px, py, sx, sy);
  
  if (!d.s[d.dl][*sx][*sy].exists ||
      px < d.s[d.dl][*sx][*sy].rx1 ||
      px > d.s[d.dl][*sx][*sy].rx2 ||
      py < d.s[d.dl][*sx][*sy].ry1 ||
      py > d.s[d.dl][*sx][*sy].ry2)
    *sx = *sy = -1;
}



/*
 * Return the tile at position (x, y).
 */

char tile_at(coord x, coord y)
{
  return map[x][y];
}



/*
 * Completely redraw the map.  Take also care of the visible panel area.
 *
 * Note: it's important that 'map_cursor' is not called in this function
 * since 'map_cursor' scrolls the screen if this is necessary.  Scrolling
 * the screen entails a call to 'paint_map' and you'd have an endless loop.
 */

void paint_map(void)
{
  coord x, y;

  /* Paint the map line by line. */
  for (y = d.psy * SECT_H; y < d.psy * SECT_H + VMAP_H; y++)
  {
    cursor(0, 1 + y - d.psy * SECT_H);
    for (x = d.psx * SECT_W; x < d.psx * SECT_W + VMAP_W; x++)
      print_tile_at_position(x, y);
  }
      
  /* Update the screen. */
  update();
}




/*
 * Set a color determined by the type of tile to be printed.
 */

void set_color_for_tile(char tile)
{
  switch (tile)
  {
    case ROCK:
      set_color(C_DARK_GRAY);
      break;
    case FLOOR:
      set_color(C_LIGHT_GRAY);
      break;
    case STAIR_UP:
    case STAIR_DOWN:
      set_color(C_WHITE);
      break;
    default:
      set_color(C_BROWN);
      break;
  }
}



/*
 * Set the screen cursor based upon the map coordinates.
 *
 * If necessary the screen map will be scrolled to show the current map
 * position on the screen.
 */

void map_cursor(coord x, coord y)
{
  _BOOL change = FALSE, any_change = FALSE;
  coord xp, yp;

  do
  {
    /* Any display change necessary? */
    any_change |= change;
    change = FALSE;

    /* Determine the screen coordinates for the map coordinates. */
    xp = x - d.psx * SECT_W;
    yp = y - d.psy * SECT_H + 1;

    /* Check scrolling to the right. */
    if (yp < 1)
    {
      d.psy--;
      change = TRUE;
    }
    /* Check scrolling to the left. */
    else if (yp >= VMAP_H)
    {
      d.psy++;
      change = TRUE;
    }
    /* Check scrolling downwards. */
    if (xp < 1)
    {
      d.psx--;
      change = TRUE;
    }
    /* Check scrolling upwards. */
    else if (xp >= VMAP_W)
    {
      d.psx++;
      change = TRUE;
    }
  }
  while (change);

  /* Scroll the map if required to do so. */
  if (any_change)
    paint_map();

  /* Set the cursor. */
  cursor(xp, yp);
}



/*
 * Change a door at a given position to another type of door.
 */

void change_door(coord x, coord y, byte door)
{
  coord sx, sy;
  byte i;

  get_current_section_coordinates(x, y, &sx, &sy);
  
  for (i = 0; i < 4; i++)
    if (d.s[d.dl][sx][sy].dx[i] == x && d.s[d.dl][sx][sy].dy[i] == y)
    {
      d.s[d.dl][sx][sy].dt[i] = door;
      map[x][y] = door;
      set_knowledge(x, y, 0);
      know(x, y);
    }
}



/*
 * Determine whether a given position is already known.
 *
 * NOTE: The knowledge map is saved in a bit field to save some memory.
 */

_BOOL is_known(coord x, coord y)
{
  return (_BOOL) (d.known[d.dl][x >> 3][y] & (1 << (x % 8)));
}



/*
 * Set or reset a knowledge bit in the knowledge map.
 */

void set_knowledge(coord x, coord y, byte known)
{
  if (known)
    d.known[d.dl][x >> 3][y] |= (1 << (x % 8));
  else
    d.known[d.dl][x >> 3][y] &= (~(1 << (x % 8)));
}
