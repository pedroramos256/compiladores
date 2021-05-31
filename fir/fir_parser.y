%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#include <iostream>
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  double                d;  /* double value */
  std::string           *s;	/* symbol name or string literal */
  cdk::basic_node       *node;	/* node pointer */
  cdk::sequence_node    *sequence;
  cdk::typed_node       *typednode;
  cdk::expression_node  *expression; /* expression nodes */
  cdk::lvalue_node      *lvalue;
  fir::block_node       *block;
  fir::prologue_node    *prologue;
};

%token tPUBLIC tPRIVATE tEXTERNAL 

%token <s> tIDENTIFIER 
%token <i> tINTEGER
%token <d> tREAL
%token <s> tSTRING

%token tTYPE_INT tTYPE_REAL tTYPE_STRING tTYPE_VOID
%token tNULL
%token tWHILE tDO tFINALLY
%token tIF tTHEN tELSE
%token tWRITE tWRITELN
%token tLEAVE tRESTART tRETURN
%token tDEFAULT_VAL tEPILOGUE
%token tSIZEOF tGE tLE tEQ tNE tNOT tAND tOR

%nonassoc tIFX
%nonassoc tELSE

%nonassoc tWHILEX
%nonassoc tFINALLY

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> declaration instruction
%type <sequence> file declarations argdecs opt_vardecs opt_instructions 
%type <sequence> exprs opt_exprs vardecs instructions
%type <typednode> vardec fundec argdec fundef
%type <expression> opt_initializer expr literal integer real
%type <lvalue> lvalue 
%type <block> block epilogue body
%type <prologue> prologue

%type<s> string
%type <type> data_type int_type string_type real_type void_type

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file          : declarations { compiler->ast($$ = $1); }
              | /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); }
              ;

declarations  :              declaration { $$ = new cdk::sequence_node(LINE, $1);     }
              | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
              ;

declaration   : vardec ';' { $$ = $1; }
              | fundec     { $$ = $1; }
              | fundef     { $$ = $1; }
              ;

/* ====================== Functions ================================== */

fundec        : data_type     tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tPRIVATE,  *$2, $4); delete $2; }
              | data_type '*' tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tPUBLIC,   *$3, $5); delete $3; }
              | data_type '?' tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tEXTERNAL, *$3, $5); delete $3; }
              | void_type     tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tPRIVATE,  *$2, $4); delete $2; }
              | void_type '*' tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tPUBLIC,   *$3, $5); delete $3; }
              | void_type '?' tIDENTIFIER '(' argdecs ')'     { $$ = new fir::function_declaration_node(LINE, $1,  tEXTERNAL, *$3, $5); delete $3; }
              ;

argdecs       : /* empty */         { $$ = nullptr;  }
              |             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
              | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
              ;

argdec        : data_type tIDENTIFIER { $$ = new fir::variable_declaration_node(LINE, $1, tPRIVATE, *$2, nullptr); delete $2; }
              ;

fundef        : data_type     tIDENTIFIER '(' argdecs ')' prologue body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, $7, $8); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' prologue body             { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, $7, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' prologue      epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, nullptr, $7); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' prologue                  { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, nullptr, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')'          body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, $6, $7); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')'          body             { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, $6, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')'               epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, nullptr, $6); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue body epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, $8, $9, $10); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue body                 { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, $8, $9, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue      epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, $8, nullptr, $9); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue                      { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, $8, nullptr, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal           body epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, nullptr, $8, $9); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal           body                 { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, nullptr, $8, nullptr); delete $2; }
              | data_type     tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal                epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $7, nullptr, nullptr, $8); delete $2; }

              | data_type '*' tIDENTIFIER '(' argdecs ')' prologue body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, $8, $9); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' prologue body             { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, $8, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' prologue      epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, nullptr, $7); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' prologue                  { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, nullptr, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')'          body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, $7, $8); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')'          body             { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, $7, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')'               epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, nullptr, $7); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue body epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, $9, $10, $11); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue body                 { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, $9, $10, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue      epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, $9, nullptr, $10); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal  prologue                      { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, $9, nullptr, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal           body epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, nullptr, $9, $10); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal           body                 { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, nullptr, $9, nullptr); delete $3; }
              | data_type '*' tIDENTIFIER '(' argdecs ')' tDEFAULT_VAL literal                epilogue        { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $8, nullptr, nullptr, $9); delete $3; }

              | void_type     tIDENTIFIER '(' argdecs ')' prologue body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, $7, $8); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')' prologue body             { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, $7, nullptr); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')' prologue      epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, nullptr, $7); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')' prologue                  { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, $6, nullptr, nullptr); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')'          body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, $6, $7); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')'          body             { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, $6, nullptr); delete $2; }
              | void_type     tIDENTIFIER '(' argdecs ')'               epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPRIVATE, *$2, $4, nullptr, nullptr, $6); delete $2; }
              | void_type '*' tIDENTIFIER '(' argdecs ')' prologue body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, $8, $9); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')' prologue body             { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, $8, nullptr); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')' prologue      epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, nullptr, $7); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')' prologue                  { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, $7, nullptr, nullptr); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')'          body epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, $7, $8); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')'          body             { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, $7, nullptr); delete $3; }
              | void_type '*' tIDENTIFIER '(' argdecs ')'               epilogue    { $$ = new fir::function_definition_node(LINE, $1,  tPUBLIC, *$3, $5, nullptr, nullptr, $7); delete $3; }
              ;

