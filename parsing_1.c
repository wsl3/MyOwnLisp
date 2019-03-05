#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>

#include "mpc.h"

int main(int argc, char* argv[])
{
	puts("Lispy  ----Version 1.0");
	mpc_parser_t* Expression = mpc_new("expression");
	mpca_lang(MPCA_LANG_DEFAULT,
		"expression : /[ab]*/;", Expression);
	
	char* input;
	mpc_result_t r;

	while(1){
		input = readline("Lispy>>> ");

		add_history(input);

		printf("\nyour input: %s\n\n语法解析:\n\n", input);
		
		//语法解析
		if(mpc_parse("input", input, Expression, &r)){
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}else{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		puts("");
		free(input);
	}

	mpc_cleanup(1, Expression);
	return 0;
}
