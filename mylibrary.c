#include <stdio.h>
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
