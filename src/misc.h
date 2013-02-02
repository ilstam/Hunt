#ifndef MISC_H
#define MISC_H

#include <string.h> /* strtok() */
#include <ctype.h>  /* tolower() */

#if defined(__linux__) || defined(__unix__)
        #define SYSTEM_CLEAR "clear"
#elif defined(_WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
        #define SYSTEM_CLEAR "cls"
#else
        /* fill this in with your platform-specific behaviour */
        #define SYSTEM_CLEAR ""
#endif


char *s_tolower(char *s);
int s_tokenize(char *s, char *tokens[], int ntoks, const char *delims);

#endif

