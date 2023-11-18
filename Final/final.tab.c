
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "final.y"

    #include<stdio.h>
    #include<math.h>
    #include<string.h>
    #include<stdlib.h>
    #include <ctype.h>
    #include "prototype_list.h"
    #include "constant.h"
    #include "var_list.h"
    #include<windows.h>


    extern char lastDataType[10];

    int yylex(void);
	void yyerror(char *s);

    struct PARAMETER *paramHead = NULL;
    struct PARAMETER *paramTail = NULL;

    struct PARAMETER *callParamHead = NULL;
    struct PARAMETER *callParamTail = NULL;

    extern char *outBuffer;
    extern int outBufferSize;
    extern bool isLastIfValid;

    struct COND{ char *left, *op, *right; };
    struct ASSIGN{ char *var, *left, *op, *right; };

    struct COND lastCond;
    struct ASSIGN lastAssign;

    struct FUNC_RESULT { 
        char type[10];
        double res;
    } lastFuncRes;


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
        //printProto(proto);

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

    bool isNumber(char* num){
        bool count = 0;
        for(int i=0; i<strlen(num); i++){
            if( num[i] == '.' && count == 0 ) { count=1; continue; }
            if( !isdigit(num[i]) ) return false;
        }
        return true;
    }

    bool evaluateCondition(char* leftChar, char* op, char *rightChar){

        lastCond.left = leftChar;
        lastCond.op = op;
        lastCond.right = rightChar;

        double left = 0, right = 0;
        
        if( isNumber(leftChar) ){ 
            left = strtod(leftChar,NULL);
        }
        else{
            if( !doesVariableExists(leftChar) ){
                printf("\nVariable %s doesn't exist. Using default value(0)\n",leftChar);
            }
            left = getValueOrDefault(leftChar);
        }

        
        if( isNumber(rightChar) ){
            right = strtod(rightChar,NULL);
        }
        else{
            if( !doesVariableExists(rightChar) ){
                printf("\nVariable %s doesn't exist. Using default value(0)\n",rightChar);
            }
            right = getValueOrDefault(rightChar);
        }

        return isConditionValid(left, op, right);
    }

    void updateLast(char *left, char *op, char *right){
        lastAssign.left = left;
        lastAssign.op = op;
        lastAssign.right = right;
    }

    char* dtoc(double val){
        char* buf = malloc( sizeof(char) * 20 );
        sprintf(buf, "%f", val);
        return buf;
    }

    void startLoop(){
        int counter = 0;
        printf("\n");
        while( evaluateCondition( lastCond.left, lastCond.op, lastCond.right ) ){
            
            printf("Iterating - %d",counter);

            if( !isNumber(lastCond.left) ){
                printf(", value: %s", getFormattedValueOrDefault(lastCond.left) );
            }
            printf("\n");

            double val1 = 0, val2 = 0;
            if( isNumber(lastAssign.left) ){
                sscanf(lastAssign.left, "%lf", &val1);
            }
            else{
                val1 = getValueOrDefault(lastAssign.left);
            }

            if( isNumber(lastAssign.right) ){
                sscanf(lastAssign.right, "%lf", &val2);
            }
            else{
                val2 = getValueOrDefault(lastAssign.right);
            }

            if( !doesVariableExists(lastAssign.var) ){
                printf("\nVariable %s doesn't exists. Stopping...\n", lastAssign.var);
                break;
            }
            double res = doArithOperation(val1, val2, lastAssign.op);

            updateVariable( lastAssign.var, res );
            counter++;
            Sleep(200);
        }
    }

    void addTypeToCall(char *name){
        //printf("Type call %s\n",name);

        char *type = "none";

        if( isNumber(name) ){ type = "any"; }
        else{

            if( doesVariableExists(name) ){
                struct VARIABLE* var = getVariable(name);
                type = var->type;
            }
            else{
                printf("\nVariable %s doesn't exists\n",name);
            }

        }

        insertParameter(&callParamHead, &callParamTail, type);

    }


    void processCall(char *funcName){

        char realName[ strlen(funcName) ];

        // Copy the string starting from the second character
        strcpy(realName, funcName + 1);

        struct PROTOTYPE* temp = createProto("void",realName, callParamHead,NULL);
        
        strcpy( lastFuncRes.type, "null" );
        lastFuncRes.res = 0;

        printf("\n function processed: ");
        printProto( temp, false );

        if( !doesProtoExists(temp) ){
            printf("\nFunction not found\n");
        }
        else{
            printf("\nCalling function ");
            struct PROTOTYPE* orig = getOriginalProto(temp);
            strcpy( lastFuncRes.type, orig->funcType);
            printProto( orig , true);
        }
        callParamHead = NULL;
        callParamTail = NULL;
    }

    void assignFromFunction(char* name, char *varType, bool update){
        double val = 0;
        if( strncmp("void",lastFuncRes.type,4) == 0 ){
            printf("\nInvalid assignment from void to %s\n",lastDataType);
            val = 0;
        }
        else if( strcmp(varType, lastFuncRes.type) == 0 ){
            val = lastFuncRes.res;
        }
        else if( strncmp("int", lastDataType, 3) != 0 ){
            val = lastFuncRes.res;
        }
        else{
            printf("\nImcompatible assignment. Using default value...\n");
        }
        
        if(update){
            assignIfPossible(name,val);
        }
        else{
            declareVariable(name, lastDataType, val);
        }

    }



