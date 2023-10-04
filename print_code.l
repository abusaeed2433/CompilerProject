%{
    // push state to stack and return same
    int pushState(int id);
    // first pop then return last state
    int popState();
    // true if last block was if-block
    bool isLastOneIf(int ifs);

    // print error, clear buffer and terminate program
    void stopProgram(char *error);
    // will remove redundant character like ' ', ',','\t','\n'
    char* removeRedundant(char *source);
    // initialize output buffer for adding more data
    void initOutBuffer();

    //print buffer and clear buffer
    void printAndClearOutBuffer();
    // remove leading space, ',' and add to outBuffer
    void continueOutBuffer(char *vc);
    // extract name and val from nameVal
    void extractNameValue(char *nameVal, char *name, double* val);

    // add variable after checking condition
    bool addVariable(char* name,char *type);
    // print header(1) or error(0) related to header
    void runHeader(int check);
    // indicate header section ends
    void exceptHeader();

    // initialize single line comment counter++
    void initSingleComment();
    // clear comment buffer and it's length
    void resetBuffer();
    // increases comment counter only
    void process_comment();

    // check if comment if closed or error if any
    void checkForEnd();
    // append ch to comment_buffer
    void appendToBuffer(char ch);
    // append string to comment_buffer
    void appendTextToBuffer(char* ch);

    // init multi-comment and setup buffer for it
    void initMultiComment();
    // print execution starting and changes flag
    void initMain();
    // print execution done and change flag
    void stopMain();
    
    // init var section and store last type
    void initVarSec(char *type);
    // called updateVariable internally
    void assignValue(char *name, double val);
    // call deleteVariable after removing redundant from name
    void removeVariable(char *name);

    // add variable, assign value or print error if any
    void declareVariable(char *name, double val);
    // extract name and val then declareVariable
    void processVariable(char *nameVal);
    // perform calculation on statment like a = b add 233
    void continueVariableCalc(char *stmt, bool addVar);

    // update variable value after extracing name and val
    void updateValue(char *nameVal);
    // delete variable with name
    void discardVariable(char *name);
    // true if num can be converted into number
    bool isNumber(char* num);

    // process if condition section and assign result to isLastIfValid if condition is valid
    void processIfBody(char *body);
    // just increase counter
    void inc(int index);
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

DQ \"
NUMBER ([0-9]+(".")[0-9]+)|([0-9]+)|("."[0-9]+)
VARIABLE [a-zA-Z][a-zA-Z0-9]*
VAR_NUM ({NUMBER}|{VARIABLE})
COND_OP ("lt"|"gt"|"eq"|"neq"|"le"|"ge")
ARITH_OP ("add"|"sub"|"mul"|"div"|"dif"|"rem")

HEADER_START "import "
HEADER_END [a-zA-Z.]+";"

OUT_START "println("[ ]*
OUT_BODY_CONST [ ]*{DQ}[^\"]*{DQ}(,)?[ ]*
OUT_BODY_VAR [ ]*{VAR_NUM}[ ]*(,)?
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
{HEADER_START} { BEGIN(pushState(HEADER)); }

<HEADER>{HEADER_END} { inc(0); runHeader(1); BEGIN( popState() ); }

<HEADER>. { runHeader(0); BEGIN(popState()); }

{SINGLE_LINE_COMMENT} { inc(1); initSingleComment(); }

"/*" { initMultiComment(); BEGIN( pushState(COMMENT) ); }

<COMMENT>"/*" { appendTextToBuffer("/*"); }
<COMMENT>"*/" { 
    appendTextToBuffer("*/"); checkForEnd(); 
    if(comment_depth == 0){
        inc(2);
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
<VAR_SEC>{VARIABLE_VALUE_ASSIGN_CALC} {
    //printf("\nassign from calc: %s\n",yytext);
    continueVariableCalc(yytext,true);
}
<VAR_SEC>{VARIABLE_VALUE_ASSIGN_CALC_LAST} {
    //printf("\nassign from calc: %s\n",yytext);
    inc(6);
    continueVariableCalc(yytext,true);
    BEGIN( popState() );
}
<VAR_SEC>{VARIABLE_VALUE} { processVariable(yytext); }
<VAR_SEC>{VARIABLE_VALUE_LAST} {
    //printf("\nvariable value last-%s-\n",yytext);
    processVariable(yytext);
    BEGIN( popState() );
}
<VAR_SEC>{VAR_SPACE} {}
<VAR_SEC>. { 
    printf("\nInvalid %s\n",yytext);
    stopProgram("Invalid character found");
}
<VAR_SEC>"/*" { 
    initMultiComment();
    BEGIN( pushState(COMMENT) );    
}


<MAIN>{IF} { inc(7); BEGIN( pushState(IF_SEC) ); }

<IF_SEC>{IF_COND} { inc(9); processIfBody(yytext); }
<IF_SEC>{IF_BODY_START} { 
    blockBalancer++;
    popState(); // popping if section and taking to main section arki
    if(isLastIfValid){
        BEGIN(pushState(MAIN));
    }
    else{
        bracketCounter=1;
        BEGIN( pushState(IGNORE_SEC) );
    }
}

<IF_SEC>. {}
<IGNORE_SEC>{IGNORE_LEFT_BRACE} { blockBalancer++; bracketCounter++; }
<IGNORE_SEC>{IGNORE_RIGHT_BRACE} { 
    blockBalancer--;
    bracketCounter--;
    if(bracketCounter == 0){
        BEGIN( popState() );
    }
}
<IGNORE_SEC>[^}] {}

{MAIN_START} { 
    blockBalancer++;
    //printf("main start %s\n",yytext); 
    initMain(); 
    BEGIN( pushState(MAIN) ); 
}
<MAIN>{DATA_TYPE} {
    //printf("main data type %s\n",yytext);
    initVarSec(yytext); 
    BEGIN( pushState(VAR_SEC) );
}
<MAIN>{VARIABLE_VALUE_ASSIGN_CONST} { 
    //printf("main assign %s\n",yytext);
    updateValue(yytext);
}
<MAIN>{VARIABLE_VALUE_ASSIGN_VAR} { 
    //printf("\nassign from var: %s\n",yytext);
    updateValue(yytext);
}

<MAIN>{VARIABLE_VALUE_ASSIGN_CALC_LAST} {
    inc(6);
    //printf("\nassign from calc: %s\n",yytext);
    continueVariableCalc(yytext,false);
}

<MAIN>{OUT_START} { initOutBuffer(); BEGIN( pushState(OUT_SEC) ); }

<OUT_SEC>{OUT_BODY_CONST} { continueOutBuffer(yytext); }
<OUT_SEC>{OUT_BODY_VAR} { continueOutBuffer(yytext); }
<OUT_SEC>{OUT_END} { inc(4); BEGIN( popState() ); printAndClearOutBuffer(); }

<MAIN>{MAIN_END} {
    blockBalancer--;
    //printf(" main end popped %s %d %d\n",yytext,val,MAIN);
    BEGIN( popState() );
}
<MAIN>{SINGLE_LINE_COMMENT} {
    inc(1);
    //printf("main comment %s\n",yytext);
    initSingleComment();
}
<MAIN>"/*" {
    //printf("main multi %s\n",yytext);
    initMultiComment(); 
    BEGIN( pushState(COMMENT) );
}
<MAIN>{NEW_LINE_AND_TAB}* { }

<MAIN>{DISCARD_START} {
    //printf("Discard section\n");
    inc(5);
    BEGIN( pushState(DISCARD_SEC) );
}

<MAIN>{LOOP_START} {
    inc(8);
    blockBalancer++;
    printf("\nloop start %s\n",yytext);
    BEGIN( pushState(LOOP_SEC) );
}
<MAIN>[^ ] {
    //printf("-%s-\n",yytext);
    stopProgram("Invalid character found\n");
}

<LOOP_SEC>{LOOP_COND} { inc(9); printf("\nloop cond: %s\n",yytext); }
<LOOP_SEC>{LOOP_START_BRACE} { printf("\nloop start {: %s\n",yytext); }
<LOOP_SEC>{LOOP_END} {
    blockBalancer--;
    printf("\nloop end: %s\n",yytext);
    BEGIN( popState() );
}
<LOOP_SEC>{NEW_LINE_AND_TAB} {}
<LOOP_SEC>{LOOP_OTHERS} { printf("\nloop others: %s\n",yytext); }

<DISCARD_SEC>{VARIABLE_ONLY} {
    //printf("\nFrom discard only:-%s-\n",yytext);
    discardVariable(yytext);
}
<DISCARD_SEC>{VARIABLE_ONLY_LAST} {
    //printf("\nFrom discard last:-%s-\n",yytext);
    discardVariable(yytext);
    int state = popState();
    //printf("\nState %d %d\n",MAIN,state);
    BEGIN( state );
}

<DISCARD_SEC>. { stopProgram("\nInvalid character while discarding\n"); }
. { 
    printf("invalid -%s-\n",yytext);
    stopProgram("Invalid char outside block");
}
{NEW_LINE_AND_TAB}* { }

%%

int yywrap(){
    return 1;
}


int main(){

    yyin = fopen("input.txt","r");
    yylex();

    if(blockBalancer != 0){ stopProgram("\nBlock not completed"); }

    tempCounter[3] = getTotalVar();
    printAll();

    if( strlen(error) == 0 ){
        printf("Total comment: %d\n",commentCounter);
        printf("Program compiled successfully\n");
    }
    else{ printf("Error: %s\n",error); }

    printf("\n----------------- Summary ------------------\n\n");
    printf("%-20s %d\n","Header:",tempCounter[0]);
    printf("%-20s %d\n","Single line comment:",tempCounter[1]);
    printf("%-20s %d\n","Multi line comment:",tempCounter[2]);
    printf("%-20s %d\n","Variable:",tempCounter[3]);
    printf("%-20s %d\n","println:",tempCounter[4]);
    printf("%-20s %d\n","discard:",tempCounter[5]);
    printf("%-20s %d\n","val_calculation:",tempCounter[6]);
    printf("%-20s %d\n","justInCase:",tempCounter[7]);
    printf("%-20s %d\n","till:",tempCounter[8]);
    printf("%-20s %d\n","condition:",tempCounter[9]);

}
