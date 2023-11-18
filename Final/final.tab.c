
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
        
        if( !doesProtoExists(temp) ){
            printf("\nInvalid function call\n");
        }
        else{
            printf("\nCalling function ");
            printProto( getOriginalProto(temp) , true);
        }
        callParamHead = NULL;
    }



/* Line 189 of yacc.c  */
#line 374 "final.tab.c"

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
#line 301 "final.y"

    double num;
    char *name;
    bool myBool;



/* Line 214 of yacc.c  */
#line 439 "final.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 451 "final.tab.c"

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
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  47
/* YYNRULES -- Number of states.  */
#define YYNSTATES  107

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
      29,    33,    36,    39,    42,    46,    50,    56,    57,    60,
      64,    68,    70,    74,    78,    82,    88,    94,    98,   104,
     109,   114,   119,   122,   126,   129,   131,   135,   142,   149,
     154,   156,   160,   164,   168,   172,   176,   180
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      29,     0,    -1,    30,     3,    32,     4,    -1,    -1,    30,
       6,    11,    22,    31,    23,    -1,    -1,     6,    31,    -1,
       6,    24,    31,    -1,    -1,    16,    41,    32,    -1,    21,
      42,    32,    -1,    33,    32,    -1,    35,    32,    -1,    37,
      32,    -1,    10,    38,    32,    -1,    12,    39,    32,    -1,
      11,    22,    34,    23,    25,    -1,    -1,    34,    20,    -1,
      34,    24,    20,    -1,     5,    36,    25,    -1,     7,    -1,
       7,    24,    36,    -1,     7,    26,     8,    -1,     7,    26,
       7,    -1,     7,    26,     8,    24,    36,    -1,     7,    26,
       7,    24,    36,    -1,     7,    26,    46,    -1,     7,    26,
      46,    24,    36,    -1,     7,    26,     8,    25,    -1,     7,
      26,     7,    25,    -1,     7,    26,    46,    25,    -1,     7,
      25,    -1,     7,    24,    38,    -1,    40,    15,    -1,    13,
      -1,    40,    14,    13,    -1,    22,    44,    23,    27,    32,
       4,    -1,    22,    45,    23,    27,    43,     4,    -1,     7,
      26,    46,    25,    -1,    45,    -1,    44,    18,    45,    -1,
      44,    19,    45,    -1,    20,    17,    20,    -1,     7,     9,
       7,    -1,     7,     9,     8,    -1,     8,     9,     7,    -1,
       8,     9,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   326,   326,   329,   330,   334,   335,   336,   340,   341,
     342,   343,   344,   345,   346,   347,   350,   352,   353,   354,
     357,   359,   362,   365,   368,   372,   375,   379,   382,   387,
     390,   399,   404,   407,   411,   413,   414,   417,   420,   426,
     433,   434,   435,   438,   444,   450,   457,   463
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
      35,    36,    36,    36,    36,    36,    36,    36,    36,    37,
      37,    37,    38,    38,    39,    40,    40,    41,    42,    43,
      44,    44,    44,    45,    46,    46,    46,    46
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     6,     0,     2,     3,     0,     3,
       3,     2,     2,     2,     3,     3,     5,     0,     2,     3,
       3,     1,     3,     3,     3,     5,     5,     3,     5,     4,
       4,     4,     2,     3,     2,     1,     3,     6,     6,     4,
       1,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     8,     8,     8,     0,    21,     0,
       0,     0,     8,    17,    35,     8,     0,     0,     8,     0,
       8,     2,    11,    12,    13,     5,     0,     0,    20,     0,
       0,     0,     0,    32,    14,     0,    15,     0,    34,     0,
       0,    40,     9,     0,    10,     5,     0,    22,    24,    23,
      27,     0,    30,     0,    29,    31,    33,    18,     0,     0,
      36,     0,     0,     0,     0,     0,     5,     6,     4,     0,
       0,     0,    44,    45,    46,    47,    16,    19,    43,    41,
      42,     8,     0,     7,    26,    25,    28,     0,     0,     0,
      37,     0,    38,     0,     0,     0,    39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    56,    13,    14,    45,    15,    19,    16,
      22,    25,    26,    28,    30,    99,    50,    51,    41
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -51
static const yytype_int8 yypact[] =
{
     -51,     9,     7,   -51,    18,    21,    12,    10,    33,    24,
      52,    42,    44,    64,    18,    18,    18,    47,    17,    45,
      37,    28,    18,   -51,   -51,    18,    40,    54,    18,    54,
      18,   -51,   -51,   -51,   -51,    65,    12,    49,   -51,     6,
       8,    48,    33,   -51,   -51,    -2,   -51,    59,   -51,    58,
      19,   -51,   -51,    56,   -51,     0,    57,   -51,     3,    11,
      60,    51,   -51,    53,   -51,   -51,   -51,   -51,    61,    62,
     -51,    63,    54,    54,    66,    67,    65,   -51,   -51,    12,
      12,    12,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,    18,    69,   -51,   -51,   -51,   -51,    74,    70,    77,
     -51,    55,   -51,    76,    78,    72,   -51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -51,   -51,   -51,   -50,   -14,   -51,   -51,   -51,   -32,   -51,
      46,   -51,   -51,   -51,   -51,   -51,   -51,   -22,   -34
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      32,    33,    34,    60,    57,    77,    55,    53,    44,     3,
       4,    46,    61,     5,    52,    61,    54,    63,    67,    18,
      63,    68,    69,     6,    76,     7,    93,    79,     8,     9,
      10,    62,    17,    64,    11,    80,    20,    72,    73,    12,
      21,    36,    74,    37,    39,    40,    23,    94,    95,    96,
      89,    90,    42,    43,    47,    48,    58,    59,    82,    83,
      84,    85,   103,   104,    27,    24,    29,   105,    31,    35,
      38,    55,    70,    65,    49,    71,    98,    97,   100,    75,
      78,   102,    87,    88,    81,    61,    86,    63,    66,     0,
       0,     0,     0,    91,    92,     0,   101,   106
};

