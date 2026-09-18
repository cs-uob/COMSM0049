#include <stdio.h>
int main(int argc, char **argv)
{
    int i=0xAABBCCDD;
    char secret[]="SECRET";
    if(argc>1)
        printf(argv[1]);
    return 0;
}
