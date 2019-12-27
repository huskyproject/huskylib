#include <stdio.h>
#include "../../huskylib/huskylib.h"

int main(void)
{
    int i, j, r;
    char buf[1024];
    hs_addr na;

    printf("%d, %d, %d, %d, %d\n", (char*)&na.zone - (char*)&na, (char*)&na.net - (char*)&na,
           (char*)&na.node - (char*)&na, (char*)&na.point - (char*)&na, (char*)&na.domain - (char*)&na);

    for(i = 0; i < 256; ++i)
    {
        scanf("%s", buf);
        printf("\nEntered '%s'\n", buf);
        for(j = 0; j <= FTNADDR_5D; ++j)
        {
            memset(&na, 0, sizeof(na));
            r = parseFtnAddrZ(buf, &na, j);
            printf("%02X:%02X %d:%d/%d.%d@%s\n", j, r, na.zone, na.net, na.node, na.point, na.domain);
        }

    }
    r = parseFtnAddrZ(buf, &na, j);
    return 0;
}

