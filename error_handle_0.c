/*
   错误(比如输入:/ 12 0)处理
   扩展: 增加 %
*/

#include<stdio.h>
#include<stdlib.h>
#include "mpc.h"
#include<readline/readline.h>
#include<readline/history.h>

enum{
    LVAL_NUM, LVAL_ERROR
};
enum{
    LVAL_ERROR_DIV_ZERO, LVAL_ERROR_BAD_OP, LVAL_ERROR_BAD_NUM
};

// 以前直接计算得到数字，现在计算得到 结构体
typedef struct{
    int type; // 0 or 1
    int num; // if type == 0
    int error; // if type ==1
}Lval;

int numNodes(mpc_ast_t*);
Lval caculate(mpc_ast_t*);
Lval caculate_op(char*, Lval, Lval);
Lval lval_num(int);
Lval lval_error(int);
void lval_print(Lval);

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
	    operator : '+' | '-' | '*' | '/'| '%' ;                  \
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
		printf("计算结果: ");
		lval_print(caculate(t));
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
Lval caculate(mpc_ast_t* t)
{
    // 数字
    if(strstr(t->tag, "number")){
	return lval_num(atoi(t->contents));
    }
    //根结点或者expr
    
    char* op = t->children[1]->contents;
    Lval x = caculate(t->children[2]);    
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

Lval caculate_op(char* op, Lval x, Lval y)
{
    if(x.type == LVAL_ERROR){ return x;}
    if(y.type == LVAL_ERROR){ return y;}
    if(strcmp(op, "+")==0){return lval_num(x.num+y.num); }
    if(strcmp(op, "-")==0){return lval_num(x.num-y.num); }
    if(strcmp(op, "*")==0){return lval_num(x.num*y.num); }
    if(strcmp(op, "/")==0){return y.num==0? lval_error(LVAL_ERROR_DIV_ZERO): lval_num(x.num/y.num); }
    if(strcmp(op, "%")==0){return y.num==0? lval_error(LVAL_ERROR_DIV_ZERO): lval_num(x.num%y.num);}
}

Lval lval_num(int n)
{
    Lval temp;
    temp.type = LVAL_NUM;
    temp.num = n;
    return temp;
}

Lval lval_error(int err)
{
    Lval temp;
    temp.type = LVAL_ERROR;
    temp.error = err;
    return temp;
}

void lval_print(Lval v)
{
    switch(v.type)
    {
	case LVAL_NUM: printf("%d\n", v.num);break;
	case LVAL_ERROR:
	    if(v.error == LVAL_ERROR_DIV_ZERO){
		printf("Error: 除数不能为 zero!\n");
	    }else if(v.error == LVAL_ERROR_BAD_NUM){
		printf("Error: bad num!\n");
	    }else if(v.error == LVAL_ERROR_BAD_OP){
		printf("Error: bad operator!\n");
	    }
	    break;
    }
}
