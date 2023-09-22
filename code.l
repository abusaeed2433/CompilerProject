%{
    #include<stdio.h>
    #include<string.h>
    #include <stdlib.h>

    char error[200];

    int commentCounter = 0;
    int comment_depth = 0;
    char *comment_buffer = NULL;
    int canDeclareHeader = 1;

    size_t buffer_length = 0;


    void runHeader(int check){
        if(!check){
            printf("Invalid header format\n");
            exit(1);
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
        canDeclareHeader = 0;
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
        printf("Captured comment: %s\n", comment_buffer);
    }
    
    void checkForEnd(){
        if (comment_depth == 0) {
            BEGIN(INITIAL);
            process_comment();
            resetBuffer();
        }
        else if(comment_depth < 0){
            resetBuffer();
            printf("Invalid comment found\n");
            exit(1);
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

%}


%x HEADER
%x COMMENT
HEADER_START "import "
HEADER_END [a-zA-Z.]+";"


SINGLE_LINE_COMMENT ("//").*(\n)?


VARIABLE [a-zA-Z_][a-zA-Z0-9_]*
DATA_TYPE ["int"|"float"|"double"|"string"]
NEW_LINE_AND_TAB [\n\t]*

%%

{HEADER_START} { BEGIN(HEADER); }

<HEADER>{HEADER_END} { runHeader(1); BEGIN(INITIAL); }
<HEADER>.* { runHeader(0); BEGIN(INITIAL); }


{SINGLE_LINE_COMMENT} { initSingleComment(); }

"/*" { initMultiComment(); BEGIN(COMMENT); }

<COMMENT>"/*" { appendTextToBuffer("/*"); }
<COMMENT>"*/" { appendTextToBuffer("*/"); checkForEnd(); }

<COMMENT>. { appendToBuffer(yytext[0]); }
<COMMENT>\n { appendTextToBuffer("\n"); }

{NEW_LINE_AND_TAB} { }

%%

int yywrap(){
    return 1;
}

int main(){
    yyin = fopen("input.txt","r");
    yylex();

    if( strlen(error) == 0 ){
        printf("Total comment: %d\n",commentCounter);
        printf("Program compiled successfully\n");
    }
    else{
        printf("Error: %s\n",error);
    }

}
