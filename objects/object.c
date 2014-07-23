/******************************************************************************
 *
 * オブジェクト
 *
 *
 * ＜概要＞
 *
 *
 *****************************************************************************/


//=============================================================================
// ヘッダ


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------

#include "hello.h"


    // 定数 -------------------------------------------------------------------

#define MAX_FREE_OBJS  4096


    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------

static Cute l_head;

static char *l_type_name_tbl[T_MAX_BUILTIN];



    // 関数宣言 ---------------------------------------------------------------




//=============================================================================
// 公開関数

void
init_free_objs( void )
{
    l_head = (Cute) malloc( sizeof(struct TAG_Object) * MAX_FREE_OBJS );

    if ( l_head == NULL )
        error( "couldn't initialize free objects memory" );

    int i;

    for ( i = 0; i < MAX_FREE_OBJS - 1; i++ )
    {
        N_cell_next(&l_head[i]) = &l_head[i+1];
    }

    N_cell_next(&l_head[MAX_FREE_OBJS - 1]) = NULL;
}



Cute
alloc_obj( Type type )
{
    if ( l_head == NULL )
        error( "couldn't allocate object" );

    Cute p = l_head;
    N_type(p) = type;

    l_head = N_cell_next(l_head);

    return p;
}



Cute
alloc_objs( Type type, int num )
{
    if ( l_head == NULL )
        error( "couldn't allocate object" );

    Cute p = l_head;
    N_type(p) = type;

    for ( int i = 0; i < num; i++ )
    {
        if ( l_head == NULL )
            error( "couldn't allocate object" );

        l_head = N_cell_next(l_head);
    }

    return p;
}



void
free_obj( Cute p )
{
    N_cell_next(p) = l_head;
    l_head = p;
}



Cute
H_print_cute( Cute p )
{
    switch ( H_get_type( p ) )
    {
    case T_NUM:
        H_num_print( p );
        break;


    case T_ID:
        H_id_print( p );
        break;


    case T_STR:
        printf( "%s", (char *) N_str_str(p) );
        break;


    case T_LIST:
        H_list_print( p );
        break;


    case T_FRAME:
        H_frame_print( p );
        break;


    case T_DICT:
        H_dict_print( p );
        break;

    default:
        printf( "[%s]", H_type_to_str( N_type(p) ) );
        break;
    }

    return H_None;
}



Cute
H_print( Cute vargs )
{
    bool first = true;

    Type type = H_get_type( vargs );
    if ( type != T_LIST )
        error( "H_print: type error [%s]", H_type_to_str( type ) );

    Cute head = N_list_head(vargs);

    for ( Cute arg = head; arg != H_None; arg = N_cell_next(arg) )
    {
        if ( first )
            first = false;
        else
            printf( " " );

        H_print_cute( N_cell_item(arg) );
    }

    printf( "\n" );

    return H_None;
}



Cute
H_dbg_print( Cute p )
{
    printf( "[dbg] addr = %zx, type [%s], val = ", (size_t) p, H_get_type_str( p ) );
    H_print_cute( p );
    printf( "\n" );

    return H_None;
}



void
init_type_name_table( void )
{
    char **tbl = l_type_name_tbl;

    tbl[ T_NONE ]     = "None";
    tbl[ T_NUM ]      = "num";
    tbl[ T_ID ]       = "ID";
    tbl[ T_CLASS ]    = "class";
    tbl[ T_STR ]      = "str";
    tbl[ T_LIST ]     = "list";
    tbl[ T_CELL ]     = "cell";
    tbl[ T_DICT ]     = "dict";
    tbl[ T_FRAME ]    = "frame";
    tbl[ T_USER_OBJ ] = "user obj";
    tbl[ T_ASSIGN ]   = "assign";
    tbl[ T_WHILE ]    = "while";
    tbl[ T_IF ]       = "if";
    tbl[ T_RETURN ]   = "return";
    tbl[ T_FUNC ]     = "func";
    tbl[ T_CALL ]     = "call";
    tbl[ T_OP_REF ]   = ".";
    tbl[ T_OP_ADD ]   = "+";
    tbl[ T_OP_SUB ]   = "-";
    tbl[ T_OP_MUL ]   = "*";
    tbl[ T_OP_DIV ]   = "/";
    tbl[ T_OP_EQ ]    = "==";
    tbl[ T_OP_NE ]    = "!=";
    tbl[ T_OP_LE ]    = "<=";
    tbl[ T_OP_GE ]    = ">=";
    tbl[ T_OP_LT ]    = "<";
    tbl[ T_OP_GT ]    = ">";
}



char *
H_get_type_str( Cute p )
{
    return H_type_to_str( H_get_type( p ) );
}



char *
H_type_to_str( Type t )
{
    if ( t < T_MAX_BUILTIN )
        return l_type_name_tbl[ t ];

    error( "H_type_to_name: unknown type %d", t );

    return "";
}


//=============================================================================
// 非公開関数


