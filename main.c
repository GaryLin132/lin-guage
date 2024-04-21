#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _lexer_
#define _lexer_
#include "lexer.h"
#endif

#ifndef _parser_
#define _parser_
#include "parser.h"
#endif

#ifndef _RDParser_
#define _RDParser_
#include "RDParser.h"
#endif

#ifndef _var_env_
#define _var_env_
#include "var_env.h"
#endif

#ifndef _body_
#define _body_
#include "body.h"
#endif

#ifndef _ast_func_
#define _ast_func_
#include "ast_func.h"
#endif

int main(void)
{
	FILE *file;
	token *token_arr;
	size_t tokenArr_size = 0;
	file = fopen("test.ll", "r");

	token_arr = lexer(&file, &tokenArr_size);
	fclose(file);

	print_tokenArr(token_arr);

	// create environment
	env *init_env = NULL;

	// create the start of AST
	AST_node *program = create_treeNode('P', 1);
	program->ptr_arr[0] = create_treeNode('B', 2);
	AST_node *currBody_ptr = program->ptr_arr[0];

	// start parsing
	printf("start parsing\n");
	size_t idx = 0;

	new_body(&init_env, token_arr, &idx, currBody_ptr);
	eval_wholeAST(program);

	// free_tokenArr(token_arr);
	// free_AST(program);
	printf("done\n");
	return 5;

	AST_node *root = op2AST(token_arr, &idx);
	print_asttree(root->ptr_arr[0]);
	printf("ans: %d\n", eval_ASTtree(init_env, root->ptr_arr[0]));

	// iterate parser expression
	/*STREE_node* root = Arr2AST(token_arr);
	print_tree(root);

	int ans = eval(root);
	printf("ans: %d\n",ans);*/

	// free lexer token array
	free_tokenArr(token_arr);
	// free AST tree
	free_ASTtree(root);

	return 0;
} 