static const yytype_int8 yycheck[] =
{
      14,    15,    16,    37,    36,    55,     6,    29,    22,     0,
       3,    25,     9,     6,    28,     9,    30,     9,    20,     7,
       9,    23,    24,     5,    24,     7,    76,    24,    10,    11,
      12,    25,    11,    25,    16,    24,    26,    18,    19,    21,
       7,    24,    23,    26,     7,     8,    22,    79,    80,    81,
      72,    73,    24,    25,    14,    15,     7,     8,     7,     8,
       7,     8,     7,     8,    22,    13,    22,   101,     4,    22,
      25,     6,    13,    25,    20,    17,     7,    91,     4,    23,
      23,     4,    20,    20,    24,     9,    25,     9,    42,    -1,
      -1,    -1,    -1,    27,    27,    -1,    26,    25
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    29,    30,     0,     3,     6,     5,     7,    10,    11,
      12,    16,    21,    32,    33,    35,    37,    11,     7,    36,
      26,     7,    38,    22,    13,    39,    40,    22,    41,    22,
      42,     4,    32,    32,    32,    22,    24,    26,    25,     7,
       8,    46,    24,    25,    32,    34,    32,    14,    15,    20,
      44,    45,    32,    45,    32,     6,    31,    36,     7,     8,
      46,     9,    25,     9,    25,    25,    38,    20,    23,    24,
      13,    17,    18,    19,    23,    23,    24,    31,    23,    24,
      24,    24,     7,     8,     7,     8,    25,    20,    20,    45,
      45,    27,    27,    31,    36,    36,    36,    32,     7,    43,
       4,    26,     4,     7,     8,    46,    25
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
#line 326 "final.y"
    { printf("program executed"); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 330 "final.y"
    { 
        processProto((yyvsp[(2) - (6)].name),(yyvsp[(3) - (6)].name));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 335 "final.y"
    { initProto((yyvsp[(1) - (2)].name)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 336 "final.y"
    { initProto((yyvsp[(1) - (3)].name)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 340 "final.y"
    { printf("\nempty block\n"); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 341 "final.y"
    {;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 342 "final.y"
    {;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 343 "final.y"
    {;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 344 "final.y"
    {;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 345 "final.y"
    {;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 346 "final.y"
    {;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 347 "final.y"
    {;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 350 "final.y"
    { processCall((yyvsp[(1) - (5)].name)) ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 353 "final.y"
    { addTypeToCall((yyvsp[(2) - (2)].name)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 354 "final.y"
    { addTypeToCall((yyvsp[(3) - (3)].name)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 359 "final.y"
    {
        declareVariable((yyvsp[(1) - (1)].name), lastDataType, 0);
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 362 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, 0);
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 365 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, (yyvsp[(3) - (3)].num));
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 368 "final.y"
    {
        double val = getValueOrDefault((yyvsp[(3) - (3)].name));
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, val);
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 372 "final.y"
    {
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, (yyvsp[(3) - (5)].num));
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 375 "final.y"
    {
        double val = getValueOrDefault((yyvsp[(3) - (5)].name));
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, val);
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 379 "final.y"
    {
        declareVariable((yyvsp[(1) - (3)].name), lastDataType, (yyvsp[(3) - (3)].num));
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 382 "final.y"
    {
        declareVariable((yyvsp[(1) - (5)].name), lastDataType, (yyvsp[(3) - (5)].num));
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 387 "final.y"
    {
        assignIfPossible((yyvsp[(1) - (4)].name),(yyvsp[(3) - (4)].num));
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 390 "final.y"
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

  case 31:

/* Line 1455 of yacc.c  */
#line 399 "final.y"
    {
        assignIfPossible((yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].num));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 404 "final.y"
    {
        discardVariable((yyvsp[(1) - (2)].name));
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 407 "final.y"
    {
        discardVariable((yyvsp[(1) - (3)].name));
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 411 "final.y"
    { printAndClearOutBuffer(); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 413 "final.y"
    { continueOutBuffer((yyvsp[(1) - (1)].name)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 414 "final.y"
    { continueOutBuffer((yyvsp[(3) - (3)].name)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 417 "final.y"
    { printf("\nif processed\n"); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 420 "final.y"
    {
        printf("\nLoop matched\n");
        startLoop();
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 426 "final.y"
    { 
        lastAssign.var = (yyvsp[(1) - (4)].name);
        //printf("=== %s %s %s ===",lastCond.left,lastCond.op,lastCond.right);
        //printf("=== %s %s %s %s ===", lastAssign.var,lastAssign.left,lastAssign.op,lastAssign.right);
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 433 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (1)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 434 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (3)].myBool) || (yyvsp[(3) - (3)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 435 "final.y"
    { (yyval.myBool) = (yyvsp[(1) - (3)].myBool) && (yyvsp[(3) - (3)].myBool); isLastIfValid=(yyval.myBool); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 438 "final.y"
    {
        (yyval.myBool) = evaluateCondition((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        //printf("------%s %s %s %d\n",$1, $2, $3, $$);
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 444 "final.y"
    {
        double val1 = getValueOrDefault((yyvsp[(1) - (3)].name));
        double val2 = getValueOrDefault((yyvsp[(3) - (3)].name));
        updateLast((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        (yyval.num) = doArithOperation(val1, val2, (yyvsp[(2) - (3)].name));
    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 450 "final.y"
    {
        double val1 = getValueOrDefault((yyvsp[(1) - (3)].name));
        
        updateLast((yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].name), dtoc((yyvsp[(3) - (3)].num)) );
        (yyval.num) = doArithOperation(val1, (yyvsp[(3) - (3)].num), (yyvsp[(2) - (3)].name));

    ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 457 "final.y"
    { 
        double val2 = getValueOrDefault((yyvsp[(3) - (3)].name));
        
        updateLast( dtoc((yyvsp[(1) - (3)].num)), (yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].name));
        (yyval.num) = doArithOperation((yyvsp[(1) - (3)].num), val2, (yyvsp[(2) - (3)].name));
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 463 "final.y"
    {
        updateLast( dtoc((yyvsp[(1) - (3)].num)), (yyvsp[(2) - (3)].name), dtoc((yyvsp[(3) - (3)].num)) );
        (yyval.num) = doArithOperation((yyvsp[(1) - (3)].num), (yyvsp[(3) - (3)].num), (yyvsp[(2) - (3)].name));
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2081 "final.tab.c"
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
#line 471 "final.y"



void yyerror(char *s)
{
	fprintf(stderr, "\n%s", s);
}

int main(){

	freopen("input.txt","r",stdin);
	//output = freopen("output.txt", "w", stdout); // output in file
	yyparse();
    printAll();
    printf("\nPrinting all prototype\n");
    printAllProto();
	return 0;
}

