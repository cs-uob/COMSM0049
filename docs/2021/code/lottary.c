#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int get_cookie()
{
    return rand();
}
int main()
{
	int check=10;
    int guess;
    char name[20];
    guess=get_cookie();
    printf("Enter your name..\n");
    gets(name);
    if(guess == 0x41424344)
        printf("You win %s\n",name);
    else printf("Better luck next time %s :(\n",name);
    if (check!= 10)
	printf("\n hmmmm....!\n");
    return 0;
}
