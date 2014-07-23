#ifndef HEADER_H_LIST
#define HEADER_H_LIST


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------

#define N_list_len(p)  ( (p)->chisato )
#define N_list_head_tail(p)  ( (p)->mai )
#define N_list_head(p)  ( (p)->mai->chisato )
#define N_list_tail(p)  ( (p)->mai->mai )


#define N_cell_item(p)   ( (p)->chisato )
#define N_cell_next(p)   ( (p)->mai )


    // 変数宣言 ---------------------------------------------------------------

extern Cute g_cls_list;


    // 関数宣言 ---------------------------------------------------------------

void create_list_class( void );

Cute H_cell( Cute item, Cute next );

Cute H_list( void );
Cute H_list_init( Cute self, Cute args );
Cute H_list_len( Cute self );
Cute H_list_append( Cute self, Cute item );
Cute H_list_cat( Cute self, Cute list );
Cute H_list_getitem( Cute self, Cute index );
Cute H_list_print( Cute self );

void H_list_test( void );



#endif
