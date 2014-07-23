/******************************************************************************
 *
 * フレーム
 *
 *
 * ＜概要＞
 *
 * 変数と値の対応を格納したオブジェクトをフレームと呼ぶ。
 * フレームの連なりを環境と呼ぶ。
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

Cute
H_frame( Cute parent )
{
    Cute p = alloc_obj( T_FRAME );

    N_frame_parent(p) = parent;
    N_frame_dict(p) = H_dict();

    return p;
}



Cute
H_frame_get( Cute self, Cute key, Cute *val )
{
    return H_dict_get( N_frame_dict(self), key, val );
}



Cute
H_frame_lookup( Cute self, Cute key, Cute *val )
{
    if ( self == H_None )
        return H_False;

    if ( H_dict_get( N_frame_dict(self), key, val ) == H_False )
        return H_frame_lookup( N_frame_parent(self), key, val );

    return H_True;
}



Cute
H_frame_put( Cute self, Cute key, Cute val )
{
    return H_dict_put( N_frame_dict(self), key, val );
}



Cute
H_frame_print( Cute self )
{
    if ( self == H_None )
        return H_None;

    Cute dict = N_frame_dict(self);

    H_dict_print( dict );

    Cute parent = N_frame_parent(self);

    if ( parent != H_None )
        printf( " => " );

    H_frame_print( parent );

    return H_None;
}




//=============================================================================
// 非公開関数

