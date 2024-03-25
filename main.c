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

int main(void)
{
	FILE *file;
	token *token_arr;
	size_t tokenArr_size = 0;
	file = fopen("test.ll", "r");

	token_arr = lexer(file, &tokenArr_size);
	fclose(file);

	token_arr[tokenArr_size] = *(token *)malloc(sizeof(token));
	token_arr[tokenArr_size].type = END_OF_TOKEN;
	token_arr[tokenArr_size].str = "$";
	tokenArr_size++;

	print_tokenArr(token_arr);

	// create environment
	env *init_env = NULL;

	// start parsing
	printf("start parsing\n");
	size_t idx = 0;
	new_body(init_env, token_arr, &idx);

	/*while(token_arr[idx].type!=END_OF_TOKEN){
		switch(token_arr[idx].type){
		case KEYWORD:
		{
			if(strcmp(token_arr[idx].str, "int")==0){
				eat_token(&idx);
				expect(token_arr[idx], IDENTIFIER, "error: forget variable");
				if(token_arr[idx+1].type==EQUAL && token_arr[idx+2].type==NUMBER){
					char* str = token_arr[idx].str;
					idx+=2;
					AST_node* root = op2AST(token_arr, &idx);
					print_asttree(root->ptr_arr[0]);
					int val = eval_ASTtree(current_env, root->ptr_arr[0]);
					defineVar(current_env, str, val);
					free_ASTtree(root);
				}else if(token_arr[idx+1].type==SEMICOLON){
					declareVar(current_env, token_arr[idx].str);
					idx++;
				}else{
					printf("invalid variable declaration or definition\n");
				}
				eat_token(&idx); //eat up semicolon
			}else if(strcmp(token_arr[idx].str, "if")==0){

			}
			break;
		}
		case NUMBER:
		{
			AST_node* root = op2AST(token_arr, &idx);
			print_asttree(root->ptr_arr[0]);
			printf("ans: %d\n",eval_ASTtree(current_env, root->ptr_arr[0]));
			free_ASTtree(root);
			eat_token(&idx); //eat up semicolon
			break;
		}
		}
	}*/

	exit(100);

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
