#ifndef _var_env_
#define _var_env_
#include "var_env.h"
#endif

void new_body(env* current_env, token* token_arr, size_t* idx);

void new_body(env* prev_env, token* token_arr, size_t* idx){
	env* current_env = create_env(prev_env);
	int end_scope = 0;
	while(token_arr[*idx].type!=END_OF_TOKEN){
    	switch(token_arr[*idx].type){
    	case KEYWORD:
    	{
    		if(strcmp(token_arr[*idx].str, "int")==0){
    			eat_token(idx);
    			expect(token_arr[*idx], IDENTIFIER, "error: forget variable");
    			char* str = token_arr[*idx].str;
    			if(token_arr[*idx+1].type==EQUAL && token_arr[*idx+2].type==NUMBER){
    				//char* str = token_arr[*idx].str;
    				*idx+=2;
    				AST_node* root = op2AST(token_arr, idx);
    				//print_asttree(root->ptr_arr[0]);
    				int val = eval_ASTtree(current_env, root->ptr_arr[0]);
    				defineVar(current_env, str, val);
    				free_ASTtree(root);
				}else if(token_arr[*idx+1].type==SEMICOLON){
					//declareVar(current_env, token_arr[*idx].str);
					declareVar(current_env, str);
					(*idx)++;
				}else{
					printf("invalid variable declaration or definition\n");
				}
				eat_token(idx); //eat up semicolon
			}else if(strcmp(token_arr[*idx].str, "if")==0){
				
			}
			break;
		}
		case NUMBER:
		{
			AST_node* root = op2AST(token_arr, idx);
    		//print_asttree(root->ptr_arr[0]);
    		printf("ans: %d\n",eval_ASTtree(current_env, root->ptr_arr[0]));
    		free_ASTtree(root);
    		eat_token(idx); //eat up semicolon
    		break;
		}
		case IDENTIFIER:
		{
			char* str = token_arr[*idx].str;
			if( token_arr[(*idx)+1].type==EQUAL){
				*idx+=2;
				AST_node* root = op2AST(token_arr, idx);
	    		//print_asttree(root->ptr_arr[0]);
	    		int val = eval_ASTtree(current_env, root->ptr_arr[0]);
	    		assignVar(current_env, str, val);
	    		free_ASTtree(root);
    			eat_token(idx); //eat up semicolon
			}
    		
    		break;
		}
		case BODY:
		{
			if(token_arr[*idx].str[0]=='{'){
				eat_token(idx);  //eat up '{'
				new_body(current_env, token_arr, idx);
			}else if(token_arr[*idx].str[0]=='}'){
				eat_token(idx);  //eat up '}'
				current_env = out_env(current_env);
				end_scope = 1;
			}
    		break;
		}
		}
		
		if(end_scope ==1){
			break;
		}
		
	}
	
}

