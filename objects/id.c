/******************************************************************************
 *
 * ID
 *
 *
 * ＜概要＞
 *
 * 文字列を数値で表したものをIDとする。しんぼる。
 *
 *****************************************************************************/


//=============================================================================
// ヘッダ


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------

#include "hello.h"


    // 定数 -------------------------------------------------------------------

#define MAX_ID_NAME_LEN  11

#define CH_UNDER_BAR 26


    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------



    // 変数宣言 ---------------------------------------------------------------

static char l_name[MAX_ID_NAME_LEN + 1];


    // 関数宣言 ---------------------------------------------------------------


//=============================================================================
// 公開関数


Cute
H_id( const char *name )
{
    size_t id = 0;
    size_t len = 0;

    for ( int i = 0; name[i]; i++ )
    {
        if ( i >= MAX_ID_NAME_LEN )
            error( "too long ID name" );

        int c = name[i];
        int shift = 55 - i*5;

        if ( 'a' <= c  &&  c <= 'z' )
            id |= (size_t)(c-'a') << shift;
        else if ( c == '_' )
            id |= (size_t)CH_UNDER_BAR << shift;
        else
            return 0;

        len++;
    }

    id |= (len << 60);
    id |= H_ID_FLAG;

    return (Cute) id;
}



char *
H_id_to_str( Cute self )
{
    if ( ! IS_H_ID (self) )
        return NULL;

    size_t id = (size_t) self;

    int len = (id >> 60) & 0xF;

    if ( len > MAX_ID_NAME_LEN )
        error( "id2str: too long ID name" );

    char *s = l_name;

    for ( int i = 0; i < len; i++ )
    {
        int shift = 55 - i*5;
        size_t v = id & ((size_t) 0x1F << shift);
        v >>= shift;

        if ( v == CH_UNDER_BAR )
            *s++ = '_';
        else
            *s++ = 'a' + v;
    }

    *s = '\0';

    return l_name;
}



void
H_id_print( Cute self )
{
    printf( "%s", H_id_to_str( self ) );
}



void
H_id_test( void )
{
    printf( "***** id *****\n" );

    Cute id_test  = H_id( "test" );
    Cute id_abc   = H_id( "abc" );
    Cute id_hello = H_id( "hello" );
    Cute id_test2 = H_id( "test" );
    Cute id_11    = H_id( "abcdefg_hij" );

    H_dbg_print( id_test );
    H_dbg_print( id_abc );
    H_dbg_print( id_hello );
    H_dbg_print( id_test2 );
    H_dbg_print( id_11 );

    printf( "\n" );
}



//=============================================================================
// 非公開関数



