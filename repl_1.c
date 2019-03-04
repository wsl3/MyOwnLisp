#include<stdio.h>
#include<stdlib.h>

#include<readline/readline.h>
#include<readline/history.h>
/*
    Note:命令行编译必须手动链接,即 gcc file.c -lreadline, 否则会报错
*/

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
