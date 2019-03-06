/*
    逆波兰式 解析
    gcc file.c mpc.c -lreadline -lm
    根据 AST 计算节点数和结果
*/

#include<stdio.h>
#include<stdlib.h>
#include "mpc.h"
#include<readline/readline.h>
#include<readline/history.h>

int numNodes(mpc_ast_t*);
int caculate(mpc_ast_t*);
int caculate_op(char*, int, int);

int main(int argc, char* argv[])
{
    char* input;
    
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Expression = mpc_new("expr");
    mpc_parser_t* Operation = mpc_new("operator");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
	"                                                     	\
	    number   : /-?[0-9]+/ ;                             \
	    operator : '+' | '-' | '*' | '/' ;                  \
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
		puts("\n*****************************");

		mpc_ast_t* t = r.output;
		printf("r.output tag:\t%s\n", t->tag);
		printf("r.output comments:\t%s\n", t->contents);
		printf("r.output children_num:\t%d\n", t->children_num);

		printf("\n节点总数: %d\n", numNodes(t));
		printf("计算结果: %d\n", caculate(t));
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

// 递归计算输入表达式的值
int caculate(mpc_ast_t* t)
{
    // 数字
    if(strstr(t->tag, "number")){
	return atoi(t->contents);
    }
    //根结点或者expr
    
    char* op = t->children[1]->contents;
    int x = caculate(t->children[2]);    
    int i = 3;
    // 没有number有expr
    while(strstr(t->children[i]->tag, "expr")){
	x = caculate_op(op, x, caculate(t->children[i]));
	i++;
    }
    return x;
}

// 递归计算 AST 中的节点数
int numNodes(mpc_ast_t* t)
{
    if(t->children_num == 0)
    {
	return 1;
    }else{
	int sum = 1;
	int i;
	for(i=0; i<t->children_num; i++)
	{
	    sum += numNodes(t->children[i]);
	}
	return sum;
    }
}

int caculate_op(char* op, int x, int y)
{
    if(strcmp(op, "+")==0){return x + y; };
    if(strcmp(op, "-")==0){return x - y; };
    if(strcmp(op, "*")==0){return x * y; };
    if(strcmp(op, "/")==0){return x / y; };
}
