/******************************************************************************
 *
 * クラス
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




//=============================================================================
// 公開関数

void
create_classes( void )
{
    create_list_class();
}



Cute
create_class( const char *name )
{
    Cute id = H_id( name );
    Cute p = H_class( id, H_None );

    H_frame_put( g_global, id, p );

    return p;
}



Cute
H_class( Cute name_id, Cute body )
{
    Cute p = alloc_objs( T_CLASS, 2 );

    N_class_id(p)   = name_id;
    N_class_body(p) = body;
    N_class_env(p)  = H_frame( H_None );

    return p;
}



Cute
H_user_obj( Cute cls )
{
    Cute p;
    Cute id = N_class_id(cls);

    Cute ID_LIST = H_id( "list" );

    if ( id == ID_LIST )
    {
        p = H_list();
    }
    else
    {
        p = alloc_obj( T_USER_OBJ );

        N_uobj_class(p) = cls;
        N_uobj_iv(p)    = H_frame( N_class_env(cls) );
    }

    return p;
}



Cute
H_get_class( Cute p )
{
    Type type = H_get_type( p );

    if ( type == T_LIST )
        return g_cls_list;
    else if ( type == T_USER_OBJ )
        return N_uobj_class(p);

    return H_None;
}



Cute
H_class_add_meth0( Cute self, const char *name, Type_c_func1 func )
{
    Cute p = H_func( H_None, H_FUNC_C, H_1, (Cute) func );

    H_frame_put( N_class_env(self), H_id(name), p );

    return p;
}



Cute
H_class_add_meth1( Cute self, const char *name, Type_c_func2 func )
{
    Cute p = H_func( H_None, H_FUNC_C, H_2, (Cute) func );

    H_frame_put( N_class_env (self), H_id(name), p );

    return p;
}



Cute
H_class_add_meth1v( Cute self, const char *name, Type_c_func2 func )
{
    Cute p = H_func( H_None, H_FUNC_C, H_2_VAR, (Cute) func );

    H_frame_put( N_class_env (self), H_id(name), p );

    return p;
}



Cute
H_class_lookup( Cute self, Cute id, Cute *obj )
{
    return H_frame_lookup( N_class_env(self), id, obj );
}



//=============================================================================
// 非公開関数


