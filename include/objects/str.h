#ifndef HEADER_H_STR
#define HEADER_H_STR


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------

#define N_str_len(p)  ( (p)->chisato )
#define N_str_str(p)  ( (p)->mai )


    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

Cute H_str( const char *s );
Cute H_str2( const char *s, size_t len );
Cute H_str_len( Cute self );

void H_str_test( void );


#endif
