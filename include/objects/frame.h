#ifndef HEADER_H_FRAME
#define HEADER_H_FRAME


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------

#define N_frame_parent(p)  ( (p)->chisato )
#define N_frame_dict(p)    ( (p)->mai )


    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

Cute H_frame( Cute parent );
Cute H_frame_get( Cute self, Cute key, Cute *val );
Cute H_frame_lookup( Cute self, Cute key, Cute *val );
Cute H_frame_put( Cute self, Cute key, Cute val );
Cute H_frame_print( Cute self );


#endif
