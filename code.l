%{
    #include<stdio.h>
    #include<string.h>
    #include <stdlib.h>
    #include <stdbool.h>

    char error[200];
    char lastDataType[10];

    int commentCounter = 0;
    int comment_depth = 0;
    char *comment_buffer = NULL;

    bool canDeclareHeader = true;
    bool isMainBlockFound = false;
    bool isCommingFromMain = false;

    size_t buffer_length = 0;

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
    
    void stopProgram(char *error){
        printf("%s\n",error);
        free(vars);
        free(comment_buffer);
        exit(1);
    }

    char* removeRedundant(char *source){
        int len = strlen(source);

        char temp[len+1];
        int j = 0;
        for(int i=0; i<len; i++){
            if(source[i] == ' ' || source[i] == ',') continue;
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

%}


%x HEADER
%x COMMENT
%x MAIN
%x VAR_SEC

HEADER_START "import "
HEADER_END [a-zA-Z.]+";"


MAIN_START "static void main(){"
MAIN_END "}"

SINGLE_LINE_COMMENT ("//").*(\n)?

VARIABLE_ONLY [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*(,)?
VARIABLE_VALUE [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*("=")[ ]*[0-9]+(,)?
VARIABLE_STATEMENT_END [ ]*;

DATA_TYPE ("int "|"float "|"double "|"string ")
NEW_LINE_AND_TAB [\n\t]*

%%

{HEADER_START} { BEGIN(HEADER); }

<HEADER>{HEADER_END} { runHeader(1); BEGIN(INITIAL); }
<HEADER>.* { runHeader(0); BEGIN(INITIAL); }

{SINGLE_LINE_COMMENT} { initSingleComment(); }

"/*" { initMultiComment(); BEGIN(COMMENT); }

<COMMENT>"/*" { appendTextToBuffer("/*"); }
<COMMENT>"*/" { 
    appendTextToBuffer("*/"); checkForEnd(); 
    if(comment_depth == 0 && isCommingFromMain){ BEGIN(MAIN); isCommingFromMain = false; } 
}

<COMMENT>. { appendToBuffer(yytext[0]); }
<COMMENT>\n { appendTextToBuffer("\n"); }

<VAR_SEC>{VARIABLE_ONLY} { declareVariable(yytext,0); }
<VAR_SEC>{VARIABLE_VALUE} { processVariable(yytext); }
<VAR_SEC>{VARIABLE_STATEMENT_END} { isCommingFromMain = true; BEGIN(MAIN); }
<VAR_SEC>. {stopProgram("Invalid character found");}

{MAIN_START} { initMain(); BEGIN(MAIN); }
<MAIN>{DATA_TYPE} { initVarSec(yytext); isCommingFromMain = true; BEGIN(VAR_SEC); }


<MAIN>{NEW_LINE_AND_TAB} {}
<MAIN>{MAIN_END} {stopMain(); BEGIN(INITIAL); }
<MAIN>{SINGLE_LINE_COMMENT} { initSingleComment(); }
<MAIN>"/*" {initMultiComment(); isCommingFromMain = true; BEGIN(COMMENT); }

{NEW_LINE_AND_TAB} { }

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
