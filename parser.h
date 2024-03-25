#ifndef _lexer_
#define _lexer_
#include "lexer.h"
#endif

typedef struct stack_node
{
    token token;
    int precedence;
} stack_node;

typedef struct STREE_node
{
    int val;
	char op;
    struct STREE_node* left;
    struct STREE_node* right;
} STREE_node;

int prec(char op);
token* infix2postfix(token *token_arr);
STREE_node* Arr2AST(token *token_arr);
void print_tree(STREE_node* root);
int eval(STREE_node* root);



int prec(char op){
	switch(op){
		case '+':
		case '-':
			return 0;
		case '*':
		case '/':
		case '%':
			return 1;
		case '(':
		case ')':
			return -1;
	}
}

token* infix2postfix(token *token_arr){
	int stack_size = 20;
    stack_node *stack = malloc(sizeof(stack_node)*stack_size);
    int stack_top = 0;
    
    size_t i = 0; //traversing index
    size_t j = 0; //operating index
    
    int precedence;
    while (token_arr[i].type != END_OF_TOKEN)
    {
    	if(token_arr[i].type==NUMBER){
    		if(i!=j){
    			token_arr[j++] = token_arr[i];
			}else{
				j++;
			}
		}else if(token_arr[i].type==OPERATOR ){
			precedence = prec(token_arr[i].str[0]);
			while(stack_top>=1 && precedence<=stack[stack_top-1].precedence){
				token_arr[j++] = stack[stack_top-1].token;
				stack_top--;
			}
			stack[stack_top].precedence = precedence;
			stack[stack_top].token = token_arr[i];
			stack_top++;
			
			if(stack_top==stack_size){
				stack_size<<1;
				stack = realloc(stack, stack_size);
			}	
		}else if(token_arr[i].type==SEPERATOR){
			if(token_arr[i].str[0]=='('){
				precedence = prec(token_arr[i].str[0]);
				stack[stack_top].precedence = precedence;
				stack[stack_top].token = token_arr[i];
				stack_top++;
			}else{
				while(stack_top>=1 && stack[stack_top-1].token.str[0]!='('){
					token_arr[j++] = stack[stack_top-1].token;
					stack_top--;
					//stack_top will stop at the index of '('
				}
				stack_top--;
			}
		}
		i++;
    }
    while(stack_top>=1){
    	stack_top--;
    	token_arr[j++] = stack[stack_top].token;
	}
	token_arr[j] = token_arr[i]; //set it to END_OF_TOKEN
	
	free(stack);
	return token_arr;
}




STREE_node* Arr2AST(token *token_arr)
{
	
	token_arr = infix2postfix(token_arr);
	
	// create AST tree
	int stack_size = 20;
	STREE_node** stack = malloc(sizeof(STREE_node*)*stack_size);
	int stack_top = 0;
	size_t i=0;
	while(token_arr[i].type!=END_OF_TOKEN){
		if(token_arr[i].type==NUMBER){
			STREE_node* node = malloc(sizeof(STREE_node));
			node->left = NULL;
			node->right = NULL;
			node->op = 'N';
			node->val = token_arr[i].val;
			//push to stack
			stack[stack_top++] = node;
		}else if(token_arr[i].type==OPERATOR){
			//it is a operator
			STREE_node* node = malloc(sizeof(STREE_node));
			node->op = token_arr[i].str[0];
			node->right = stack[--stack_top];
			node->left = stack[--stack_top];
			//push back to stack
			stack[stack_top++] = node;
		}
		
		if(stack_top==stack_size){
			stack_size<<=1;
			stack = (STREE_node**)realloc(stack, stack_size);
		}
		i++;
	}
	
	STREE_node* ptr = stack[0];
	free(stack);
	
	
	
	i = 0;
	while(token_arr[i].type != END_OF_TOKEN){
		if (token_arr[i].type!=NUMBER)
        {
            printf("token type: %s, token string: %s\n", tokenType_toStr(token_arr[i].type), token_arr[i].str);
        }
        else
        {
            printf("token type: %s, token val: %d\n", tokenType_toStr(token_arr[i].type), token_arr[i].val);
        }
        i++;
	} 
	return ptr;
}




void print_tree(STREE_node* root){
	
	if(root==NULL){
		return;
	}else{
		print_tree(root->left);
		
		if(root->op!='N'){
			printf("%c\n",root->op);
		}else{
			printf("%d\n",root->val);
		}
		
		print_tree(root->right);
		
	}
	
}

int eval(STREE_node* root){
	if(root==NULL){
		return NULL;
	}else{
		int a = eval(root->left);
		int b = eval(root->right);
		
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
		}
		
	}
}


