#ifndef HEADER_HELLO
#define HEADER_HELLO


#if __SIZEOF_SIZE_T__  !=  8
#error "H-Lang can run only on a 64 bit CPU."
#endif


    // システム ヘッダ --------------------------------------------------------

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


    // 定数 -------------------------------------------------------------------



    // 構造体 -----------------------------------------------------------------



    // マクロ -----------------------------------------------------------------

#define STRCMP(a, R, b)  (strcmp(a,b) R 0)

#define STRNCMP(a, R, b, n)  (strncmp(a,b,n) R 0)


    // ローカル ヘッダ --------------------------------------------------------

#include "objects/object.h"
#include "objects/class.h"
#include "objects/num.h"
#include "objects/str.h"
#include "objects/id.h"
#include "objects/list.h"
#include "objects/dict.h"
#include "objects/frame.h"
#include "objects/func.h"

#define YYSTYPE Cute
#include "y.tab.h"


    // 変数宣言 ---------------------------------------------------------------

extern Cute g_global;
extern Cute g_env;



    // 関数宣言 ---------------------------------------------------------------

Cute H_assign( Cute id, Cute val );
Cute H_op( Type op_type, Cute left, Cute right );
Cute H_while( Cute cond, Cute block );
Cute H_for( Cute id, Cute cond, Cute block );
Cute H_if( Cute cond, Cute if_block, Cute else_block );
Cute H_return( Cute exp );

__attribute__ ((noreturn)) 
__attribute__((format (printf, 1, 2)))
void error( const char *fmt, ... );
Cute eval( Cute exp, Cute env );


#endif
