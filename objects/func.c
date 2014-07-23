/******************************************************************************
 *
 * 関数
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



    // ローカル ヘッダ --------------------------------------------------------

#include "hello.h"


    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

static void check_arg_num( size_t argc, size_t expect );

static Cute H_call_c_func0( Type_c_func0 func );
static Cute H_call_c_func1( Type_c_func1 func, Cute arg1 );
static Cute H_call_c_func1v( Type_c_func1 func, Cute arg1 );
static Cute H_call_c_func2( Type_c_func2 func, Cute arg1, Cute arg2 );



//=============================================================================
// 公開関数

Cute
H_func( Cute env, Cute type, Cute params, Cute body )
{
    Cute p = alloc_objs( T_FUNC, 2 );

    N_func_env(p)    = env;
    N_func_type(p)   = type;
    N_func_params(p) = params;
    N_func_body(p)   = body;

    return p;
}



Cute
H_meth_call( Cute obj_id, Cute id, Cute args )
{
    return H_call( H_op( T_OP_REF, obj_id, id ), args );
}



Cute
H_call( Cute id, Cute args )
{
    Cute p = alloc_obj( T_CALL );

    N_call_id(p)   = id;
    N_call_args(p) = args;

    return p;
}



void
H_c_func0( const char *name, Type_c_func0 func, bool var_args, Cute env )
{
    Cute p = H_func( env, H_FUNC_C, H_0, (Cute) func );

    H_frame_put( env, H_id( name ), p );
}



void
H_c_func1( const char *name, Type_c_func1 func, bool var_args, Cute env )
{
    Cute params = var_args ? H_1_VAR : H_1;
    Cute p = H_func( env, H_FUNC_C, params, (Cute) func );

    H_frame_put( env, H_id( name ), p );
}



Cute
H_call_c_func( Cute func, Cute args, Cute env )
{
    Cute ret = H_None;

    size_t argc = H_num_val( N_list_len(args) );
    args = N_list_head(args);

    Cute paramc = N_func_params(func);

    if ( paramc == H_0 )
    {
        check_arg_num( argc, 0 );

        ret = H_call_c_func0( (Type_c_func0) N_func_body(func) );
    }
    else if ( paramc == H_1 )
    {
        check_arg_num( argc, 1 );

        Cute arg1 = eval( N_cell_item(args), env );
        ret = H_call_c_func1( (Type_c_func1) N_func_body(func), arg1 );
    }
    else if ( paramc == H_1_VAR )
    {
        Cute vargs = H_list();

        for ( Cute arg = args; arg != H_None; arg = N_cell_next(arg) )
        {
            Cute arg_val = eval( N_cell_item(arg), env );
            H_list_append( vargs, arg_val );
        }

        ret = H_call_c_func1( (Type_c_func1) N_func_body(func), vargs );
    }
    else if ( paramc == H_2 )
    {
        check_arg_num( argc, 2 );

        Cute arg = N_cell_item(args);
        Cute arg1 = eval( arg, env );
        args = N_cell_next(args);
        arg = N_cell_item(args);
        Cute arg2 = eval( arg, env );

        ret = H_call_c_func2( (Type_c_func2) N_func_body(func), arg1, arg2 );
    }
    else if ( paramc == H_2_VAR )
    {
        Cute arg = N_cell_item(args);
        Cute arg1 = eval( arg, env );
        args = N_cell_next(args);

        Cute vargs = H_list();

        for ( Cute arg = args; arg != H_None; arg = N_cell_next(arg) )
        {
            Cute arg_val = eval( N_cell_item(arg), env );
            H_list_append( vargs, arg_val );
        }

        ret = H_call_c_func2( (Type_c_func2) N_func_body(func), arg1, vargs );
    }
    else
    {
        printf( "c func\n" );
    }

    return ret;
}



//=============================================================================
// 非公開関数

static void
check_arg_num( size_t argc, size_t expect )
{
    if ( argc != expect )
        error( "argc(%zd) != %zd", argc, expect );
}



static Cute
H_call_c_func0( Type_c_func0 func )
{
    return func();
}



static Cute
H_call_c_func1( Type_c_func1 func, Cute arg1 )
{
    return func( arg1 );
}



static Cute
H_call_c_func1v( Type_c_func1 func, Cute arg1 )
{
    return func( arg1 );
}



static Cute
H_call_c_func2( Type_c_func2 func, Cute arg1, Cute arg2 )
{
    return func( arg1, arg2 );
}

