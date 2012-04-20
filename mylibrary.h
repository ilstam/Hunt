#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include <string.h>
#include <ctype.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #define SYSTEM_CLEAR  "cls"
#elif defined( __linux__) || defined(_unix_)
    #define SYSTEM_CLEAR  "clear"
#else 
    // fill this in with your platform-specific behaviour
    #define SYSTEM_CLEAR  ""
#endif

char *s_tolower(char *s);
int s_tokenize(char *s, char *tokens[], int ntoks, const char *delims);

#endif
