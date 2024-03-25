#ifndef _std_
#define _std_
#include <stdio.h>
#include <stdlib.h>
#endif

typedef struct var{
	char* varName;
	int val;
	int isDefined;
	struct var* next;
}var;

typedef struct env{
	var* variable;
	struct env* next;
}env;

env* create_env(env* current_env);
env* out_env(env* current_env);
var* create_var(char* varName);
int getVarVal(env* current_env, char* varName);
var* getVar(env* current_env, char* varName);
void declareVar(env* current_env, char* varName);
void defineVar(env* current_env, char* varName, int val);
void assignVar(env* current_env, char* varName, int val);


env* create_env(env* current_env){
	env* new_env = (env*)malloc(sizeof(env));
	new_env->next = current_env;
	new_env->variable = NULL;
	return new_env;
}

env* out_env(env* current_env){
	env* temp = current_env->next;
	var* temp_var;
	var* variable = current_env->variable;
	while(variable->next!=NULL){
		free(variable->varName);
		temp_var = variable->next;
		free(variable);
		variable = temp_var;
	}
	
	free(current_env);
	return temp;
}

var* create_var(char* varName){
	var* new_var = (var*)malloc(sizeof(var));
	new_var->varName = malloc(sizeof(char)*15);
	strcpy(new_var->varName, varName);
	new_var->next = NULL;
	new_var->isDefined = 0;
	return new_var;
}

int getVarVal(env* current_env, char* varName){
	var* variable = getVar(current_env, varName);
	if(variable){
		if(variable->isDefined==1){
			return (variable)->val;
		}else{
			printf("variable %s is declared but not defined\n",varName);
			exit(2);
		}
	}else{
		printf("can't find variable\n");
	}
}

var* getVar(env* current_env, char* varName){
	var* scope_var = current_env->variable;
	
	while(1){
		if(strcmp(scope_var->varName, varName)==0){
			return scope_var;	
		}
		
		if(scope_var->next==NULL && current_env->next==NULL){
			printf("err: variable %s is not yet declared anywhere\n",varName);
			exit(2);
		}else if(scope_var->next==NULL){
			current_env = current_env->next;
			scope_var = current_env->variable;
		}else{
			scope_var = scope_var->next;
		}
		
	}
	return NULL;
}

void declareVar(env* current_env, char* varName){
	var* new_var = create_var(varName);
	new_var->isDefined = 0;
	new_var->next = current_env->variable;
	current_env->variable = new_var;
	printf("declared variable %s\n", varName);
}

void defineVar(env* current_env, char* varName, int val){
	var* new_var = create_var(varName);
	new_var->isDefined = 1;
	new_var->next = current_env->variable;
	new_var->val = val;
	current_env->variable = new_var;
	printf("defined variable %s with value %d\n", varName, val);
}

void assignVar(env* current_env, char* varName, int val){
	var* variable = getVar(current_env, varName);
	if(variable){
		(variable)->isDefined = 1;
		(variable)->val = val;
	}
	printf("variable %s is assigned value %d\n",varName, val);
	
}


