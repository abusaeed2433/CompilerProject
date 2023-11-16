%{
    #include<stdio.h>
    #include<math.h>
    #include<string.h>

    int yylex(void);
	void yyerror(char *s);
%}

%union{
    double num;
    char *name;
}

%error-verbose
%token ENTRY_POINT END_POINT TYPE

// defining token type
//%type<TYPE> ID1 ID2

// grammar rules
%%
program:ENTRY_POINT END_POINT     { printf("program executed"); }
    | block
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