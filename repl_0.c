#include<stdio.h>

#define MAXINPUT 2048

static char input[MAXINPUT];

int main(int argc, char* argv[])
{
    while(1)
    {
	fputs("Lispy$ ", stdout);

	fgets(input, 2048, stdin);

	printf("your input: %s", input);
    }

    return 0;
}
