%{
    #include<stdio.h>
    #include<string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <ctype.h>
    #include "final.tab.h"

    // header, single, multi, var, print, dis, calc_val, if, till
    int tempCounter[10];

    int blockBalancer = 0;
    int stack[100];
    int stackSize = 0;

    const int MAXIMUM_VAR_SIZE = 10;

    char error[200];

    // extern
    char lastDataType[10];
    char *outBuffer = NULL;
    int outBufferSize = 0;
    //extern

    int commentCounter = 0;
    int comment_depth = 0;
    char *comment_buffer = NULL;
    
    bool canDeclareHeader = true;
    bool isMainBlockFound = false;
    bool isCommingFromMain = false;
    bool isCommingFromVar = false;
    bool isLastIfValid = false;

    size_t buffer_length = 0;

    int bracketCounter=0;

    char *removeRedundant(char*);

    // variable
    const int MAXIMUM_VARIABLE_LENGTH = 200;

    int pushState(int id){
        stackSize++;
        stack[stackSize] = id;
        return id;
    }

    int popState(){
        if(stackSize > 0) stackSize--;
        return stack[stackSize];
    }

    void inc(int index){
        tempCounter[index]++;
    }

    void initSingleComment(){
        commentCounter++;
        printf("\nSingle line comment: %s\n",yytext);
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
        comment_depth = 1; 
        buffer_length = 2;
        comment_buffer = (char*) malloc(3);

        memset(comment_buffer, 0, 3);
        strcat(comment_buffer, "/*");
    }

    void resetBuffer(){
        free(comment_buffer);
        comment_buffer = NULL;
        buffer_length = 0;
    }

    void process_comment() {
        commentCounter++;
        printf("\nCaptured comment: %s\n\n", comment_buffer);
        //printf("Multi-line comment found\n");
    }

    void stopProgram(char *error){
        printf("%s\n",error);
        free(comment_buffer);
        exit(1);
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

    void initMain(){
        canDeclareHeader = false;
        //printf("execution started\n");
    }

    void processHeader(){
        if(!canDeclareHeader){
            stopProgram("Header must be at top");
            return;
        }
        printf("header: -%s-\n",yytext); 
    }

    void initVarSec(char *temp){
        char *type = removeRedundant(temp);
        strcpy(lastDataType,type);
        canDeclareHeader = false;
    }

    void sendNumber(char *text){
        double num = 0;
        sscanf(text, "%lf", &num);
        yylval.num = num;
        //printf("======%lf=====",num);
    }

    void initOutBuffer(){
        outBuffer = (char*) malloc(1);
        memset(outBuffer, 0, 1);
        outBufferSize = 1;
    }

%}

%x HEADER
%x COMMENT
%x MAIN
%x VAR_SEC
%x VAR_CALC_SEC
%x IF_SEC
%x IGNORE_SEC
%x DISCARD_SEC
%x LOOP_SEC
%x OUT_SEC
%x PROTO_SEC

DQ \"
NUMBER ([0-9]+(".")[0-9]+)|([0-9]+)|("."[0-9]+)
VARIABLE [a-zA-Z][a-zA-Z0-9]*
VAR_NUM ({NUMBER}|{VARIABLE})
COND_OP ("lt"|"gt"|"eq"|"neq"|"le"|"ge")
ARITH_OP ("add"|"sub"|"mul"|"div"|"dif"|"rem")

HEADER "import "[a-zA-Z.]+";"

OUT_START "println("[ ]*
OUT_BODY_CONST {DQ}[^\"]*{DQ}
OUT_BODY_VAR {VAR_NUM}
OUT_SEP [ ]*(","|" ")[ ]*
OUT_END [ ]*")"[ ]*;[ ]*

MAIN_START "static void entryPoint"[ ]*"("[ ]*")"[ ]*"{"[ ]*
MAIN_END "}"

SINGLE_LINE_COMMENT ("//").*(\n)?

VARIABLE_ONLY [ ]*{VARIABLE}*[ ]*(,)
VARIABLE_VALUE [ ]*{VARIABLE}[ ]*("=")[ ]*{NUMBER}(,)
VARIABLE_VALUE_ASSIGN_CONST [ ]*{VARIABLE}[ ]*("=")[ ]*{NUMBER};
VARIABLE_VALUE_ASSIGN_VAR [ ]*{VARIABLE}[ ]*("=")[ ]*{VARIABLE};
VARIABLE_VALUE_ASSIGN_CALC [ ]*{VARIABLE}[ ]*("=")[ ]*{VAR_NUM}[ ]*{ARITH_OP}[ ]*{VAR_NUM}[ ]*,
VARIABLE_VALUE_ASSIGN_CALC_LAST [ ]*{VARIABLE}[ ]*("=")[ ]*{VAR_NUM}[ ]*{ARITH_OP}[ ]*{VAR_NUM}[ ]*;

DISCARD_START "discard "

VARIABLE_ONLY_LAST [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*(;)
VARIABLE_VALUE_LAST [ ]*[a-zA-Z][a-zA-Z0-9]*[ ]*("=")[ ]*{NUMBER}(;)

VAR_SPACE [ ]*

DATA_TYPE ("int"|"float"|"double")[ ]
FUNC_TYPE ("int"|"float"|"double"|"void")

IF "justInCase"[ ]*
IF_COND ("(")[ ]*{VAR_NUM}[ ]*{COND_OP}[ ]*{VAR_NUM}[ ]*(")")[ ]*
IF_BODY_START [ ]*"{"
IF_BODY_END [ ]*"}"
IF_SPACE [ ]*

IGNORE_LEFT_BRACE "{"
IGNORE_RIGHT_BRACE "}"

LOOP_START "till"
LOOP_START_BRACE "{"
LOOP_COND {IF_COND}
LOOP_OTHERS [^{}\n]*\n
LOOP_END {IF_BODY_END}

NEW_LINE_AND_TAB [\n\t]*

%%
{SINGLE_LINE_COMMENT} { inc(1); initSingleComment(); }
"/*" { initMultiComment(); BEGIN( pushState(COMMENT) ); }

<COMMENT>"/*" { appendTextToBuffer("/*"); }

<COMMENT>"*/" { 
    appendTextToBuffer("*/"); checkForEnd(); 
    if(comment_depth == 0){ inc(2); BEGIN( popState() ); }
}

<COMMENT>{NEW_LINE_AND_TAB}* { appendTextToBuffer("|"); }

<COMMENT>. { appendToBuffer(yytext[0]); }

<VAR_SEC>"/*" { initMultiComment(); BEGIN( pushState(COMMENT) ); }

<MAIN>{SINGLE_LINE_COMMENT} { inc(1); initSingleComment(); }

<MAIN>"/*" { initMultiComment();  BEGIN( pushState(COMMENT) ); }

{HEADER} { inc(0); processHeader();}

{FUNC_TYPE} { 
    //printf("m_type: -%s-\n",yytext);
    BEGIN( pushState(PROTO_SEC) ); 
    char *type = removeRedundant(yytext);
    yylval.name =  strdup( type );
    return FUNC_TYPE;
}

<PROTO_SEC>{FUNC_TYPE} { 
    //printf("type: -%s-\n",yytext); 
    char *type = removeRedundant(yytext);
    yylval.name =  strdup( type );
    return FUNC_TYPE; 
}
<PROTO_SEC>{VARIABLE} { 
    //printf("name: -%s-\n",yytext); 
    yylval.name = strdup(yytext);
    return FUNC_NAME; 
}

<PROTO_SEC>"," { return *yytext; }
<PROTO_SEC>"(" { return *yytext; }
<PROTO_SEC>");" { BEGIN( popState() ); return *yytext; }

{MAIN_START} { 
    blockBalancer++;
    //printf("main start %s\n",yytext); 
    initMain(); 
    BEGIN( pushState(MAIN) );
    return ENTRY_POINT;
}


<MAIN>{DATA_TYPE} {
    //printf("main data type %s\n",yytext);
    initVarSec(yytext); 
    char *type = removeRedundant(yytext);
    yylval.name =  strdup( type );
    BEGIN( pushState(VAR_SEC) );
    return DATA_TYPE;
}

<MAIN>{VARIABLE} {
    yylval.name = strdup(yytext);
    BEGIN( pushState(VAR_SEC) );
    return VAR;
}

<VAR_SEC>{NUMBER} {
    sendNumber(yytext);
    return NUMBER;
}
<VAR_SEC>{ARITH_OP} {
    yylval.name = strdup(yytext);
    return ARITH_OPE;
}
<VAR_SEC>{VARIABLE} {
    yylval.name = strdup(yytext);
    return VAR;
}
<VAR_SEC>"," { return *strdup(yytext); }
<VAR_SEC>"=" { return *strdup(yytext); }
<VAR_SEC>";" {
    BEGIN( popState() );
    return *strdup(yytext);
}
<VAR_SEC>{VAR_SPACE} {}


<MAIN>{DISCARD_START} {
    //printf("Discard section\n");
    inc(5);
    BEGIN( pushState(DISCARD_SEC) );
    return DISCARD;
}

<DISCARD_SEC>{VARIABLE} {
    //printf("\nFrom discard only:-%s-\n",yytext);
    yylval.name = strdup(yytext);
    return VAR;
}
<DISCARD_SEC>"," { return *yytext; }
<DISCARD_SEC>";" { BEGIN( popState() ); return *yytext; }


<MAIN>{OUT_START} {  
    initOutBuffer();
    BEGIN( pushState(OUT_SEC) );
    return OUTPUT;
}
<OUT_SEC>{OUT_BODY_CONST} { 
    yylval.name = strdup(yytext);
    return OUTPUT_VC;
}
<OUT_SEC>{OUT_BODY_VAR} { 
    yylval.name = strdup(yytext);
    return OUTPUT_VC;
}
<OUT_SEC>{OUT_SEP} {
    return OUTPUT_SEP;
}
<OUT_SEC>{OUT_END} {
    inc(4);
    BEGIN( popState() );
    return OUTPUT_END;
 }


<MAIN>{MAIN_END} {
    blockBalancer--;
    BEGIN( popState() );
    return END_POINT;
}

<MAIN>{NEW_LINE_AND_TAB}* { }
{NEW_LINE_AND_TAB}* { }

%%

int yywrap(){
    return 1;
}
