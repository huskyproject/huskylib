/* $Id$
 */


#include<stdio.h>
#include "../../huskylib/huskylib.h"

void printStrArrayInfo(s_str_array *ss)
{
    int ii;
    printf("%d tokens found\n", ss->count);
    for(ii=0; ii < ss->count-1; ++ii)
        printf("\"%s\", ", STR_N(ss, ii));
    printf("\"%s\"\n", STR_N(ss, ii));
}

int main(void)
{
    s_str_array *ss, *ssc;
    char *test_string = " \tt1   t3, ,,t3\t,t4\tt5,t6\t ttt7   , "; /* that's right, 2xt3 */
    int ii;
    char *converted;
    printf("test string = \"%s\"\n", test_string);
    ss = makeStrArray(test_string);
    printStrArrayInfo(ss);

    printf("Testing copy...\n");
    ssc = copyStrArray(ss);
    printStrArrayInfo(ssc);

    printf("Testing find...\n");

    for(ii=0; ii < ss->count; ++ii)
        if(findInStrArray(ss, STR_N(ssc,ii)) != ii)
        {
            printf("ERROR! finding string #%d failed\n", ii);
            if(ii == 2)
                printf("(not an error, findInStrArray finds first occurrence of a string)\n");
        }
        else
        {
            printf("finding string #%d ok\n", ii);
            if(ii == 2)
                printf("Not OK at all!!! findInStrArray should find first occurrence of a string.\n");
        }

    ii = findInStrArray(ss, "t7");
    if(ii != -1)
        printf("ERROR! found nonexistent string at #%d\n", ii);

    converted = StrArray2String(ss);
    if(converted == NULL)
        printf("ERROR! something unexpected happened in StrArray2String\n");
    printf("StrArray2String returned this string:\n \"%s\"\n", converted);

    return 0;
}
