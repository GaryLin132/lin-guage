#ifndef _lexer_
#define _lexer_
#include "lexer.h"
#endif

#ifndef _var_env_
#define _var_env_
#include "var_env.h"
#endif

typedef struct AST_node{
	char op;  //Number is 'N', Identifier is 'I', operator is the operator, 'E' means the start of an expression
	union{
		int val;
		char* id;
	};
	int body_size;
	struct AST_node** ptr_arr;
}AST_node;

//declarations
AST_node* create_treeNode(char op, int size);
token peek_token(token* token_arr, size_t* index);
void eat_token(size_t* index);
AST_node* primary_expr(token* token_arr, size_t* idx);
AST_node* multiplicative_expr(token* token_arr, size_t* idx);
AST_node* additive_expr(token* token_arr, size_t* idx);
AST_node* op2AST(token* token_arr, size_t* idx);
void print_asttree(AST_node* root);
int eval_ASTtree(env* current_env, AST_node* root);
void free_ASTtree(AST_node* root);
void expect(token token, tokenType_enum type, char* err);
int expr_len(token* token_arr, size_t index);




AST_node* create_treeNode(char op, int size){
	AST_node* node = malloc(sizeof(AST_node));
	node->op = op; //start for expression tree
	node->body_size = size;
	node->ptr_arr = (AST_node**)malloc( sizeof(AST_node*)*size );
	for(int i=0; i<size; i++){
		node->ptr_arr[i] = NULL;
	}
	return node;
}

token peek_token(token* token_arr, size_t* index){
	return token_arr[*index];
}

void eat_token(size_t* index){
	(*index)++;
}

AST_node* primary_expr(token* token_arr, size_t* idx){
	
	switch( peek_token(token_arr, idx).type ){
		case NUMBER:
		{
			AST_node* node = create_treeNode('N',2);
			node->val = token_arr[ *idx ].val;
			eat_token(idx);
			return node;
		}
		case SEPERATOR:
		{
			eat_token(idx); //eat up "("
			AST_node* node = additive_expr(token_arr, idx);
			if( peek_token(token_arr, idx).type==SEPERATOR || peek_token(token_arr, idx).str[0]==')' ){
				eat_token(idx); //eat up ")"
				return node;
			}else{
				printf("err: ( didn't follow with a )\n");
				exit(1);
			}
			
		}
		case IDENTIFIER:
		{
			AST_node* node = create_treeNode('I',2);
			node->id = malloc(sizeof(char)*15);
			strcpy(node->id, peek_token(token_arr, idx).str);
			eat_token(idx);
			return node;
		}
	}
}

AST_node* multiplicative_expr(token* token_arr, size_t* idx){
	AST_node* left = primary_expr(token_arr, idx);
	AST_node *oper, *right, *temp;
	
	while( token_arr[*idx].type!=NUMBER && ( peek_token(token_arr, idx).str[0]=='*' || peek_token(token_arr, idx).str[0]=='/' || peek_token(token_arr, idx).str[0]=='%' ) ){
		oper = create_treeNode( peek_token(token_arr, idx).str[0], 2);
		eat_token(idx);
		right = primary_expr(token_arr, idx);
		oper->ptr_arr[0] = left;
		oper->ptr_arr[1] = right;
		left = oper;
	}
	
	return left;
}



AST_node* additive_expr(token* token_arr, size_t* idx){
	AST_node* left = multiplicative_expr(token_arr, idx);
	AST_node *oper, *right;
	
	while( token_arr[*idx].type!=NUMBER && ( peek_token(token_arr, idx).str[0]=='+' || peek_token(token_arr, idx).str[0]=='-' ) ){
		oper = create_treeNode( peek_token(token_arr, idx).str[0], 2);
		eat_token(idx);
		right = multiplicative_expr(token_arr, idx);
		oper->ptr_arr[0] = left;
		oper->ptr_arr[1] = right;
		left = oper;
	}
	
	return left;
}

AST_node* op2AST(token* token_arr, size_t* idx){
	
	AST_node* start = create_treeNode('E', 1);  //stand for the start for expression tree
	
	int len = expr_len(token_arr, *idx);
		
	len = len+(*idx);
	
	/*while(token_arr[*idx].type!=SEMICOLON && token_arr[*idx].type!=COMPARE){
		start->ptr_arr[0] = additive_expr(token_arr, idx);
	}*/
	while(*idx<len){
		start->ptr_arr[0] = additive_expr(token_arr, idx);
	}
	
	
	return start;
}

int expr_len(token* token_arr, size_t idx){
	int len = 0;
	
	while(1){
		if(token_arr[idx+len].type==SEMICOLON || token_arr[idx+len].type==COMPARE || token_arr[idx+len].type==CONDITION){
			break;
		}
		
		len++;
	}
	return len;
}

void print_asttree(AST_node* root){
	
	if(root==NULL){
		return;
	}else{
		print_asttree(root->ptr_arr[0]);
		
		if(root->op=='N'){
			printf("%d\n",root->val);
		}else if(root->op=='I'){
			printf("%s\n",root->id);
		}else{
			printf("%c\n",root->op);
		}
		
		print_asttree(root->ptr_arr[1]);
	}
	
}


int eval_ASTtree(env* current_env, AST_node* root){
	if(root==NULL){
		return NULL;
	}else{
		int a = eval_ASTtree(current_env, root->ptr_arr[0]);
		int b = eval_ASTtree(current_env, root->ptr_arr[1]);

		switch(root->op){
			case '+':
				return a+b;
				break;
			case '-':
				return a-b;
				break;
			case '*':
				return a*b;
				break;
			case '/':
				return a/b;
				break;
			case '%':
				return a%b;
				break;
			case 'N':
				return root->val;
			case 'I':
				printf("meet an identifier\n");
				return getVarVal(current_env, root->id);
		}
		
	}
}

void free_ASTtree(AST_node* root){
	if(root!=NULL){
		for(int i=0; i<root->body_size; i++){
			free_ASTtree(root->ptr_arr[i]);
		}
		if(root->op=='I'){
			free(root->id);
		}
		free(root);
	}
}

void expect(token token, tokenType_enum type, char* err){
	if(token.type!=type){
		printf("%s\n"); 
		exit(3);
	}
}
