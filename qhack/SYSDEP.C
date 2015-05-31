/*                               -*- Mode: C -*- 
 * sysdep.c -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Sun Dec 29 22:29:49 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 22:20:44 1997
 * Update Count    : 36
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
#if defined(__TS_LINUX__)
# include <ncurses.h>
#elif defined(__TS_DOS__)
# include <conio.h>
#elif defined(__WINDOWS__)
#include "conio2.h"
#include <conio.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



/*
 * Prototypes.
 */

void initialize_colors(void);



/*
 * Initialize the random number generator.
 */

void init_rand(void)
{
  srand(time(NULL));
}



/*
 * Return a random 8-bit number.
 */

byte rand_byte(byte max)
{
  return (byte) (rand() % max);
}


/*
 * Return a random 16-bit number.
 */

uint16 rand_int(uint16 max)
{
  return (uint16) (rand() % max);
}


/*
 * Return a random 32-bit number.
 */

uint32 rand_long(uint32 max)
{
  return (uint32) (rand() % max);
}


/*
 * Initialize the system-dependent I/O stuff.
 */

void init_io(void)
{
#if defined(__TS_LINUX__)
  /* Basic setup. */
  if (initscr() == NULL)
    die("could not open the screen");

  notimeout(stdscr, 1);
  noecho();
  cbreak();
  keypad(stdscr, 1);
#elif defined(__TS_DOS__)
#elif defined(__WINDOWS__)
#else
# error Please define 'init_io' for your target system.
#endif
  
  initialize_colors();
}



/*
 * Clean up the IO stuff.
 */

void clean_up_io(void)
{
#if defined(__TS_LINUX__)
  nocbreak();
  echo();
  endwin();
#elif defined(__TS_DOS__)
  set_color(C_LIGHT_GRAY);
  clear_screen();
#elif defined(__WINDOWS__)
	set_color(C_LIGHT_GRAY);
	clear_screen();
#else
# error Please define 'clean_up_io' for your target system.
#endif
}



/*
 * Clear the screen.
 *
 * The cursor is expected to be at position (0, 0) afterwards.
 */

void clear_screen(void)
{
#if defined(__TS_LINUX__)
  clear();
  update();
#elif defined(__TS_DOS__)
  clrscr();
#elif defined(__WINDOWS__)
	clrscr();
#else
# error Please define 'clear_screen' for your target system.
#endif
}


/*
 * Set the cursor to a specified scren position.
 *
 * Start at position (0, 0).
 */

void cursor(byte x, byte y)
{
#if defined(__TS_LINUX__)
  move(y, x);
  update();
#elif defined(__TS_DOS__)
  gotoxy(x + 1, y + 1);
#elif defined(__WINDOWS__)
	gotoxy(x + 1, y + 1);
#else
# error Please define 'cursor' for your target system.
#endif
}


/*
 * Print one character.
 */

void prtchar(byte c)
{
#if defined(__TS_LINUX__)
  addch((char) c);
#elif defined(__TS_DOS__)
  cprintf("%c", c);
#elif defined(__WINDOWS__)
	_cprintf("%c", c);
#else
# error Please define 'prtchar' for your target system.
#endif
}



/*
 * Read one character from the keyboard without echoing it.
 */

char getkey(void)
{
  return (char) _getch();
}



/*
 * Print a string to the screen.
 */

void prtstr(char *fmt, ...)
{
  va_list vl;
  static char buffer[1000];

  va_start(vl, fmt);
  vsprintf_s(buffer, sizeof(buffer), fmt, vl);
  va_end(vl);
  
#if defined(__TS_LINUX__)
  addstr(buffer);
#elif defined(__TS_DOS__)
  cprintf("%s", buffer);
#elif defined(__WINDOWS__)
  _cprintf("%s", buffer);
#else
# error Please define 'prtstr' for your target system.
#endif
  update();
}


/*
 * Print a string to the screen using the standard I/O functionality.
 */

void stdprtstr(char *fmt, ...)
{
  va_list vl;
  static char buffer[1000];

  va_start(vl, fmt);
  vsprintf_s(buffer, sizeof(buffer), fmt, vl);
  va_end(vl);
  
  printf("%s", buffer);
  fflush(stdout);
}


/*
 * Update the screen.
 */

void update(void)
{
#if defined(__TS_LINUX__)
  refresh();
#elif defined(__TS_DOS__)
#elif defined(__WINDOWS__)
#else
# error Please define 'update' for your target system.
#endif
}



/*
 * Clear the current line up to its end without moving the cursor.
 */

void clear_to_eol(void)
{
#if defined(__TS_LINUX__)
  clrtoeol();
#elif defined(__TS_DOS__)
  clreol();
#elif defined(__WINDOWS__)
	clreol();
#else
# error Please define 'clear_to_eol' for your target system.
#endif
}




/*
 * Color handling stuff.
 */

#if defined(__TS_LINUX__)
static byte ct[MAX_COLOR] =
{
  COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_MAGENTA,
  COLOR_YELLOW, COLOR_WHITE, COLOR_BLACK, COLOR_BLUE, COLOR_GREEN,
  COLOR_CYAN, COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};
#endif

/*
 * Initialize the basic color stuff.
 */

void initialize_colors(void)
{
#if defined(__TS_LINUX__)
  byte i;
  
  start_color();

  for (i = 0; i < MAX_COLOR; i++)
    init_pair(i, ct[i], COLOR_BLACK);
#elif defined(__TS_DOS__)
#elif defined(__WINDOWS__)
#else
# error Please define 'initialize_colors' for your target system.
#endif
}


/*
 * Set a given color.
 */

void set_color(byte c)
{
#if defined(__TS_LINUX__)
  if (c >= 8)
    attrset((A_BOLD | (COLOR_PAIR(c))));
  else
    attrset(COLOR_PAIR(c));
#elif defined(__TS_DOS__)
	textcolor(c);
#elif defined(__WINDOWS__)
  textcolor(c);
#else
# error Please define 'set_color' for your target system.
#endif
}
