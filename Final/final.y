%{
    #include<stdio.h>
    #include<math.h>
    #include<string.h>
    #include<stdlib.h>
    #include "prototype_list.h"

    int yylex(void);
	void yyerror(char *s);

    struct PARAMETER *paramHead = NULL;
    struct PARAMETER *paramTail = NULL;

    void initProto(char *paramType){
        //printf("type found: %s\n",paramType);
        insertParameter(&paramHead, &paramTail, paramType);
    }

    void processProto(char *funcType, char *funcName){
        //printf("Inside\n");

        struct PROTOTYPE* proto = createProto(funcType, funcName, paramHead,paramTail);
        //printf("Inside-2\n");

        paramHead = NULL; paramTail = NULL;

        if( doesProtoExists(proto) ){
            printf("\nDuplicate proto-type found\n");
            return;
        }

        insertProto(proto);
        printProto(proto);

    }



%}

%union{
    double num;
    char *name;
}

%error-verbose
%token ENTRY_POINT END_POINT 
%token TYPE FUNC_TYPE FUNC_NAME 

// defining token type
//%type<TYPE> ID1 ID2
%type<name> TYPE FUNC_TYPE FUNC_NAME

// grammar rules
%%
program: many_proto_type ENTRY_POINT END_POINT     { printf("program executed"); }
    | block
    ;

many_proto_type: 
    | many_proto_type FUNC_TYPE FUNC_NAME '(' many_type ');' { 
        processProto($2,$3);
    }

many_type: 
    | FUNC_TYPE many_type { initProto($1); }
    | FUNC_TYPE ',' many_type { initProto($1); }
    ;


block: { printf("empty block\n"); }
    |stmt { printf("idk\n"); }
    ;

stmt: TYPE { printf("hudai\n"); }
    ;
%%




void yyerror(char *s)
{
	fprintf(stderr, "\n%s", s);
}

int main(){

	freopen("input.txt","r",stdin);
	//output = freopen("output.txt", "w", stdout); // output in file
	yyparse();
	return 0;
}