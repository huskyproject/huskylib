#include <stdio.h>
#include "../../huskylib/huskylib.h"
#include "../../huskylib/strext.h"

void main()
{
    char *s;

    s = sstrdup("	\"This Is A Test\"     ");
    stripRoundingChars(s, " \t\"");
    printf("'%s'\n", s);
    nfree(s);
}
