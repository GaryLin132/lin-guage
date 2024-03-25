#ifndef _std_
#define _std_
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef _RDParser_
#define _RDParser_
#include "RDParser.h"
#endif

AST_node* parse_condition(token* token_arr, int* idx){
	
	AST_node* root = op2AST(token_arr, &idx);
	print_asttree(root->ptr_arr[0]);
	
	AST_node* comp = create_treeNode('C', 2);
	comp->id = malloc(sizeof(char)3);
	strcpy(comp->id, peek_token(token_arr, idx).str);
	comp->ptr_arr[0] = root;
	comp->ptr_arr[1] = op2AST(token_arr, &idx);
	
	return comp;
}


