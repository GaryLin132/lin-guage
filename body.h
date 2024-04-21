#ifndef _var_env_
#define _var_env_
#include "var_env.h"
#endif

#ifndef _if_stmt_
#define _if_stmt_
#include "if_stmt.h"
#endif


void new_body(env** current_env, token* token_arr, size_t* idx, AST_node* currBody_ptr);

void new_body(env** prev_env, token* token_arr, size_t* idx, AST_node* currBody_ptr){
	env* current_env = create_env(prev_env);
	
	int leaf_num = 0;
	int loopStart_idx = -1;
	int end_scope = 0;
	while(token_arr[*idx].type!=END_OF_TOKEN){
		
    	switch(token_arr[*idx].type){
    	case KEYWORD:
    	{
    		if(strcmp(token_arr[*idx].str, "int")==0){
    			eat_token(idx);
    			expect(token_arr[*idx], IDENTIFIER, "error: forget variable");
    			char* str = token_arr[*idx].str;
    			AST_node* root;
    			if(peek_token(token_arr,idx,1).type==EQUAL && peek_token(token_arr,idx,2).type==NUMBER){
    				*idx+=2;
    				AST_node* subtree_root = op2AST(token_arr, idx);
    				int val = eval_ASTtree(current_env, subtree_root->ptr_arr[0]);
    				root = defineVar1(current_env, str, val, "int");
    				root->ptr_arr[1] = subtree_root;
    				//free_ASTtree(root);
				}else if(peek_token(token_arr,idx,1).type==SEMICOLON){
					//declareVar(current_env, token_arr[*idx].str);
					root = declareVar1(current_env, str, "int");
					(*idx)++;
				}else{
					printf("invalid variable declaration or definition\n");
				}
				currBody_ptr->ptr_arr[leaf_num++] = root;
				expect(token_arr[*idx], SEMICOLON, "error: forget semicolon");
				eat_token(idx); //eat up semicolon
			}else if(strcmp(token_arr[*idx].str, "if")==0){
				eat_token(idx);  //eat 'if'
				AST_node* root = create_treeNode('K',2);
				root->id = (char*)malloc(sizeof(char)*3);
				strcpy(root->id, "if");
				root->ptr_arr[0] = parse_condition(token_arr, idx);
				printf("%d\n",eval_condition(current_env, root->ptr_arr[0]));
				
				//deal with if
				expect(peek_token(token_arr,idx,1), BODY, "error: forget set a body with {}");
				int paran_count = 0;
				if( !eval_condition(current_env, root->ptr_arr[0]) ){
					while( !(token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='}'  )
						    ||
							paran_count!=1  ){
						
						if( (token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='{'  )){
							paran_count++;
						}else if((token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='}'  )){
							paran_count--;
						}
						eat_token(idx);
						//printf("eat one token of index: %d\n",*idx);
					}
					expect(token_arr[*idx], BODY, "error: forget close bracket");
					eat_token(idx); //eat up '}'
				}
				
				currBody_ptr->ptr_arr[leaf_num++] = root;
			}else if(strcmp(token_arr[*idx].str, "else")==0){
				eat_token(idx);
			}else if(strcmp(token_arr[*idx].str, "while")==0){
				
				loopStart_idx = *idx; //the index of the 'while' token
				eat_token(idx); //eat 'while'
				
				AST_node* root = create_treeNode('K',2);
				root->id = (char*)malloc(sizeof(char)*6);
				strcpy(root->id, "while");
				root->ptr_arr[0] = parse_condition(token_arr, idx);
				printf("%d\n",eval_condition(current_env, root->ptr_arr[0]));
				
				
				if( eval_condition(current_env, root->ptr_arr[0]) ){
					//printf("loop one time\n");
				}else{
					loopStart_idx = -1;
					int paran_count = 0;
					while( !(token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='}' )
							||
							paran_count!=1){
						if( (token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='{'  )){
							paran_count++;
						}else if((token_arr[*idx].type==BODY && token_arr[*idx].str[0]=='}'  )){
							paran_count--;
						}
						
						eat_token(idx);
						//printf("eat one token of index: %d\n",*idx);
					}
					expect(token_arr[*idx], BODY, "error: forget }");
					eat_token(idx); //eat up '}'
					
				}
				currBody_ptr->ptr_arr[leaf_num++] = root;
				
			}
			
			//connect to the AST tree
			
			break;
		}
		case SEPERATOR:
		case NUMBER:
		{
			AST_node* root = op2AST(token_arr, idx);
    		//print_asttree(root->ptr_arr[0]);
    		printf("ans: %d\n",eval_ASTtree(current_env, root->ptr_arr[0]));
    		free_ASTtree(root);
    		expect(token_arr[*idx], SEMICOLON, "error: forget semicolon");
    		eat_token(idx); //eat up semicolon
    		
    		//connect to the AST tree
    		currBody_ptr->ptr_arr[leaf_num++] = root;
    		
    		break;
		}
		case IDENTIFIER:
		{
			char* str = token_arr[*idx].str;
			if( token_arr[(*idx)+1].type==EQUAL){
				*idx+=2;
				AST_node* subtree_root = op2AST(token_arr, idx);
	    		//print_asttree(root->ptr_arr[0]);
	    		int val = eval_ASTtree(current_env, subtree_root->ptr_arr[0]);
	    		AST_node* root = assignVar1(current_env, str, val);
	    		root->ptr_arr[1] = subtree_root;
	    		currBody_ptr->ptr_arr[leaf_num++] = root;
	    		//free_ASTtree(root);
    			eat_token(idx); //eat up semicolon
			}else{
				printf("invalid expression\n");
				exit(2);
			}
    		
    		break;
		}
		case BODY:
		{
			if(token_arr[*idx].str[0]=='{'){
				eat_token(idx);  //eat up '{'
				
				//for tree operation
				AST_node* nextBody_ptr;
				if(currBody_ptr->ptr_arr[leaf_num-1]->op=='K' && strcmp(currBody_ptr->ptr_arr[leaf_num-1]->id, "if")==0){
					//deal with the new body after if condition
					currBody_ptr->ptr_arr[leaf_num-1]->ptr_arr[1] = create_treeNode('B',2);
					nextBody_ptr = currBody_ptr->ptr_arr[leaf_num-1]->ptr_arr[1];
				}else if(currBody_ptr->ptr_arr[leaf_num-1]->op=='K' && strcmp(currBody_ptr->ptr_arr[leaf_num-1]->id, "while")==0){
					//merely with the new body after while
					currBody_ptr->ptr_arr[leaf_num-1]->ptr_arr[1] = create_treeNode('B',2);
					nextBody_ptr = currBody_ptr->ptr_arr[leaf_num-1]->ptr_arr[1];
				}else{
					//merely a new {}
					currBody_ptr->ptr_arr[leaf_num++] = create_treeNode('B',1);
					nextBody_ptr = currBody_ptr->ptr_arr[leaf_num-1];
				}
				
				new_body(&current_env, token_arr, idx, nextBody_ptr);
				if(loopStart_idx!=-1){
					*idx = loopStart_idx;
				}
			}else if(token_arr[*idx].str[0]=='}'){
				eat_token(idx);  //eat up '}'
				out_env(&current_env);
				end_scope = 1;
			}
    		break;
		}
		case CONDITION:
		{
			eat_token(idx);
			break;
		}	
		}
		
		if(end_scope ==1){
			
			break;
		}
		if(leaf_num>=currBody_ptr->body_size){
			currBody_ptr->body_size<<=1;
			currBody_ptr->ptr_arr = realloc(currBody_ptr->ptr_arr, sizeof(AST_node)*currBody_ptr->body_size);
			
			for(int i=leaf_num; i<currBody_ptr->body_size; i++){
				//it doesn't defaultly set to NULL
				currBody_ptr->ptr_arr[i] = NULL;
			}
		}
		
	}
	
}

