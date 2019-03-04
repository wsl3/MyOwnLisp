/*
   考虑跨平台
*/

#include<stdio.h>
#include<stdlib.h>

#define _WIN32
#ifdef _WIN32

#include<string.h>
#define MAXINPUT 2048
static char input[MAXINPUT];

char* readline(const char* promt)
{
    fputs(promt, stdout); // 不用puts(), 因为还会输出\n
    fgets(input, MAXINPUT, stdin);
    
    char* temp = malloc(strlen(input));
    
    strncpy(temp, input, strlen(input)-1);
    temp[strlen(input)-1] = '\0';
    return temp;
}

void add_history(const char* input){}

#else
#include<readline/readline.h>
#include<readline/history.h>
#endif

int main(int argc, char* argv[])
{
    char* input;

    while(1){
	
	input = readline("Lispy>>>"); //读入是可以按左右方向键

	//添加到history, 可以按上下键
	add_history(input);

	printf("your input: %s\n", input);
	// 必须free()
	free(input);
    }
    return 0;
}
