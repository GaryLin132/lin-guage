#ifndef _std_
#define _std_
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef _ast_func_
#define _ast_func_
#include "ast_func.h"
#endif

AST_node* parse_condition(token* token_arr, size_t* idx);
int eval_condition(env* current_env, AST_node* root);

AST_node* parse_condition(token* token_arr, size_t* idx){
	
	AST_node* root = op2AST(token_arr, idx);
	//print_asttree(root->ptr_arr[0]);
	
	AST_node* comp = create_treeNode('C', 2);
	comp->id = malloc(sizeof(char)*3);
	strcpy(comp->id, peek_token(token_arr, idx, 0).str);
	eat_token(idx);
	comp->ptr_arr[0] = root;
	comp->ptr_arr[1] = op2AST(token_arr, idx);
	//print_asttree(comp->ptr_arr[1]->ptr_arr[0]);
	
	return comp;
}

int eval_condition(env* current_env, AST_node* root){
	int temp1 = eval_ASTtree(current_env, root->ptr_arr[0]->ptr_arr[0]);
	int temp2 = eval_ASTtree(current_env, root->ptr_arr[1]->ptr_arr[0]);
	
	if(strcmp(root->id, ">")==0){
		return (temp1>temp2) ? 1 : 0;
	}else if(strcmp(root->id, "<")==0){
		return (temp1<temp2) ? 1 : 0;
	}else if(strcmp(root->id, ">=")==0){
		return (temp1>=temp2) ? 1 : 0;
	}else if(strcmp(root->id, "<=")==0){
		return (temp1<=temp2) ? 1 : 0;
	}else if(strcmp(root->id, "==")==0){
		return (temp1==temp2) ? 1 : 0;
	}
}


