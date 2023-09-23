%{
    #include<stdio.h>
    #include<string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <ctype.h>

    int stack[100];
    int stackSize = 0;

    char error[200];
    char lastDataType[10];

    int commentCounter = 0;
    int comment_depth = 0;
    char *comment_buffer = NULL;

    bool canDeclareHeader = true;
    bool isMainBlockFound = false;
    bool isCommingFromMain = false;
    bool isCommingFromVar = false;
    bool isLastIfValid = false;

    size_t buffer_length = 0;

    int pushState(int id){
        stackSize++;
        stack[stackSize] = id;
        return id;
    }

    int popState(){
        if(stackSize > 0) stackSize--;
        return stack[stackSize];
    }

    bool isLastOneIf(int ifs){
        if(stackSize > 1){
            if( stack[stackSize-1] == ifs){
                return true;
            }
        }
        return false;
    }


    // variable
    const int MAXIMUM_VARIABLE_LENGTH = 200;
    
    struct VAR{
        char name[200];
        double value;
    };
    
    // variable
    struct VAR *vars = 0;

    int varSize = 0;
    const int MAXIMUM_VAR_SIZE = 10;


    struct VAR* getVariable(char* name){
        for(int i=0; i<varSize; i++){
            if( strcmp(name,vars[i].name) == 0 ) return &vars[i];
        }
        return NULL;
    }
    
    void stopProgram(char *error){
        printf("%s\n",error);
        free(vars);
        free(comment_buffer);
        exit(1);
    }

    bool doesVariableExists(char *name){
        for(int i=0; i<varSize; i++){
            if( strcmp(name,vars[i].name) == 0 ) return true;
        }
        return false;
    }

    char* removeRedundant(char *source){
        int len = strlen(source);

        char temp[len+1];
        int j = 0;
        for(int i=0; i<len; i++){
            if(source[i] == ' ' || source[i] == ',' || source[i] == ';') continue;
            temp[j] = source[i];
            j++;
        }

        temp[j] = '\0';
        char *dest;
        dest = malloc(j*sizeof(char));
        strcpy(dest,temp);
        return dest;
    }

    void extractNameValue(char *nameVal, char *name, double* val){
        char *temp;
        temp = removeRedundant(nameVal);
        sscanf(temp, "%[^=]=%lf", name, val);
    }

    bool addVariable(char* name){
        int size = varSize;

        for(int i=0; i<size; i++){
            if( strcmp(name,vars[i].name) == 0 ){
                stopProgram("Variable is already defined");
                return false;
            }
        }

        vars = (struct VAR *) realloc(vars,(size+2)*MAXIMUM_VAR_SIZE*sizeof(struct VAR));
        if(vars == NULL){
            printf("Why null\n");
        }
        strcpy(vars[size].name, name);
        varSize = size+1;
        
        return true;
    }


    void runHeader(int check){
        if(!check){
            stopProgram("Invalid header format");
        }

        if(!canDeclareHeader){
            printf("Header must be at top of proram\n");
            return;
        }
        else{
            printf("Imported %s\n",yytext);
        }

    }

    void exceptHeader(){
        canDeclareHeader = false;
    }

    void initSingleComment(){
        exceptHeader();
        commentCounter++;
    }

    void resetBuffer(){
        free(comment_buffer);
        comment_buffer = NULL;
        buffer_length = 0;
    }

    void process_comment() {
        commentCounter++;
        //printf("Captured comment: %s\n", comment_buffer);
    }
    
    void checkForEnd(){
        if (comment_depth == 0) {
            BEGIN(INITIAL);
            process_comment();
            resetBuffer();
        }
        else if(comment_depth < 0){
            resetBuffer();
            stopProgram("Invalid comment found");
        }
    }

    void appendToBuffer(char ch){
        comment_buffer = (char*) realloc(comment_buffer, buffer_length + 1);
        buffer_length += sprintf(comment_buffer + buffer_length, "%c", ch);
    }

    void appendTextToBuffer(char* ch){

        if( strncmp(ch,"/*",2) == 0) comment_depth++;
        if( strncmp(ch,"*/",2) == 0) comment_depth--;
        comment_buffer = (char*) realloc(comment_buffer, buffer_length + strlen(ch) );  
        buffer_length += sprintf(comment_buffer + buffer_length, ch);
    }

    void initMultiComment(){
        exceptHeader();
        comment_depth = 1; 
        buffer_length = 2;
        comment_buffer = (char*) malloc(3);

        memset(comment_buffer, 0, 3);

        strcat(comment_buffer, "/*");
    }

    void initMain(){
        canDeclareHeader = false;
        printf("execution started\n");
    }

    void stopMain(){
        printf("Execution done\n");
        isMainBlockFound = true;
    }

    void initVarSec(char *type){
        strcpy(lastDataType,type);
        canDeclareHeader = false;
    }

    void assignValue(char *name, double val){
        for(int i=0; i<varSize; i++){
            if( strcmp(vars[i].name,name) == 0){
                vars[i].value = val;
                break;
            }
        }
    }

    void declareVariable(char *name, double val){
        name = removeRedundant(name);

        if( !addVariable(name) ){
            char message[50];
            strcpy(message,"Duplicate variable found ");
            strcat(message,name);
            stopProgram(message);
            return;
        }

        assignValue(name,val);
    }

    void processVariable(char *nameVal){
        //printf("nameVal %s\n",nameVal);

        char name[20];
        double value;
        extractNameValue(nameVal,name,&value);
        declareVariable(name,value);
    }

    void updateValue(char *nameVal){
        char *name;
        double val;
        extractNameValue(nameVal,name,&val);

        if( !doesVariableExists(name) ){
            char message[50];
            strcpy(message, "Variable doesn't exists: ");
            strcat(message,name);

            stopProgram(message);
            return;
        }

        assignValue(name,val);
    }

    bool isNumber(char* num){
        bool count = 0;
        for(int i=0; i<strlen(num); i++){
            if( num[i] == '.' && count == 0 ) { count=1; continue; }
            if( !isdigit(num[i]) ) return false;
        }
        return true;
    }

    bool isCondOpValid(char* op){
        char arr[6][5] = { "lt","gt","eq","neq","le","ge" };
        for(int i=0; i<6; i++){
            if(strcmp(arr[i],op) == 0) return true;
        }
        return false;
    }

    bool isConditionValid(double left, char* op, double right){
        if( strncmp(op,"lt",2) == 0) return (left < right);
        if( strncmp(op,"gt",2) == 0) return (left > right);
        if( strncmp(op,"eq",2) == 0) return (left == right);

        if( strncmp(op,"le",2) == 0) return (left <= right);
        if( strncmp(op,"ge",2) == 0) return (left >= right);
        if( strncmp(op,"neq",2) == 0) return (left != right); // 2 fine also
    }

    void processIfBody(char *body){
        //processed body
        char pBody[strlen(body)];

        int i=0;
        while(body[i] != '(')i++;
        i++;

        int j=0;
        while(body[i] != ')'){ pBody[j] = body[i]; i++; j++; }
        pBody[j] = '\0';

        char leftVal[50], operator[10], rightVal[50];
        sscanf(pBody, "%49s %9s %49s", leftVal, operator, rightVal);

        double left = 0, right = 0;

        if( isNumber(leftVal) ) left = strtod(leftVal,NULL);
        else{
            struct VAR *val = getVariable(leftVal);
            if(val == NULL) {
                printf("%s\n",leftVal);
                stopProgram("Invalid variable in if");
            }
            else left = (*val).value;
        }

        if( isNumber(rightVal) ) right = strtod(rightVal,NULL);
        else{
            struct VAR *val = getVariable(rightVal);
            if(val == NULL) {
                printf("%s\n",rightVal);
                stopProgram("Invalid variable in if");
            }
            else right = (*val).value;
        }

        if( !isCondOpValid(operator) ){
            stopProgram("Invalid operator in if");
            return;
        }

        isLastIfValid = isConditionValid(left,operator,right);

        printf("body of if is: %d\n",isLastIfValid);

        //printf("extracted: %s %s %s\n",leftVar,operator,rightVal);
    }

%}


