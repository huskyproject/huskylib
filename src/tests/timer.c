#include <stdio.h>
#include "huskylib/huskylib.h"

void main()
{
    hs_time timer;
    husky_SetTimer(&timer);
    tdelay(1000);
    printf("%d\n", husky_GetTimer(&timer));
}