%{

#include "hello.h"


int yylex( void );
int yyerror( char *s );

Cute g_env;
Cute g_global;


%}



%token NUMBER
%token ID
%token STRING
%token CLASS
%token DEF
%token WHILE
%token FOR
%token IN
%token IF
%token RETURN
%token EOL
%nonassoc IFX
%nonassoc ELSE
%nonassoc CALL

%left OP_EQ  OP_NE  OP_LE  OP_GE  '<'  '>'
%left '+'  '-'
%left '*'  '/'
%nonassoc UMINUS
%left '['  ']'
%nonassoc '.'



%%


program:
    function  {
        exit( 0 );
    }
;



function:
    function  stmt  {
        eval( $2, g_env );
    }

    |

    /* NULL */
;



stmt:
    EOL  {
        $$ = H_None;
    }

    |

    expr  EOL  {
        $$ = $1;
    }

    |

    ID  '='  expr  EOL  {
        $$ = H_assign( $1, $3 );
    }

    |

    ID  '.'  ID  '='  expr  EOL  {
        $$ = H_assign( H_op( T_OP_REF, $1, $3 ), $5 );
    }

    |

    CLASS  ID  ':'  EOL  block  {
        $$ = H_assign( $2, H_class( $2, $5 ) );
    }

    |

    DEF  ID  params_list  ':'  EOL  block  {
        $$ = H_assign( $2, H_func( H_None, H_FUNC_USER, $3, $6 ) );
    }

    |

    WHILE  expr  ':'  EOL  block  {
        $$ = H_while( $2, $5 );
    }

    |

    FOR  ID  IN  expr  ':'  EOL  block  {
        $$ = H_for( $2, $4, $7 );
    }

    |

    IF  expr  ':'  EOL  block  %prec IFX  {
        $$ = H_if( $2, $5, H_None );
    }

    |

    IF  expr  ':'  EOL  block  ELSE  ':'  EOL  block  {
        $$ = H_if( $2, $5, $9 );
    }

    |

    RETURN  expr  EOL  {
        $$ = H_return( $2 );
    }
;



params_list:
    '('  ')'  {
        $$ = H_list();
    }

    |

    '('  params  ')'  {
        $$ = $2;
    }
;




params:
    ID  {
        Cute list = H_list();
        H_list_append( list, $1 );
        $$ = list;
    }

    |

    params  ','  ID {
        $$ = H_list_append( $1, $3 );
    }
;



args_list:
    '('  ')'  {
        $$ = H_list();
    }

    |

    '('  args  ')'  {
        $$ = $2;
    }
;




args:
    expr  {
        Cute list = H_list();
        H_list_append( list, $1 );
        $$ = list;
    }

    |

    args  ','  expr {
        $$ = H_list_append( $1, $3 );
    }
;



block:
    '{'  stmt_list  '}'  {
        $$ = N_list_head( $2 );
    }
;



stmt_list:
    stmt  {
        Cute list = H_list();
        H_list_append( list, $1 );
        $$ = list;
    }

    |

    stmt_list  stmt  {
        if ( $2 == H_None )
            $$ = $1;
        else
            $$ = H_list_append( $1, $2 );
    }
;



expr:
      NUMBER  { $$ = $1; }
    | ID      { $$ = $1; }
    | STRING  { $$ = $1; }

    |

    expr  '+'  expr  {
        if ( IS_H_NUM( $1 ) && IS_H_NUM( $3 ) )
            $$ = H_num_add( $1, $3 );
        else
            $$ = H_op( T_OP_ADD, $1, $3 );
    }

    |

    expr  '-'  expr  {
        if ( IS_H_NUM( $1 ) && IS_H_NUM( $3 ) )
            $$ = H_num_sub( $1, $3 );
        else
            $$ = H_op( T_OP_SUB, $1, $3 );
    }

    |

    expr  '*'  expr  {
        if ( IS_H_NUM( $1 ) && IS_H_NUM( $3 ) )
            $$ = H_num_mul( $1, $3 );
        else
            $$ = H_op( T_OP_MUL, $1, $3 );
    }

    |

    expr  '/'  expr  {
        if ( IS_H_NUM( $1 ) && IS_H_NUM( $3 ) )
            $$ = H_num_div( $1, $3 );
        else
            $$ = H_op( T_OP_DIV, $1, $3 );
    }

    | expr  '<'  expr    { $$ = H_op( T_OP_LT , $1, $3 ); }
    | expr  '>'  expr    { $$ = H_op( T_OP_GT , $1, $3 ); }
    | expr  OP_EQ  expr  { $$ = H_op( T_OP_EQ , $1, $3 ); }
    | expr  OP_NE  expr  { $$ = H_op( T_OP_NE , $1, $3 ); }
    | expr  OP_LE  expr  { $$ = H_op( T_OP_LE , $1, $3 ); }
    | expr  OP_GE  expr  { $$ = H_op( T_OP_GE , $1, $3 ); }
    | '('  expr  ')'  { $$ = $2; } 
    | '-'  expr  %prec UMINUS  { $$ = H_op( T_OP_SUB, H_0, $2 ); }

    |

    ID  '.'  ID  args_list  {
        $$ = H_call( H_op( T_OP_REF, $1, $3 ), $4 );
    }

    |

    ID  '.'  ID  {
        $$ = H_op( T_OP_REF, $1, $3 );
    }

    |

    ID  args_list  {
        $$ = H_call( $1, $2 );
    }

    |

    ID  '['  NUMBER  ']'  {
        Cute args = H_list();
        H_list_append( args, $3 );
        $$ = H_call( H_op( T_OP_REF, $1, H_id( "__getitem__" ) ), args );
    }

    |

    '['  args  ']'  {
        $$ = $2;
    }
;



%%

Cute
H_assign( Cute id, Cute val )
{
    Cute p = alloc_obj( T_ASSIGN );

    N_assign_id(p)  = id;
    N_assign_val(p) = val;

    return p;
}



Cute
H_op( enum ObjectType op_type, Cute left, Cute right )
{
    Cute p = alloc_obj( op_type );

    N_op_left(p)  = left;
    N_op_right(p) = right;

    return p;
}



Cute
H_while( Cute cond, Cute block )
{
    Cute p = alloc_obj( T_WHILE );

    N_while_cond(p)  = cond;
    N_while_block(p) = block;

    return p;
}



Cute
H_for( Cute id, Cute iter, Cute block )
{
    Cute p = alloc_objs( T_FOR, 2 );

    N_for_id(p)    = id;
    N_for_iter(p)  = iter;
    N_for_block(p) = block;

    return p;
}



Cute
H_if( Cute cond, Cute if_block, Cute else_block )
{
    Cute p = alloc_objs( T_IF, 2 );

    N_if_cond(p) = cond;
    N_if_if(p)   = if_block;
    N_if_else(p) = else_block;

    return p;
}



Cute
H_return( Cute exp )
{
    Cute p = alloc_obj( T_RETURN );

    N_return_exp(p) = exp;

    return p;
}



extern int g_line_no;

int
yyerror( char *s )
{
    fprintf( stderr, "parser: %s  line:%d\n", s, g_line_no );

    exit( 1 );
}