/* Line 189 of yacc.c  */
#line 415 "final.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ENTRY_POINT = 258,
     END_POINT = 259,
     DATA_TYPE = 260,
     FUNC_TYPE = 261,
     VAR = 262,
     NUMBER = 263,
     ARITH_OPE = 264,
     DISCARD = 265,
     FUNC_NAME = 266,
     OUTPUT = 267,
     OUTPUT_VC = 268,
     OUTPUT_SEP = 269,
     OUTPUT_END = 270,
     JUST_IN_CASE = 271,
     COND_OPE = 272,
     OR = 273,
     AND = 274,
     VAR_CON = 275,
     TILL = 276
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 342 "final.y"

    double num;
    char *name;
    bool myBool;



/* Line 214 of yacc.c  */
#line 480 "final.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 492 "final.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   119

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  50
/* YYNRULES -- Number of states.  */
#define YYNSTATES  112

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   276

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      22,    23,     2,     2,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    25,
       2,    26,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,     9,    16,    17,    20,    24,    25,
      29,    33,    37,    40,    43,    47,    51,    56,    57,    60,
      64,    68,    70,    74,    78,    82,    86,    92,    98,   104,
     108,   114,   119,   124,   129,   134,   137,   141,   144,   146,
     150,   157,   164,   169,   171,   175,   179,   183,   187,   191,
     195
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      29,     0,    -1,    30,     3,    32,     4,    -1,    -1,    30,
       6,    11,    22,    31,    23,    -1,    -1,     6,    31,    -1,
       6,    24,    31,    -1,    -1,    16,    41,    32,    -1,    21,
      42,    32,    -1,    33,    25,    32,    -1,    35,    32,    -1,
      37,    32,    -1,    10,    38,    32,    -1,    12,    39,    32,
      -1,    11,    22,    34,    23,    -1,    -1,    34,    20,    -1,
      34,    24,    20,    -1,     5,    36,    25,    -1,     7,    -1,
       7,    24,    36,    -1,     7,    26,     8,    -1,     7,    26,
       7,    -1,     7,    26,    33,    -1,     7,    26,    33,    24,
      36,    -1,     7,    26,     8,    24,    36,    -1,     7,    26,
       7,    24,    36,    -1,     7,    26,    46,    -1,     7,    26,
      46,    24,    36,    -1,     7,    26,     8,    25,    -1,     7,
      26,     7,    25,    -1,     7,    26,    33,    25,    -1,     7,
      26,    46,    25,    -1,     7,    25,    -1,     7,    24,    38,
      -1,    40,    15,    -1,    13,    -1,    40,    14,    13,    -1,
      22,    44,    23,    27,    32,     4,    -1,    22,    45,    23,
      27,    43,     4,    -1,     7,    26,    46,    25,    -1,    45,
      -1,    44,    18,    45,    -1,    44,    19,    45,    -1,    20,
      17,    20,    -1,     7,     9,     7,    -1,     7,     9,     8,
      -1,     8,     9,     7,    -1,     8,     9,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   368,   368,   371,   372,   376,   377,   378,   382,   383,
     384,   385,   386,   387,   388,   389,   392,   396,   397,   398,
     401,   403,   406,   409,   412,   416,   419,   422,   425,   429,
     432,   437,   440,   449,   452,   457,   460,   464,   466,   467,
     470,   473,   479,   486,   487,   488,   491,   497,   503,   510,
     516
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ENTRY_POINT", "END_POINT", "DATA_TYPE",
  "FUNC_TYPE", "VAR", "NUMBER", "ARITH_OPE", "DISCARD", "FUNC_NAME",
  "OUTPUT", "OUTPUT_VC", "OUTPUT_SEP", "OUTPUT_END", "JUST_IN_CASE",
  "COND_OPE", "OR", "AND", "VAR_CON", "TILL", "'('", "')'", "','", "';'",
  "'='", "'{'", "$accept", "program", "many_proto_type", "many_type",
  "block", "func_call", "many_var_con", "var_declare", "others",
  "var_assign", "discard_var", "out_sec", "out_vc", "if_sec", "loop_sec",
  "loop_updater", "many_logic_cond", "logic_cond", "arith_exp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    40,    41,    44,    59,    61,   123
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    28,    29,    30,    30,    31,    31,    31,    32,    32,
      32,    32,    32,    32,    32,    32,    33,    34,    34,    34,
      35,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    37,    37,    37,    37,    38,    38,    39,    40,    40,
      41,    42,    43,    44,    44,    44,    45,    46,    46,    46,
      46
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     6,     0,     2,     3,     0,     3,
       3,     3,     2,     2,     3,     3,     4,     0,     2,     3,
       3,     1,     3,     3,     3,     3,     5,     5,     5,     3,
       5,     4,     4,     4,     4,     2,     3,     2,     1,     3,
       6,     6,     4,     1,     3,     3,     3,     3,     3,     3,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     8,     8,     0,    21,     0,
       0,     0,     8,    17,    38,     8,     0,     0,     8,     0,
       8,     2,     8,    12,    13,     5,     0,     0,    20,     0,
       0,     0,     0,     0,    35,    14,     0,    15,     0,    37,
       0,     0,    43,     9,     0,    10,    11,     5,     0,    22,
      24,    23,    25,    29,     0,    32,     0,    31,    33,    34,
      36,    18,    16,     0,    39,     0,     0,     0,     0,     0,
       5,     6,     4,     0,     0,     0,     0,    47,    48,    49,
      50,    19,    46,    44,    45,     8,     0,     7,    28,    27,
      26,    30,     0,     0,     0,    40,     0,    41,     0,     0,
       0,    42
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    58,    13,    14,    46,    15,    19,    16,
      22,    25,    26,    28,    30,   104,    51,    52,    42
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -54
static const yytype_int8 yypact[] =
{
     -54,    11,    32,   -54,     9,     7,    47,    14,    51,    48,
      56,    50,    52,    64,    53,     9,     9,    54,    33,    58,
      26,    24,     9,   -54,   -54,     9,    46,    61,     9,    61,
       9,   -54,     9,   -54,   -54,    67,    47,    34,   -54,    -3,
       3,    60,    62,    51,   -54,   -54,    23,   -54,    66,   -54,
      65,    13,   -54,   -54,    63,   -54,   -54,    -1,    68,   -54,
      15,    20,    69,    70,    55,   -54,    57,   -54,   -54,   -54,
     -54,   -54,   -54,    72,   -54,    75,    61,    61,    71,    73,
      67,   -54,   -54,    47,    47,    47,    47,   -54,   -54,   -54,
     -54,   -54,   -54,   -54,   -54,     9,    77,   -54,   -54,   -54,
     -54,   -54,    84,    49,    85,   -54,    59,   -54,    81,    87,
      74,   -54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -54,   -54,   -54,   -53,   -15,   -11,   -54,   -54,   -33,   -54,
      76,   -54,   -54,   -54,   -54,   -54,   -54,   -21,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      33,    34,    63,    59,    81,    57,    64,    45,    54,    41,
      47,     3,    66,    53,     6,    55,     7,    56,    17,     8,
       9,    10,    65,    80,    64,    11,    62,    97,    67,    66,
      12,    76,    77,    39,    40,     4,    78,     9,     5,    83,
      20,    60,    61,    71,    84,     9,    72,    73,    43,    44,
      98,    99,   100,   101,    18,    93,    94,    36,    21,    37,
      48,    49,    87,    88,    89,    90,   108,   109,    31,    24,
      23,   110,    27,    57,    29,   106,    35,     0,    32,    74,
     102,    50,    75,    38,   103,    68,    79,    69,   105,   107,
      64,    82,    91,    85,    86,    92,    66,     0,    95,   111,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70
};

