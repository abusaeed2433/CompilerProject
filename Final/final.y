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

    extern char *outBuffer;
    extern int outBufferSize;


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

    void printAndClearOutBuffer(){
        printf("\n%s < - - - - - - - - - -\n",outBuffer);
        free(outBuffer);
        outBufferSize = 0;
    }
    
    void continueOutBuffer(char *vc){
        int i=0, len = strlen(vc) - 1;
        while( i <= len && vc[i] == ' ') i++; // leading space
        while( len>=0 && vc[len] == ' ') len--; // trailing space after "
        while( len>=0 && vc[len] == ',' || vc[len]=='\"') len--; // space

        char *res;

        if(vc[i] == '\"'){ // constant
            i++; // trailing quotaion bad dilam

            int size = len-i+1;
            res = (char *)malloc(size+1);
            strncpy(res, vc + i, size);
            res[size] = '\0';
        }
        else{ // variable
            vc = removeRedundant(vc);
            struct VARIABLE *var = getVariable(vc);
            
            if( !doesVariableExists(vc)) {
                printf("\nVariable %s doesn't exist\n",vc);
            }
            char* val = getFormattedValueOrDefault(vc);
            res = (char *) malloc(20);
            sprintf(res, "%s ", val);
        }
        
        outBufferSize += strlen(res);
        outBuffer = (char *)realloc(outBuffer,outBufferSize);

        strcat(outBuffer,res);
    }
    

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

    void assignIfPossible(char *name, double val){

        if( !doesVariableExists(name) ){
            printf("\nVariable %s doesn't exist\n",name);
            return;
        }

        updateVariable(name,val);

    }

    void discardVariable(char *name){
        if( !doesVariableExists(name) ){
            printf("\nCan't discard %s since not found.\n",name);
            return;
        }
        printf("\nDiscarded variable %s\n",name);
        deleteVariable(name);
    }

%}

%union{
    double num;
    char *name;
}

%error-verbose
%token ENTRY_POINT END_POINT 
%token DATA_TYPE FUNC_TYPE FUNC_NAME VAR NUMBER ARITH_OPE DISCARD 

%token OUTPUT OUTPUT_VC OUTPUT_SEP OUTPUT_END

// defining token type
//%type<TYPE> ID1 ID2

%type<name> DATA_TYPE VAR FUNC_TYPE FUNC_NAME ARITH_OPE DISCARD
%type<name> OUTPUT_VC OUTPUT_SEP
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
    | var_assign block  {}
    | DISCARD discard_var block {}
    | OUTPUT out_sec block {}
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

var_assign: VAR '=' NUMBER ';'{
        assignIfPossible($1,$3);
    }
    | VAR '=' VAR ';'{
        if( !doesVariableExists($3) ){
            printf("\nVariable %s doesn't exist\n",$3);
        }
        else{
            double val = getValueOrDefault($3);
            assignIfPossible($1, val);
        }
    }
    | VAR '=' arith_exp ';'{
        assignIfPossible($1, $3);
    }
    ;

discard_var: VAR ';'{
        discardVariable($1);
    }
    | VAR ',' discard_var {
        discardVariable($1);
    }

out_sec: out_vc OUTPUT_END { printAndClearOutBuffer(); }
    ;
out_vc: OUTPUT_VC { continueOutBuffer($1); }
    | out_vc OUTPUT_SEP OUTPUT_VC { continueOutBuffer($3); }
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
