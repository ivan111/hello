/******************************************************************************
 *
 * Hello, Hello Language!
 *
 *
 * ＜概要＞
 *
 *
 *
 *****************************************************************************/


//=============================================================================
// ヘッダ


    // システム ヘッダ --------------------------------------------------------

#include <setjmp.h>


    // ローカル ヘッダ --------------------------------------------------------

#include "hello.h"


    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------

typedef Cute (*EVAL_FUNC)( Cute p, Cute env );


    // マクロ -----------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------

static EVAL_FUNC eval_tbl[T_MAX_BUILTIN];

static bool l_return;
static Cute l_ret_val;



    // 関数宣言 ---------------------------------------------------------------

static void create_global_env( void );
static void init_eval_table( void );

static Cute undef_eval( Cute p, Cute env );
static Cute identifier_eval(Cute p, Cute env);

static Cute eval_id    ( Cute p, Cute env );
static Cute eval_class ( Cute p, Cute env );
static Cute eval_assign( Cute p, Cute env );
static Cute eval_cell  ( Cute p, Cute env );
static Cute eval_while ( Cute p, Cute env );
static Cute eval_for   ( Cute p, Cute env );
static Cute eval_if    ( Cute p, Cute env );
static Cute eval_return( Cute p, Cute env );
static Cute eval_func  ( Cute p, Cute env );
static Cute eval_call  ( Cute p, Cute env );
static Cute eval_op_ref( Cute p, Cute env );
static Cute eval_op_add( Cute p, Cute env );
static Cute eval_op_sub( Cute p, Cute env );
static Cute eval_op_mul( Cute p, Cute env );
static Cute eval_op_div( Cute p, Cute env );
static Cute eval_op_eq ( Cute p, Cute env );
static Cute eval_op_ne ( Cute p, Cute env );
static Cute eval_op_le ( Cute p, Cute env );
static Cute eval_op_ge ( Cute p, Cute env );
static Cute eval_op_lt ( Cute p, Cute env );
static Cute eval_op_gt ( Cute p, Cute env );


static Cute H_call_func( Cute func, Cute args, Cute env );
static Cute H_call_user_func( Cute func, Cute args, Cute env );
static Cute get_member( Cute obj, Cute member_id, bool must_exists );



//=============================================================================
// 公開関数


int
main( void )
{
    init_type_name_table();
    init_free_objs();
    init_eval_table();

    /*
    printf( "h-lang start\n\n" );

    H_id_test();
    H_str_test();
    H_list_test();
    H_dict_test();

    printf( "h-lang end\n\n" );
    */

    create_global_env();
    g_env = g_global;

    yyparse();

    return 0;
}



__attribute__ ((noreturn)) 
__attribute__((format (printf, 1, 2)))
void
error( const char *fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );
    fprintf( stderr, "ERROR: " );
    vfprintf( stderr, fmt, ap );
    fprintf( stderr, "\n" );
    va_end( ap );

    exit( 1 );
}



Cute
eval( Cute p, Cute env )
{
    Type type = H_get_type( p );

    return eval_tbl[type]( p, env );
}



//=============================================================================
// 非公開関数

static void
init_eval_table( void )
{
    memset( eval_tbl, 0, sizeof(eval_tbl) );

    eval_tbl[T_NONE]   = identifier_eval;
    eval_tbl[T_NUM]    = identifier_eval;
    eval_tbl[T_ID]     = eval_id;
    eval_tbl[T_CLASS]  = eval_class;
    eval_tbl[T_STR]    = identifier_eval;
    eval_tbl[T_LIST]   = identifier_eval;
    eval_tbl[T_CELL]   = eval_cell;
    eval_tbl[T_DICT]   = identifier_eval;
    eval_tbl[T_FRAME]  = undef_eval;
    eval_tbl[T_USER_OBJ] = identifier_eval;

    eval_tbl[T_ASSIGN] = eval_assign;
    eval_tbl[T_WHILE]  = eval_while;
    eval_tbl[T_FOR]    = eval_for;
    eval_tbl[T_IF]     = eval_if;
    eval_tbl[T_RETURN] = eval_return;
    eval_tbl[T_FUNC]   = eval_func;
    eval_tbl[T_CALL]   = eval_call;
    eval_tbl[T_OP_REF] = eval_op_ref;
    eval_tbl[T_OP_ADD] = eval_op_add;
    eval_tbl[T_OP_SUB] = eval_op_sub;
    eval_tbl[T_OP_MUL] = eval_op_mul;
    eval_tbl[T_OP_DIV] = eval_op_div;
    eval_tbl[T_OP_EQ]  = eval_op_eq;
    eval_tbl[T_OP_NE]  = eval_op_ne;
    eval_tbl[T_OP_LE]  = eval_op_le;
    eval_tbl[T_OP_GE]  = eval_op_ge;
    eval_tbl[T_OP_LT]  = eval_op_lt;
    eval_tbl[T_OP_GT]  = eval_op_gt;
}


