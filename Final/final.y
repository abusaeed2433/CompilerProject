%{
    #include<stdio.h>
    #include<math.h>
    #include<string.h>
    #include<stdlib.h>
    #include "prototype_list.h"
    #include "constant.h"
    #include "var_list.h"


    extern char lastDataType[10];

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

    
    char* removeRedundant(char *source){
        int len = strlen(source);

        char temp[len+1];
        int j = 0;
        for(int i=0; i<len; i++){
            //printf("%d, ", source[i]);
            if( source[i] == ' ' || 
                source[i] == ',' ||
                source[i] == ';' ||
                source[i] == '\t' ||
                source[i] == '\n' ||
                source[i] == 32
            )continue;

            temp[j] = source[i];
            j++;
        }

        temp[j] = '\0';
        char *dest;
        dest = malloc(j*sizeof(char));
        strcpy(dest,temp);
        //printf("\n%s\n",dest);
        return dest;
    }

    double doArithOperation(double val1, double val2, char* op){
        //printf("%lf %lf %s ---------kuttar baccha-\n",val1,val2,op);
        return getValue(val1, val2, op);
    }

    bool addVariable(char* name,char *type){
        if( doesVariableExists(name) ){
            printf("\nVariable %s is already defined\n",name);
            return false;
        }
        insertVariable(name,type,0);
        return true;
    }

    void declareVariable(char *name, char *type, double val){
        if( addVariable(name, type) ){
            updateVariable(name,val);
        }
    }

%}

%union{
    double num;
    char *name;
}

%error-verbose
%token ENTRY_POINT END_POINT 
%token DATA_TYPE FUNC_TYPE FUNC_NAME VAR NUMBER ARITH_OPE

// defining token type
//%type<TYPE> ID1 ID2

%type<name> DATA_TYPE VAR FUNC_TYPE FUNC_NAME ARITH_OPE
%type<num> NUMBER arith_exp

%%
program: many_proto_type ENTRY_POINT block END_POINT     { printf("program executed"); }
    ;

many_proto_type: 
    | many_proto_type FUNC_TYPE FUNC_NAME '(' many_type ');' { 
        processProto($2,$3);
    }

many_type: 
    | FUNC_TYPE many_type { initProto($1); }
    | FUNC_TYPE ',' many_type { initProto($1); }
    ;


block: { printf("\nempty block\n"); }
    | var_declare block {}
    ;

var_declare: DATA_TYPE others';'
    ;
others: VAR {
        declareVariable($1, lastDataType, 0);
    }
    | VAR ',' others {
        declareVariable($1, lastDataType, 0);
    }
    | VAR '=' NUMBER {
        declareVariable($1, lastDataType, $3);
    }
    | VAR '=' VAR {
        double val = getValueOrDefault($3);
        declareVariable($1, lastDataType, val);
    }
    | VAR '=' NUMBER ',' others {
        declareVariable($1, lastDataType, $3);
    }
    | VAR '=' VAR ',' others {
        double val = getValueOrDefault($3);
        declareVariable($1, lastDataType, val);
    }
    | VAR '=' arith_exp {
        declareVariable($1, lastDataType, $3);
    }
    | VAR '=' arith_exp ',' others {
        declareVariable($1, lastDataType, $3);
    }
    ;

arith_exp: VAR ARITH_OPE VAR {
        double val1 = getValueOrDefault($1);
        double val2 = getValueOrDefault($3);
        $$ = doArithOperation(val1, val2, $2);
    }
    | VAR ARITH_OPE NUMBER {
        double val1 = getValueOrDefault($1);
        $$ = doArithOperation(val1, $3, $2);
    }
    | NUMBER ARITH_OPE VAR { 
        double val2 = getValueOrDefault($3);
        $$ = doArithOperation($1, val2, $2);
    }
    | NUMBER ARITH_OPE NUMBER { 
        $$ = doArithOperation($1, $3, $2);
    }
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
    printAll();
	return 0;
}
