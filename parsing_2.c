/*
    逆波兰式 解析
    gcc file.c mpc.c -lreadline -lm
    添加 % 操作符
*/

#include<stdio.h>
#include<stdlib.h>
#include "mpc.h"
#include<readline/readline.h>
#include<readline/history.h>


int main(int argc, char* argv[])
{
    char* input;
    
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Expression = mpc_new("expr");
    mpc_parser_t* Operation = mpc_new("operator");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
	"                                                     	\
	number   : /-?[0-9]+/ |'('/-?[0-9]+/')';   \
	operator : '+' | '-' | '*' | '/' | '%' | \"add\" | \"div\" | \"sub\" | \"mul\" ;   \
	expr     : <number> | '(' <operator> <expr>+ ')' ;  \
	lispy    : /^/ <operator> <expr>+ /$/ ;             \
  	",
	Number, Expression, Operation, Lispy);

    mpc_result_t r;
    while(1){
	
	input = readline("Lispy>>>"); //读入是可以按左右方向键

	//添加到history, 可以按上下键
	add_history(input);
	printf("\nyour input: %s\n", input);

	puts("语法解析:");
	//语法解析
	if(mpc_parse("input", input, Lispy, &r)){
		mpc_ast_print(r.output);
		mpc_ast_delete(r.output);
	}else{
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
	}
	puts("");
	// 必须free()
	free(input);
    }
    mpc_cleanup(4, Number, Expression, Operation, Lispy);
    return 0;
}
