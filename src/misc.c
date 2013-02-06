/*
* Copyright (C) 2012-2013 Ilias Stamatis <stamatis.iliass@gmail.com>
*
* This file is part of Hunt.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "misc.h"

/*
 * s_tolower:
 *    Convert all characters of a c-string to lowercase.
 *    Return the converted string on success, else NULL.
 */
char *s_tolower(char *s)
{
	char *cp;

	/* sanity checks */
	if (s == NULL)
		return NULL;
	if (!*s)
		return s;

	for (cp=s; (*cp=tolower(*cp)); cp++)
		; /* void */

	return s;
}

/*
 * s_tokenize:
 *    Tokenize a c-string, into variable size tokens.
 *    Return the number of tokens which s was broken to on success,
 *    else 0.
 */
int s_tokenize(char *s,        /* mutable string */
               char *tokens[], /* array of strings, to be filled by the
                                 function with NUL-terminated string tokens. */
               int ntoks,      /* maximum number of desired tokens.  */
               const char *delims /* string consisting of the desired
                                     delimiter characters */
)
{
	register int i;

	/* sanity checks */
	if (s  == NULL || tokens == NULL || delims == NULL
	|| !*s || !*delims || ntoks < 1)
		return 0;

	tokens[0] = strtok(s, delims);
	if (tokens[0] == NULL)
		return 0;

	for (i = 1; i < ntoks && (tokens[i] = strtok(NULL, delims)) != NULL; i++)
		; /* void */

	return i;
}

