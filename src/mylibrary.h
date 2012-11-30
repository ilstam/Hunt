#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include <string.h> // strtok()
#include <ctype.h>  // tolower()

#if defined(_WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #define SYSTEM_CLEAR  "cls"
#elif defined(__linux__) || defined(__unix__)
    #define SYSTEM_CLEAR  "clear"
#else
    // fill this in with your platform-specific behaviour
    #define SYSTEM_CLEAR  ""
#endif

/********************************************************************
 * s_tolower: Converts each letter of a string to its lower.
 *            Returns a pointer to the string.
 ********************************************************************/
char *s_tolower(char *s);

/********************************************************************
 * s_tokenize: Splits s into tokens (ntoks is the max) using strtok()
 *             with delims as delimiters. Each token saved in tokens[].
 *             Returns the number of tokens that saved.
 ********************************************************************/
int s_tokenize(char *s, char *tokens[], int ntoks, const char *delims);

#endif

