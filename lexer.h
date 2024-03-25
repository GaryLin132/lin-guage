#ifndef _std_
#define _std_
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef _tokenType_
#define _tokenType_
#define TOKEN_TYPE \
    X(BEGINNING)   \
    X(EXIT)        \
    X(SEPERATOR)   \
    X(SEMICOLON)   \
    X(KEYWORD)     \
    X(IDENTIFIER)  \
    X(NUMBER)      \
    X(OPERATOR)    \
    X(EQUAL)       \
    X(COMPARE)     \
    X(CONDITION)   \
    X(BODY)        \
    X(END_OF_TOKEN)

#define tokenType_number 13

#endif

typedef enum
{
#define X(type) type,
    TOKEN_TYPE
#undef X
} tokenType_enum;

//convert enum to string 
char *tokenType_toStr(tokenType_enum type)
{
    switch (type)
    {
#define X(type) \
    case type:  \
        return #type;
        TOKEN_TYPE
#undef X
    }
};

char* keyword[] = {
	"int",
	"function",
	"while",
	"let",
	"const",
	"if",
	"else"
};

typedef struct token
{
    tokenType_enum type;
    union
    {
        int val;
        char *str;
    };
} token;

token *num_token(FILE *file, char ch);
token *string_token(FILE *file, char ch);
token *lexer(FILE *file, size_t *idx);
void print_tokenArr(token* token_arr);
void free_tokenArr(token* token_arr);
token* create_lexer_token(char ch, tokenType_enum type);

token *num_token(FILE *file, char ch)
{
    token *token = malloc(sizeof(token));
    token->type = NUMBER;
    token->val = 0;
    while (1)
    {
        if (ch == EOF || !(ch >= '0' && ch <= '9'))
        {
            break;
        }

        token->val *= 10;
        token->val += (int)(ch - '0');
        ch = fgetc(file);
    }
    ch = ungetc(ch, file);
    if (ch == EOF)
    {
        printf("ungetc failed\n");
    }
    return token;
}

token *string_token(FILE *file, char ch)
{
    token *token = malloc(sizeof(token));
    token->str = malloc(sizeof(char) * 15);
    token->type = IDENTIFIER;
    int idx = 0;

    while (1)
    {
        if (ch == EOF || !((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
        {
            break;
        }
        token->str[idx++] = ch;
        ch = fgetc(file);
    }
    token->str[idx] = '\0';
    ch = ungetc(ch, file);
    if (ch == EOF)
    {
        printf("ungetc failed\n");
    }
	
	int num_of_keywords = sizeof(keyword)/sizeof(keyword[0]);
	
    for (int i = 0; i < num_of_keywords; i++)
    {
        if (strcmp(token->str, keyword[i]) == 0)
        {
            token->type = KEYWORD;
            printf("find %s\n",token->str);
        }
    }
    return token;
}

token* create_lexer_token(char ch, tokenType_enum type){
	token *new_token = malloc(sizeof(token));
    new_token->str = malloc(sizeof(char) * 2);
    new_token->str[0] = ch;
    new_token->str[1] = '\0';
    new_token->type = type;
}

token *lexer(FILE *file, size_t *idx)
{
    char ch = fgetc(file);
    size_t size = 10;
    token *token_arr = malloc(sizeof(token) * size);

    while (ch != EOF)
    {
        switch (ch)
        {
        case '(':
        case ')':
        {
            // use {} in case to set it as a scope, so that you can declare variable in a case
            token *new_token = create_lexer_token(ch, SEPERATOR);
            token_arr[(*idx)++] = *new_token;
            break;
        }
        case '{':
        case '}':
        {
            token *new_token = create_lexer_token(ch, BODY);
            token_arr[(*idx)++] = *new_token;
            break;
        }
        case ';':
        {
            // use {} in case to set it as a scope, so that you can declare variable in a case
            token *new_token = create_lexer_token(ch, SEMICOLON);
            token_arr[(*idx)++] = *new_token;
            break;
        }
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        {
            token *new_token = create_lexer_token(ch, OPERATOR);
            token_arr[(*idx)++] = *new_token;
            break;
        }
        case '=':
        {
        	token *new_token = create_lexer_token(ch, EQUAL);
        	if((*idx)>1 && token_arr[(*idx)-1].str[0]=='='){
        		token_arr[(*idx)-1].type = COMPARE;
			}
	        token_arr[(*idx)++] = *new_token;
			break;
        }
        case ':':
        {
        	token *new_token = create_lexer_token(ch, CONDITION);
	        token_arr[(*idx)++] = *new_token;
			break;
        }
        case '>':
        case '<':
        {
        	token *new_token = create_lexer_token(ch, COMPARE);
	        token_arr[(*idx)++] = *new_token;
			break;
        }
        default:
            if (ch >= '0' && ch <= '9')
            {
                // fgetc doesn't change file pointer, therefore can pass file instead of &file
                token_arr[(*idx)++] = *(num_token(file, ch));
            }
            else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            {
                token_arr[(*idx)++] = *(string_token(file, ch));
            }
            break;
        }
        
        if ((*idx) == size)
        {
            size <<= 1;
            token_arr = (token *)realloc(token_arr, sizeof(token) * size);
            printf("size double\n");
        }
        // printf("current ch: %c\n", ch);
        ch = fgetc(file);
    }

    return token_arr;
}

void print_tokenArr(token* token_arr){
	size_t i = 0;
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
}

void free_tokenArr(token* token_arr){
	int i=0;
    while(token_arr[i].type!=END_OF_TOKEN){
		if (token_arr[i].type!=NUMBER)
        {
        	//printf("free i=%d str = %s\n",i,token_arr[i].str);
            free(token_arr[i].str);
        }else{
        	//printf("i=%d, num = %d\n",i, token_arr[i].val);
		}
		i++; 
	}
    free(token_arr); // also have to free inside the struct
}
