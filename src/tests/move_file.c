#include <stdio.h>
#include "../../huskylib/huskylib.h"

int main()
{
    move_file("/tmp/f1", "/mnt/c/f2", 1);
    return 0;
}