static void
create_global_env( void )
{
    g_global = H_frame( H_None );
    Cute env = g_global;

    H_c_func1( "print", H_print, true, env );

    create_classes();
}



static Cute
undef_eval( Cute p, Cute env )
{
    error( "undefined eval func" );
}



static Cute
identifier_eval( Cute p, Cute env )
{
    return p;
}



static Cute
eval_id( Cute p, Cute env )
{
    Cute val = H_None;

    if ( H_frame_lookup( env, p, &val ) )
        return val;
    else
        error( "not found id [%s]", H_id_to_str(p) );

    return H_None;
}



static Cute
eval_assign( Cute p, Cute env )
{
    Cute val = eval( N_assign_val(p), env );
    Cute id = N_assign_id(p);

    Type type = H_get_type( id );

    if ( type == T_ID )
    {
        H_frame_put( env, id, val );
    }
    else if ( type == T_OP_REF )
    {
        Cute obj_id = N_op_left(id);
        Cute obj;
        if ( H_frame_lookup( env, obj_id, &obj ) == H_False )
            error( "not found object [%s]", H_id_to_str( obj_id ) );

        Type obj_type = H_get_type( obj );
        if ( obj_type != T_USER_OBJ )
            error( "[%s] is not user object", H_id_to_str( obj_id ) );

        Cute member_id = N_op_right(id);

        H_frame_put (N_uobj_iv(obj), member_id, val);
    }
    else
    {
        error( "eval_assign: invalid type [%s]", H_type_to_str( type ) );
    }

    return val;
}



static Cute
eval_cell( Cute node, Cute env )
{
    Cute ret;

    for ( Cute p = node; p != H_None; p = N_cell_next(p) )
    {
        ret = eval( N_cell_item(p), env );

        if ( l_return )
            return ret;
    }

    return ret;
}



static Cute
eval_while( Cute p, Cute env )
{
    while ( to_c_cond( eval( N_while_cond(p), env ) ) )
    {
        eval( N_while_block(p), env );

        if ( l_return )
            return l_ret_val;
    }

    return H_None;
}



static Cute
eval_for( Cute p, Cute env )
{
    Cute id = N_for_id(p);

    Cute iter = eval( N_for_iter(p), env );
    Cute next = get_member( iter, H_id( "__next__" ), true );

    Cute args = H_list();
    H_list_append( args, iter );

    Cute val = H_call_func( next, args, env );
    eval( H_assign( id, val ), env );

    while ( to_c_cond( val ) )
    {
        eval( N_for_block(p), env );

        if ( l_return )
            return l_ret_val;

        val = H_call_func( next, args, env );
        eval( H_assign( id, val ), env );
    }

    return H_None;
}



static Cute
eval_if( Cute p, Cute env )
{
    if ( eval( N_if_cond(p), env ) )
        eval( N_if_if(p), env );
    else
        eval( N_if_else(p), env );

    if ( l_return )
        return l_ret_val;

    return H_None;
}



static Cute
eval_class( Cute p, Cute env )
{
    Cute class_env = N_class_env(p);

    // メソッドが使う環境をつくる
    Cute new_env = alloc_obj( T_FRAME );
    N_frame_parent(new_env) = env;
    N_frame_dict(new_env) = N_frame_dict(class_env);  // dictを共有する

    eval( N_class_body(p), new_env );

    return p;
}



static Cute
eval_func( Cute p, Cute env )
{
    N_func_env(p) = env;

    return p;
}



static Cute
eval_return( Cute p, Cute env )
{
    l_ret_val = eval( N_return_exp(p), env );
    l_return = true;

    return l_ret_val;
}



static Cute
eval_call( Cute p, Cute env )
{
    Cute id = N_call_id(p);
    Cute args = N_call_args(p);

    Type id_type = H_get_type( id );

    Cute func;
    //printf( "env => " ); H_frame_print( env );

    if ( id_type == T_ID )
    {
        if ( H_frame_lookup( env, id, &func ) == H_False )
        {
            H_frame_print( env );
            error( "'%s' func is not found", H_id_to_str(id) );
        }

        //printf( "call %s\n", H_id_to_str(id) );
    }
    else if ( id_type == T_OP_REF )  // メソッド呼び出し
    {
        func = eval_op_ref( id, env );

        Cute obj_id = N_op_left(id);
        Cute obj;
        if ( H_frame_lookup( env, obj_id, &obj ) == H_False )
            error( "not found object [%s]", H_id_to_str( obj_id ) );

        Cute new_args = H_list();
        H_list_append( new_args, obj );
        H_list_cat( new_args, args );

        args = new_args;
    }
    else
        error( "couldn't call type [%s]", H_type_to_str( id_type ) );

    Type type = H_get_type( func );

    if ( type == T_CLASS )
    {
        Cute ret = H_user_obj( func );

        Cute member = get_member( ret, H_id( "__init__" ), false );
        if ( member != H_None   &&   H_get_type( member ) == T_FUNC )
        {
            Cute new_args = H_list();
            H_list_append( new_args, ret );
            H_list_cat( new_args, args );

            H_call_func( member, new_args, env );
        }

        return ret;
    }
    else if ( type == T_FUNC )
        return H_call_func( func, args, env );

    error( "couldn't call type [%s]", H_type_to_str( type ) );

    return H_None;
}



