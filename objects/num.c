/******************************************************************************
 *
 * 数値
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



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------



//=============================================================================
// 公開関数

Cute
H_num_print( Cute self )
{
    //printf( "val = %zd\n", H_num_val( self ) );
    printf( "%zd", H_num_val( self ) );

    return H_None;
}



Cute
H_num_add( Cute self, Cute p )
{
    return H_num( H_num_val( self ) + H_num_val( p ) );
}



Cute
H_num_sub( Cute self, Cute p )
{
    return H_num( H_num_val( self ) - H_num_val( p ) );
}



Cute
H_num_mul( Cute self, Cute p )
{
    return H_num( H_num_val( self ) * H_num_val( p ) );
}



Cute
H_num_div( Cute self, Cute p )
{
    if ( p == H_0 )
        error( "div 0" );

    return H_num( H_num_val( self ) / H_num_val( p ) );
}



//=============================================================================
// 非公開関数



