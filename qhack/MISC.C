/*                               -*- Mode: C -*- 
 * misc.c -- 
 * ITIID           : $ITI$ $Header $__Header$
 * Author          : Thomas Biskup
 * Created On      : Mon Dec 30 17:21:49 1996
 * Last Modified By: Thomas Biskup
 * Last Modified On: Thu Jan  9 21:18:23 1997
 * Update Count    : 26
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

/*
 * Includes.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "qhack.h"


/*
 * Local variables.
 */

/* Is the message buffer currently used? */
static _BOOL mbuffer_full = FALSE;

/* What's the current x position in the message buffer? */
static byte mbuffer_x = 0;



/*
 * Local prototypes.
 */

void more(void);



/*
 * Functions.
 */

/*
 * Display a message in the message line.
 *
 * The color will be reset to light gray and the message buffer will be
 * cleared if it was full.
 */

void message(char *fmt, ...)
{
  va_list vl;
  static char buffer[1000];

  /* Evaluate the format string. */
  va_start(vl, fmt);
  vsprintf_s(buffer, sizeof(buffer), fmt, vl);
  va_end(vl);

  /* Clear the message buffer if necessary. */
  if (mbuffer_full)
    more();

  /* Position the cursor. */
  cursor(0, 0);

  /* Reset the color. */
  set_color(C_LIGHT_GRAY);

  /* Display the message. */
  prtstr("%s", buffer);

  /* Update the screen. */
  update();

  /* Note the new message in the buffer. */
  mbuffer_full = TRUE;
  mbuffer_x = strlen(buffer) + 1;
}



/*
 * A simple convenience function for typical PC-related messages.
 */

void you(char *fmt, ...)
{
  va_list vl;
  static char buffer[1000];

  va_start(vl, fmt);
  vsprintf_s(buffer, sizeof(buffer), fmt, vl);
  va_end(vl);
  
  message("You %s", buffer);
}



/*
 * Display a (more) prompt at the appropriate position in the message
 * buffer and aftwards clear the message buffer.
 */

void more(void)
{
  cursor(mbuffer_x, 0);
  set_color(C_WHITE);
  prtstr("(more)");
  while (getkey() != ' ');
  clear_messages();
}



/*
 * Clear the message buffer.
 */

void clear_messages(void)
{
  cursor(0, 0);
  clear_to_eol();
  mbuffer_full = FALSE;
  mbuffer_x = 0;
}



/*
 * Get a target position starting from a base position at (xp, yp).
 */

void get_target(coord xp, coord yp, coord *x, coord *y)
{
  char c;

  *x = xp;
  *y = yp;
  
  message("Which direction? ");
  c = getkey();
  clear_messages();

  switch (c)
  {
    case 'i':
      (*y)--;
      break;
      
    case 'j':
      (*x)--;
      break;
      
    case 'k':
      (*y)++;
      break;
      
    case 'l':
      (*x)++;
      break;

    default:
      (*x) = (*y) = -1;
      break;
  }
}



/*
 * Extract a number from a string in dice notation.  Dice may have
 * up to 127 sides.
 */

int16 dice(char *dice)
{
  int16 roll = 0, sides = 0, i, amount = 0, bonus = 0, prefix = 0;
  char *c;

  c = dice;
  while (*c && isdigit(*c))
  {
    amount *= 10;
    amount += *c - '0';
    c++;
  }
  if (!*c)
    return amount;
  if (*c != 'd')
    die("Illegal die format (1)");
  c++;
  while (*c && isdigit(*c))
  {
    sides *= 10;
    sides += *c - '0';
    c++;
  }
  if (*c)
  {
    if (*c == '-')
      prefix = -1;
    else if (*c == '+')
      prefix = +1;
    else
      die("Illegal die roll format (2)");
    c++;
    while (*c && isdigit(*c))
    {
      bonus *= 10;
      bonus += *c - '0';
      c++;
    }
    if (*c)
      die("Illegal die roll format (3)");
  }
  
  for (i = 0; i < amount; i++)
    roll += rand_byte(sides) + 1;

  return (int16) (roll + (prefix * bonus));
}
  
  

/*
 * Return the absolute value of a variable.
 */

uint32 iabs(int32 x)
{
  return (uint32) x;
}



/*
 * Return the maximum of two given values.
 */

uint32 imax(int32 a, int32 b)
{
  return ((a > b) ? a : b);
}


/*
 * Return the minimum of two given values.
 */

uint32 imin(int32 a, int32 b)
{
  return ((a < b) ? a : b);
}



/*
 * Construct a string from a given format string plus variables.
 *
 * NOTE: This function should not be called more than once in one run
 *       (e.g. in one 'printf' instruction) since the second call will
 *       overwrite the values determined by the first call.
 */

char *string(char *fmt, ...)
{
  static char buffer[1000];
  va_list vl;

  va_start(vl, fmt);
  vsprintf_s(buffer, sizeof(buffer), fmt, vl);
  va_end(vl);

  return buffer;
}
