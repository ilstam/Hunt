/*
 * Copyright (C) 2012 Ilias Stamatis <stamatis.iliass@gmail.com>
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

#include "mylibrary.h"

char *s_tolower(char *s)
{
        char *ret;
        for (ret=s; (*s=tolower(*s)); s++)
            ;
        return ret;
}

int s_tokenize(char *s, char *tokens[], int ntoks, const char *delims)
{
    register int i;

    if (!s || !tokens || !delims || !*s || !*delims || ntoks < 1)
        return 0;

    tokens[0] = strtok(s, delims);
    if (NULL == tokens[0])
        return 0;

    for (i = 1; i < ntoks && (tokens[i] = strtok(NULL, delims)) != NULL; i++)
        ;

    return i;
}