static Cute
eval_op_ref( Cute p, Cute env )
{
    Cute obj_id = N_op_left(p);
    Cute obj;
    if ( H_frame_lookup( env, obj_id, &obj ) == H_False )
        error( "not found object [%s]", H_id_to_str( obj_id ) );

    Cute member_id = N_op_right(p);

    return get_member( obj, member_id, true );
}



static Cute
eval_op_add( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
        return H_num_add( o1, o2 );
    else
        error( "must num" );

    return H_None;
}



static Cute
eval_op_sub( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
        return H_num_sub( o1, o2 );
    else
        error( "must num" );

    return H_None;
}



static Cute
eval_op_mul( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
        return H_num_mul( o1, o2 );
    else
        error( "must num" );

    return H_None;
}



static Cute
eval_op_div( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
    {
        return H_num_div( o1, o2 );
    }
    else
        error( "must num" );

    return H_None;
}



static Cute
eval_op_eq( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if (o1 == o2)
        return H_True;
    else
        return H_False;
}



static Cute
eval_op_ne( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if (o1 != o2)
        return H_True;
    else
        return H_False;
}



static Cute
eval_op_le( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
    {
        if ( o1 <= o2 )
            return H_True;
        else
            return H_False;
    }
    else
        error( "must num" );
}



static Cute
eval_op_ge( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
    {
        if ( o1 >= o2 )
            return H_True;
        else
            return H_False;
    }
    else
        error( "must num" );
}



static Cute
eval_op_lt( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
    {
        if ( o1 < o2 )
            return H_True;
        else
            return H_False;
    }
    else
        error( "must num" );
}



static Cute
eval_op_gt( Cute p, Cute env )
{
    Cute o1 = eval( N_op_left(p) , env );
    Cute o2 = eval( N_op_right(p), env );

    if ( IS_H_NUM(o1) && IS_H_NUM(o2) )
    {
        if ( o1 > o2 )
            return H_True;
        else
            return H_False;
    }
    else
        error( "must num" );
}



static Cute
H_call_func( Cute func, Cute args, Cute env )
{
    Cute func_type = N_func_type(func);

    if ( func_type == H_FUNC_C )
        return H_call_c_func( func, args, env );

    if ( func_type == H_FUNC_USER )
        return H_call_user_func( func, args, env );

    error( "unknown func type %zx", (size_t) func_type );
}



static Cute
H_call_user_func( Cute func, Cute args, Cute env )
{
    size_t argc = H_num_val( N_list_len(args) );
    args = N_list_head(args);

    Cute params = N_func_params(func);
    size_t paramc = H_num_val( N_list_len(params) );
    params = N_list_head(params);

    if ( argc != paramc )
        error( "params num(%zd) != args num(%zd)", paramc, argc );

    Cute body = N_func_body(func);
    Cute param;
    Cute arg;

    Cute f_env = N_func_env(func);
    f_env = H_frame( f_env );

    for ( param = params, arg = args;
            param != H_None && arg != H_None;
            param = N_cell_next(param), arg = N_cell_next(arg) )
    {
        Cute key = N_cell_item(param);
        Cute val = eval( N_cell_item(arg), env );

        H_frame_put( f_env, key, val );
    }

    Cute ret = eval( body, f_env );

    if ( l_return )
    {
        l_return = false;
        return l_ret_val;
    }

    return ret;
}



static Cute
get_member( Cute obj, Cute member_id, bool must_exists )
{
    Cute member;

    Type obj_type = H_get_type( obj );

    if ( obj_type == T_USER_OBJ )
    {
        if ( H_frame_lookup( N_uobj_iv(obj), member_id, &member ) == H_False )
        {
            if ( must_exists )
                error( "not found member [%s]", H_id_to_str( member_id ) );
            else
                return H_None;
        }
    }
    else
    {
        Cute cls = H_get_class( obj );

        Type cls_type = H_get_type( cls );
        if ( cls_type != T_CLASS )
            error( "type [%s] is not class", H_type_to_str( cls_type ) );

        if ( H_class_lookup( cls, member_id, &member) == H_False )
        {
            if ( must_exists )
                error( "not found member [%s]", H_id_to_str( member_id ) );
            else
                return H_None;
        }
    }

    return member;
}

