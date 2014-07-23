/******************************************************************************
 *
 * ディクショナリ
 *
 *
 * ＜概要＞
 *
 * ディクショナリ（辞書・ハッシュテーブル・連想配列などともいう）の実装
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

static Cute H_dict_find( Cute self, Cute key );



//=============================================================================
// 公開関数


Cute
H_dict( void )
{
    Cute p = alloc_obj( T_DICT );

    N_dict_len(p) = H_0;
    N_dict_ent(p) = H_None;

    return p;
}



Cute
H_dict_get( Cute self, Cute key, Cute *val )
{
    Cute ent = H_dict_find( self, key );

    if ( ent == H_None )
        return H_False;

    *val = N_dent_val(ent);

    return H_True;
}



Cute
H_dict_put( Cute self, Cute key, Cute val )
{
    Cute ent = H_dict_find( self, key );

    if ( ent != H_None )
    {
        N_dent_val(ent) = val;
        return H_True;
    }

    ent = alloc_obj( T_NONE );

    N_dent_key(ent)  = key;
    N_dent_val(ent)  = val;
    N_dent_next(ent) = N_dict_ent(self);

    N_dict_ent(self) = ent;

    N_dict_len(self) = H_num_inc( N_dict_len(self) );

    return H_False;
}



Cute
H_dict_print( Cute self )
{
    Cute p;
  
    printf( "{\n" );

    for ( p = N_dict_ent(self); p != H_None; p = N_dent_next(p) )
    {
        printf( "    " );
        H_print_cute( N_dent_key(p) );
        printf( " => " );
        H_print_cute( N_dent_val(p) );
        printf( "\n" );
    }

    printf( "}\n\n" );

    return H_None;
}



void
H_dict_test( void )
{
    printf( "***** dict *****\n" );

    Cute p = H_dict();
    Cute id1 = H_id( "a" );
    Cute id2 = H_id( "b" );
    Cute id3 = H_id( "c" );

    H_dict_put( p, id1, H_num( 3 ) );
    H_dict_put( p, id2, H_num( 8 ) );
    H_dict_put( p, id3, H_num( 11 ) );

    Cute v;

    H_dict_get( p, id2, &v );
    H_dbg_print( v );

    H_dict_get( p, id1, &v );
    H_dbg_print( v );

    H_dict_get( p, id3, &v );
    H_dbg_print( v );

    printf( "dict len: " );
    H_dbg_print( N_dict_len(p) );

    printf( "\n\n" );
}




//=============================================================================
// 非公開関数

static Cute
H_dict_find( Cute self, Cute key )
{
    for ( Cute p = N_dict_ent(self); p != H_None; p = N_dent_next(p) )
    {
        if ( N_dent_key(p) == key )
            return p;
    }

    return H_None;
}
