#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    const char *message="Hello\n";

    FILE *f = fopen("a", "w");
    fwrite(message, 1, strlen(message), f);
    fclose(f);
    f = fopen("b", "w");
    fwrite(message, 1, strlen(message), f);
    fclose(f);
    return 0;
}   // main
