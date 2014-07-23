#ifndef HEADER_CLASS
#define HEADER_CLASS


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------

typedef Cute (*Type_c_func0)( void );
typedef Cute (*Type_c_func1)( Cute arg1 );
typedef Cute (*Type_c_func2)( Cute arg1, Cute arg2 );



    // マクロ -----------------------------------------------------------------

#define N_class_id(p)     ( (p)->chisato )
#define N_class_body(p)   ( (p)->mai->chisato )
#define N_class_env(p)    ( (p)->mai->mai )



    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

void create_classes( void );
Cute create_class( const char *name );
Cute H_class( Cute name_id, Cute body );
Cute H_user_obj( Cute cls );
Cute H_get_class( Cute p );
Cute H_class_add_meth0( Cute self, const char *name, Type_c_func1 meth );
Cute H_class_add_meth1( Cute self, const char *name, Type_c_func2 meth );
Cute H_class_add_meth1v( Cute self, const char *name, Type_c_func2 meth );
Cute H_class_lookup( Cute self, Cute id, Cute *obj );


#endif
