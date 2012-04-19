#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include <string.h>
#include <ctype.h>

char *s_tolower(char *s);
int s_tokenize(char *s, char *tokens[], int ntoks, const char *delims);

#endif
