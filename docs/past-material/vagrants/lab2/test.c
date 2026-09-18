#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char b[1024];
    FILE *f;

    f = fopen("badfile", "r");
    fread(b, sizeof(char), 1024, f);
    fclose(f);

    ((void(*)())b)();
}
