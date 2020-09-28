#include <stdio.h>
#include <stdlib.h>

int pin=12345;

int check(int upin, int spin)
{
    if (2*upin == spin)
        return 1;
    else return -1;
}
int main(int argc, char *argv[])
{
    char welcome[50];
    char name[40];
    int upin,auth;
    printf("Enter you name followed by your pin:\n");
    scanf("%39s%d",name, &upin);
    sprintf(welcome,"Hello %s",name);

    auth=check(upin,pin);
    printf(welcome);
    if(auth==-1)
    {
        printf("Sorry, try again...\n");
        exit(0);
    }
    return 0;
}
    
        