prologue      : '@' '{' opt_vardecs opt_instructions '}'        { $$ = new fir::prologue_node(LINE, $3, $4); }
              ;                               

epilogue      : tEPILOGUE block                                 { $$ = $2; }
              ;

body          : block                                           { $$ = $1; }
              ;

block         : '{' opt_vardecs opt_instructions '}'            { $$ = new fir::block_node(LINE, $2, $3); }
              ;
  

  /* ====================== Variables ================================== */

opt_vardecs  : /* empty */ { $$ = nullptr; }
             | vardecs     { $$ = $1; }
             ;


vardecs      : vardec ';'           { $$ = new cdk::sequence_node(LINE, $1);     }
             | vardecs vardec ';'   { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;
             

vardec        : data_type     tIDENTIFIER   opt_initializer     { $$ = new fir::variable_declaration_node(LINE, $1,  tPRIVATE, *$2, $3); delete $2; }
              | data_type '*' tIDENTIFIER   opt_initializer     { $$ = new fir::variable_declaration_node(LINE, $1,  tPUBLIC, *$3, $4); delete $3; }
              | data_type '?' tIDENTIFIER   opt_initializer     { $$ = new fir::variable_declaration_node(LINE, $1,  tEXTERNAL, *$3, $4); delete $3; }

opt_initializer  : /* empty */          { $$ = nullptr; /* must be nullptr, not NIL */ }
                 | '=' expr             { $$ = $2; }
                 ;

  /* ====================== Instructions ================================== */

opt_instructions: /* empty */  { $$ = nullptr; }
                | instructions { $$ = $1; }
                ;

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

instruction     : expr              ';'                             { $$ = new fir::evaluation_node(LINE, $1); }
                | tWRITE exprs      ';'                             { $$ = new fir::write_node(LINE, $2); }
                | tWRITELN exprs    ';'                             { $$ = new fir::write_node(LINE, $2, true); }
                | tLEAVE            ';'                             { $$ = new fir::leave_node(LINE); }
                | tLEAVE tINTEGER   ';'                             { $$ = new fir::leave_node(LINE, $2); }
                | tRESTART          ';'                             { $$ = new fir::restart_node(LINE); }
                | tRESTART tINTEGER ';'                             { $$ = new fir::restart_node(LINE, $2); }
                | tRETURN                                           { $$ = new fir::return_node(LINE); }
                | tIF expr tTHEN instruction %prec tIFX             { $$ = new fir::if_node(LINE, $2, $4); }
                | tIF expr tTHEN instruction tELSE instruction      { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
                | tWHILE expr tDO instruction %prec tWHILEX         { $$ = new fir::while_node(LINE, $2, $4); }
                | tWHILE expr tDO instruction tFINALLY instruction  { $$ = new fir::while_node(LINE, $2, $4, $6); }
                | block                                             { $$ = $1; }
                ;

  
  /* ============================== Types ================================== */

data_type     : string_type                       { $$ = $1; }
              | int_type                          { $$ = $1; }
              | real_type                         { $$ = $1; }
              | '<' data_type '>'                 { $$ = cdk::reference_type::create(4, $2); }
              ;

string_type   : tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);   }
              ;
