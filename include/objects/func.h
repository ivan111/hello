#ifndef HEADER_FUNC
#define HEADER_FUNC


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------




    // 変数宣言 ---------------------------------------------------------------



    // 関数宣言 ---------------------------------------------------------------

Cute H_func( Cute type, Cute params, Cute body, Cute env );
Cute H_meth_call( Cute obj_id, Cute id, Cute args );
Cute H_call( Cute id, Cute args );

void H_c_func0( const char *name, Type_c_func0 func, bool var_args, Cute env );
void H_c_func1( const char *name, Type_c_func1 func, bool var_args, Cute env );
Cute H_call_c_func( Cute env, Cute func, Cute args );


#endif
