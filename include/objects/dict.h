#ifndef HEADER_H_DICT
#define HEADER_H_DICT


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------

#define N_dict_len(p)   ( (p)->chisato )
#define N_dict_ent(p)   ( (p)->mai )
#define N_dent_key(p)   ( (p)->airi )
#define N_dent_val(p)   ( (p)->chisato )
#define N_dent_next(p)  ( (p)->mai )


    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

Cute H_dict( void );
Cute H_dict_get( Cute self, Cute key, Cute *val );
Cute H_dict_put( Cute self, Cute key, Cute val );
Cute H_dict_print( Cute self );

void H_dict_test( void );


#endif