int_type      : tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);      }
              ;
real_type     : tTYPE_REAL                       { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);   }
              ;
void_type     : tTYPE_VOID { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);   }
              ;

  /* ============================== Expressions ================================== */

exprs     : exprs ',' expr  { $$ = new cdk::sequence_node(LINE, $3, $1); }
          | expr            { $$ = new cdk::sequence_node(LINE, $1); } 
          ;

opt_exprs : exprs           { $$ = $1; }
          | /* empty */     { $$ = nullptr; }

expr      : literal                       { $$ = $1; }
          /* PRIMARY EXPRESSION */
          | '(' expr ')'                  { $$ = $2; }
          | '[' expr ']'                  { $$ = new fir::stack_alloc_node(LINE, $2); }
          /* UNARY EXPRESSION */
          | '-' expr %prec tUNARY         { $$ = new cdk::neg_node(LINE, $2); }
          | '+' expr %prec tUNARY         { $$ = new fir::identity_node(LINE, $2); }
          | lvalue '?'                    { $$ = new fir::address_of_node(LINE, $1); }
          /* ARITHMETIC EXPRESSIONS */
          | expr '+' expr                 { $$ = new cdk::add_node(LINE, $1, $3); }
          | expr '-' expr                 { $$ = new cdk::sub_node(LINE, $1, $3); }
          | expr '*' expr                 { $$ = new cdk::mul_node(LINE, $1, $3); }
          | expr '/' expr                 { $$ = new cdk::div_node(LINE, $1, $3); }
          | expr '%' expr                 { $$ = new cdk::mod_node(LINE, $1, $3); }
          /* LOGICAL EXPRESSIONS */
          | expr  '<' expr                { $$ = new cdk::lt_node(LINE, $1, $3); }
          | expr tLE  expr                { $$ = new cdk::le_node(LINE, $1, $3); }
          | expr tEQ  expr                { $$ = new cdk::eq_node(LINE, $1, $3); }
          | expr tGE  expr                { $$ = new cdk::ge_node(LINE, $1, $3); }
          | expr  '>' expr                { $$ = new cdk::gt_node(LINE, $1, $3); }
          | expr tNE  expr                { $$ = new cdk::ne_node(LINE, $1, $3); }
          /* LOGICAL EXPRESSIONS */
          | '~' expr                      { $$ = new cdk::not_node(LINE, $2); }
          | expr tAND  expr               { $$ = new cdk::and_node(LINE, $1, $3); }
          | expr tOR   expr               { $$ = new cdk::or_node (LINE, $1, $3); }
          /* LEFT VALUES */
          | lvalue                        { $$ = new cdk::rvalue_node(LINE, $1); }
          /* ASSIGNMENTS */
          | lvalue '=' expr               { $$ = new cdk::assignment_node(LINE, $1, $3); }
          /* OTHER EXPRESSION */
          | '@'                           { $$ = new fir::read_node(LINE); }
          /* OTHER EXPRESSION */
          | tIDENTIFIER '(' opt_exprs ')' { $$ = new fir::function_call_node(LINE, *$1, $3); delete $1; }
          | tSIZEOF '(' expr ')'          { $$ = new fir::sizeof_node(LINE, $3); }
          ;

lvalue    : tIDENTIFIER                                     { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
          | lvalue '[' expr ']'                             { $$ = new fir::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
          | '(' expr ')' '[' expr ']'                       { $$ = new fir::index_node(LINE, $2, $5); }
          | tIDENTIFIER '(' opt_exprs ')' '[' expr ']'      { $$ = new fir::index_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); delete $1; }
          ;
  /* ============================== Literals ================================== */

literal         : integer                       { $$ = $1; }
                | real                          { $$ = $1; }
                | string                        { $$ = new cdk::string_node(LINE, $1); }
                | tNULL                         { $$ = new fir::null_node(LINE); }
                ;

integer         : tINTEGER                      { $$ = new cdk::integer_node(LINE, $1); }
                ;

real            : tREAL                         { $$ = new cdk::double_node(LINE, $1); }
                ;

string          : tSTRING                       { $$ = $1; }
                | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
                ;

%%
