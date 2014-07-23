/******************************************************************************
 *
 * 文字列
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



    // 関数宣言 ---------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------

/*
struct H_MethodDef g_String_methods[]  =  {
    { 0 , NULL , 0 }
};


struct H_Class *g_cString;
*/


//=============================================================================
// 公開関数

Cute
H_str( const char *s )
{
    Cute p = alloc_obj( T_STR );

    size_t len = strlen( s );
    N_str_len(p) = H_num( len );

    char *str = (char *) malloc( len + 1 );
    if ( str == NULL )
        error( "couldn't allocate memory" );

    strcpy( str, s );

    N_str_str(p) = (Cute) str;

    return p;
}



Cute
H_str2( const char *s, size_t len )
{
    Cute p = alloc_obj( T_STR );

    N_str_len(p) = H_num( len );

    char *str = (char *) malloc( len + 1 );
    if ( str == NULL )
        error( "couldn't allocate memory" );

    strncpy( str, s, len );

    N_str_str(p) = (Cute) str;

    return p;
}



Cute
H_str_len( Cute self )
{
    return N_str_len(self);
}



void
H_str_test( void )
{
    printf( "***** str *****\n" );

    Cute p = H_str( "test" );
    Cute p2 = H_str ("hello");

    H_dbg_print( p );
    H_dbg_print( p2 );

    printf( "\n" );
}



//=============================================================================
// 非公開関数