%x HEADER
%x COMMENT
%x MAIN
%x VAR_SEC
%x IF_SEC
%x IGNORE_SEC

NUMBER [0-9.]+
VARIABLE [a-zA-z][a-zA-Z0-9]*
VAR_NUM ({NUMBER}|{VARIABLE})
COND_OP ("lt"|"gt"|"eq"|"neq"|"le"|"ge")

HEADER_START "import "
HEADER_END [a-zA-Z.]+";"


MAIN_START "static void main(){"
MAIN_END "}"

SINGLE_LINE_COMMENT ("//").*(\n)?

VARIABLE_ONLY [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*(,)
VARIABLE_VALUE [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*("=")[ ]*[0-9]+(,)
VARIABLE_VALUE_ASSIGN [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*("=")[ ]*[0-9]+;

VARIABLE_ONLY_LAST [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*(;)
VARIABLE_VALUE_LAST [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*("=")[ ]*[0-9]+(;)

VAR_SPACE [ ]*

DATA_TYPE ("int "|"float "|"double ")

IF "if"[ ]*
IF_COND ("(")[ ]*{VAR_NUM}[ ]*{COND_OP}[ ]*{VAR_NUM}[ ]*(")")[ ]*
IF_BODY_START [ ]*"{"
IF_BODY_END [ ]*"}"
IF_SPACE [ ]*

IGNORE_TILL_BRACE "}"

NEW_LINE_AND_TAB [\n\t]*

%%
{HEADER_START} { BEGIN(pushState(HEADER)); }

<HEADER>{HEADER_END} { runHeader(1); BEGIN( popState() ); }

<HEADER>. { runHeader(0); BEGIN(popState()); }

{SINGLE_LINE_COMMENT} { initSingleComment(); }

"/*" { initMultiComment(); BEGIN( pushState(COMMENT) ); }

<COMMENT>"/*" { appendTextToBuffer("/*"); }
<COMMENT>"*/" { 
    appendTextToBuffer("*/"); checkForEnd(); 
    if(comment_depth == 0){
        BEGIN( popState() );
    } 
}

<COMMENT>. { appendToBuffer(yytext[0]); }
<COMMENT>\n { appendTextToBuffer("\n"); }

<VAR_SEC>{VARIABLE_ONLY} { declareVariable(yytext,0); }
<VAR_SEC>{VARIABLE_ONLY_LAST} {
    declareVariable(yytext,0);
    BEGIN( popState() );
}

<VAR_SEC>{VARIABLE_VALUE} { processVariable(yytext); }
<VAR_SEC>{VARIABLE_VALUE_LAST} {
    processVariable(yytext);
    BEGIN( popState() );
}
<VAR_SEC>{VAR_SPACE} {}
<VAR_SEC>. { stopProgram("Invalid character found");}
<VAR_SEC>"/*" { 
    initMultiComment();
    BEGIN( pushState(COMMENT) );    
}


<MAIN>{IF} { BEGIN( pushState(IF_SEC) ); }

<IF_SEC>{IF_COND} { processIfBody(yytext); }
<IF_SEC>{IF_BODY_START} { 
    if(isLastIfValid){
        BEGIN(pushState(MAIN));
    }
    else{
        BEGIN( pushState(IGNORE_SEC) );
    }
}

<IF_SEC>. {}

<IGNORE_SEC>{IGNORE_TILL_BRACE} { 
    int state = popState();
    BEGIN( popState() );
}
<IGNORE_SEC>[^}] {}

{MAIN_START} { 
    //printf("main start %s\n",yytext); 
    initMain(); 
    BEGIN( pushState(MAIN) ); 
}
<MAIN>{DATA_TYPE} {
    //printf("main data type %s\n",yytext);
    initVarSec(yytext); 
    BEGIN( pushState(VAR_SEC) );
}
<MAIN>{VARIABLE_VALUE_ASSIGN} { 
    //printf("main assign %s\n",yytext);
    updateValue(yytext);
}
<MAIN>{MAIN_END} { stopMain(); BEGIN( popState() ); }
<MAIN>{SINGLE_LINE_COMMENT} {
    //printf("main comment %s\n",yytext);
    initSingleComment();
}
<MAIN>"/*" {
    //printf("main multi %s\n",yytext);
    initMultiComment(); 
    BEGIN( pushState(COMMENT) );
}
<MAIN>{NEW_LINE_AND_TAB}* { }

{NEW_LINE_AND_TAB}* { }

%%

int yywrap(){
    return 1;
}


int main(){

    // while(1){
    //     char arr[10];
    //     scanf("%s",&arr);
    //     if(strncmp(arr,"-1",2) == 0) break;
    //     addVariable(arr);
    // }

    yyin = fopen("input.txt","r");
    yylex();

    if(!isMainBlockFound){
        stopProgram("Incomplete main block");
    }

    for(int i=0; i<varSize; i++){
        printf("%s : %3lf\n",vars[i].name,vars[i].value);
    }
    stopProgram("");

    if( strlen(error) == 0 ){
        printf("Total comment: %d\n",commentCounter);
        printf("Program compiled successfully\n");
    }
    else{
        printf("Error: %s\n",error);
    }

}
