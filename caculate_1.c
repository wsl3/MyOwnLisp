/*
   第七章彩蛋
   1. 编写递归函数计算语法树的节点个数。
   2. 编写递归函数计算语法树的叶子节点个数。
   3. 编写递归函数计算语法树中的分支个数。
   4. 怎样使用 strstr 检查节点是否有 expr 标签(tag)
   5. 怎样使用 strcmp 检查节点是否为 ( 或 ) 节点？
   6. 添加 % 运算符，用于取余运算，如：% 10 6 等于 4。
   7. 添加 ^ 运算符，用于指数运算，如：^ 4 2 等于 16。
   8. 添加 min 函数，返回操作数中最小的数，如：min 1 5 3 等于 1。
   9. 添加 max 函数，返回操作数中最大的数，如：max 1 5 3 等于 5。
   10. 改变 ｀-｀ 字符的作用，使其后面只有一个操作数的时候，返回该数的负数
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mpc.h"
#include<readline/readline.h>
#include<readline/history.h>

int numNodes(mpc_ast_t*);
double caculate(mpc_ast_t*);
double caculate_op(char*, int, int);
int numLeaf(mpc_ast_t*);

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
	    operator : '+'| '-'| '*'| '/'| '%'| '^'| \"min\"| \"max\" ;                  \
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
		printf("叶子节点: %d\n", numLeaf(t));
		printf("计算结果: %.2lf\n", caculate(t));
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
double caculate(mpc_ast_t* t)
{
    // 数字
    if(strstr(t->tag, "number")){
	return atoi(t->contents);
    }
    //根结点或者expr
   
    /* // 彩蛋5
    if(strstr(t->tag, "expr") && (t->children_num > 0)){
	printf("彩蛋: 看我发现了什么! %s , %s\n", t->children[0]->contents, t->children[t->children_num-1]->contents);
    }*/
    char* op = t->children[1]->contents;
    double x = caculate(t->children[2]);    
    int i = 3;
    // 没有number有expr
    while(strstr(t->children[i]->tag, "expr")){
	x = caculate_op(op, x, caculate(t->children[i]));
	i++;
    }

    // 彩蛋10
    if((strstr(t->children[3]->tag, "expr" )==NULL) && (strcmp(op, "-")==0)){
	return -x;
    }
    return x;
}

//彩蛋1. 递归计算 AST 中的节点数
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

double caculate_op(char* op, int x, int y)
{
    if(strcmp(op, "+") == 0){return x + y; }
    if(strcmp(op, "-") == 0){return x - y; }
    if(strcmp(op, "/") == 0){return x / y; }
    if(strcmp(op, "*") == 0){return x * y; }
    if(strcmp(op, "/") == 0){return x / y; }

    if(strcmp(op, "%") == 0){return x % y; }
    if(strcmp(op, "^") == 0){return pow(x, y); }
    if(strcmp(op, "min") == 0){return (x>y?y:x); }
    if(strcmp(op, "max") == 0){return (x>y?x:y); }

}

// 彩蛋2. 递归计算叶子节点数
int numLeaf(mpc_ast_t* t)
{
    if(t->children_num == 0){ return 1;}
    int i, x;
    x = 0;
    for(i=0; i<t->children_num; i++)
    {
	x += numLeaf(t->children[i]);
    }
    return x;
}