static const yytype_int8 yycheck[] =
{
      15,    16,    37,    36,    57,     6,     9,    22,    29,    20,
      25,     0,     9,    28,     5,    30,     7,    32,    11,    10,
      11,    12,    25,    24,     9,    16,    37,    80,    25,     9,
      21,    18,    19,     7,     8,     3,    23,    11,     6,    24,
      26,     7,     8,    20,    24,    11,    23,    24,    24,    25,
      83,    84,    85,    86,     7,    76,    77,    24,     7,    26,
      14,    15,     7,     8,     7,     8,     7,     8,     4,    13,
      22,   106,    22,     6,    22,    26,    22,    -1,    25,    13,
      95,    20,    17,    25,     7,    25,    23,    25,     4,     4,
       9,    23,    20,    24,    24,    20,     9,    -1,    27,    25,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    29,    30,     0,     3,     6,     5,     7,    10,    11,
      12,    16,    21,    32,    33,    35,    37,    11,     7,    36,
      26,     7,    38,    22,    13,    39,    40,    22,    41,    22,
      42,     4,    25,    32,    32,    22,    24,    26,    25,     7,
       8,    33,    46,    24,    25,    32,    34,    32,    14,    15,
      20,    44,    45,    32,    45,    32,    32,     6,    31,    36,
       7,     8,    33,    46,     9,    25,     9,    25,    25,    25,
      38,    20,    23,    24,    13,    17,    18,    19,    23,    23,
      24,    31,    23,    24,    24,    24,    24,     7,     8,     7,
       8,    20,    20,    45,    45,    27,    27,    31,    36,    36,
      36,    36,    32,     7,    43,     4,    26,     4,     7,     8,
      46,    25
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 368 "final.y"
    { printf("program executed"); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 372 "final.y"
    { 
        processProto((yyvsp[(2) - (6)].name),(yyvsp[(3) - (6)].name));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 377 "final.y"
    { initProto((yyvsp[(1) - (2)].name)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 378 "final.y"
    { initProto((yyvsp[(1) - (3)].name)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 382 "final.y"
    { printf("\nempty block\n"); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 383 "final.y"
    {;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 384 "final.y"
    {;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 385 "final.y"
    {;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 386 "final.y"
    {;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 387 "final.y"
    {;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 388 "final.y"
    {;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 389 "final.y"
    {;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 392 "final.y"
    { 
        processCall((yyvsp[(1) - (4)].name)); (yyval.name) = lastFuncRes.type;
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 397 "final.y"
    { addTypeToCall((yyvsp[(2) - (2)].name)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 398 "final.y"
    { addTypeToCall((yyvsp[(3) - (3)].name)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 403 "final.y"
    {
        declareVariable((yyvsp[(1) - (1)].name), lastDataType, 0);
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 406 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, 0);
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 409 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, (yyvsp[(3) - (3)].num));
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 412 "final.y"
    {
        double val = getValueOrDefault((yyvsp[(3) - (3)].name));
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, val);
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 416 "final.y"
    {
        assignFromFunction((yyvsp[(1) - (3)].name),lastDataType,false);
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 419 "final.y"
    {
        assignFromFunction((yyvsp[(1) - (5)].name),lastDataType,false);
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 422 "final.y"
    {
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, (yyvsp[(3) - (5)].num));
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 425 "final.y"
    {
        double val = getValueOrDefault((yyvsp[(3) - (5)].name));
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, val);
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 429 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, (yyvsp[(3) - (3)].num));
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 432 "final.y"
    {
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, (yyvsp[(3) - (5)].num));
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 437 "final.y"
    {
        assignIfPossible((yyvsp[(1) - (4)].name),(yyvsp[(3) - (4)].num));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 440 "final.y"
    {
        if( !doesVariableExists((yyvsp[(3) - (4)].name)) ){
            printf("\nVariable %s doesn't exist\n",(yyvsp[(3) - (4)].name));
        }
        else{
            double val = getValueOrDefault((yyvsp[(3) - (4)].name));
            assignIfPossible((yyvsp[(1) - (4)].name), val);
        }
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 449 "final.y"
    {
        assignFromFunction((yyvsp[(1) - (4)].name),lastDataType,true);
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 452 "final.y"
    {
        assignIfPossible((yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].num));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 457 "final.y"
    {
        discardVariable((yyvsp[(1) - (2)].name));
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 460 "final.y"
    {
        discardVariable((yyvsp[(1) - (3)].name));
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 464 "final.y"
    { printAndClearOutBuffer(); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 466 "final.y"
    { continueOutBuffer((yyvsp[(1) - (1)].name)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 467 "final.y"
    { continueOutBuffer((yyvsp[(3) - (3)].name)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 470 "final.y"
    { printf("\nif processed\n"); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 473 "final.y"
    {
        printf("\nLoop matched\n");
        startLoop();
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 479 "final.y"
    { 
        lastAssign.var = (yyvsp[(1) - (4)].name);
        //printf("=== %s %s %s ===",lastCond.left,lastCond.op,lastCond.right);
        //printf("=== %s %s %s %s ===", lastAssign.var,lastAssign.left,lastAssign.op,lastAssign.right);
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 486 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (1)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 487 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (3)].myBool) || (yyvsp[(3) - (3)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 488 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (3)].myBool) && (yyvsp[(3) - (3)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 491 "final.y"
    {
        (yyval.myBool) = evaluateCondition((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        //printf("------%s %s %s %d\n",$1, $2, $3, $$);
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 497 "final.y"
    {
        double val1 = getValueOrDefault((yyvsp[(1) - (3)].name));
        double val2 = getValueOrDefault((yyvsp[(3) - (3)].name));
        updateLast((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        (yyval.num) = doArithOperation(val1, val2, (yyvsp[(2) - (3)].name));
    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 503 "final.y"
    {
        double val1 = getValueOrDefault((yyvsp[(1) - (3)].name));
        
        updateLast((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), dtoc((yyvsp[(3) - (3)].num)) );
        (yyval.num) = doArithOperation(val1, (yyvsp[(3) - (3)].num), (yyvsp[(2) - (3)].name));

    ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 510 "final.y"
    { 
        double val2 = getValueOrDefault((yyvsp[(3) - (3)].name));
        
        updateLast( dtoc((yyvsp[(1) - (3)].num)), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        (yyval.num) = doArithOperation((yyvsp[(1) - (3)].num), val2, (yyvsp[(2) - (3)].name));
    ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 516 "final.y"
    {
        updateLast( dtoc((yyvsp[(1) - (3)].num)), (yyvsp[(2) - (3)].name), dtoc((yyvsp[(3) - (3)].num)) );
        (yyval.num) = doArithOperation((yyvsp[(1) - (3)].num), (yyvsp[(3) - (3)].num), (yyvsp[(2) - (3)].name));
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2163 "final.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 524 "final.y"



void yyerror(char *s)
{
	fprintf(stderr, "\n%s", s);
}

int main(){

	freopen("input2.txt","r",stdin);
	//output = freopen("output.txt", "w", stdout); // output in file
	yyparse();
    printAll();
    printf("\nPrinting all prototype\n");
    printAllProto();
	return 0;
}

