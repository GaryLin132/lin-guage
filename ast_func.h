typedef struct AST_node{
	char op;  //Number is 'N', Identifier is 'I', operator is the operator, 'E' means the start of an expression
	union{
		int val;
		char* id;
	};
	int body_size;
	struct AST_node** ptr_arr;
}AST_node;

AST_node* create_treeNode(char op, int size);
void eval_wholeAST(AST_node* root);


AST_node* create_treeNode(char op, int size){
	AST_node* node = malloc(sizeof(AST_node));
	node->op = op; //start for expression tree
	node->body_size = size;
	if(size==0){
		node->ptr_arr=NULL;
	}else{
		node->ptr_arr = (AST_node**)malloc( sizeof(AST_node*)*size );
		for(int i=0; i<size; i++){
			node->ptr_arr[i] = NULL;
		}
	}
	
	return node;
}

void eval_wholeAST(AST_node* entry){
	if(entry==NULL){
		return;
	}
	
	
	for(int i=0; i<entry->body_size; i++){
		eval_wholeAST(entry->ptr_arr[i]);
	}
	
	switch(entry->op){
		case 'E':
			printf("Expression detected: ");
			printf("\n");
			break;
		case 'I':
			printf("Identifier detected: %s",entry->id);
			printf("\n");
			break;
		case 'K':
			printf("Keyword detected: %s",entry->id);
			printf("\n");
			break;
		case '=':
			printf("Variable assigned: ");
			printf("\n");
			break;
		case 'C':
			printf("Compare detected: %s",entry->id);
			printf("\n");
			break;
		case 'N':
			printf("Number detected: %d",entry->val);
			printf("\n");
			break;
		case 'B':
			printf("Body detected: ");
			printf("\n");
			break;
		case 'P':
			printf("Program start ");
			printf("\n");
			break;
	}
	
	
}

void free_AST(AST_node* entry){
	if(entry==NULL){
		return;
	}
	
	for(int i=0; i<entry->body_size; i++){
		free_AST(entry->ptr_arr[i]);
	}
	
	switch(entry->op){
		case 'I':
		case 'K':
		case 'C':
			free(entry->id);
			break;
	}
	free(entry->ptr_arr);
	free(entry);
}
