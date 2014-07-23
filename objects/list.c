/******************************************************************************
 *
 * リスト
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



    // 変数宣言 ---------------------------------------------------------------

Cute g_cls_list;


    // 関数宣言 ---------------------------------------------------------------




//=============================================================================
// 公開関数

void
create_list_class( void )
{
    Cute p = create_class( "list" );

    H_class_add_meth0( p, "len", H_list_len );
    H_class_add_meth1( p, "append", H_list_append );
    H_class_add_meth1( p, "__getitem__", H_list_getitem );
    H_class_add_meth1v( p, "__init__", H_list_init );

    g_cls_list = p;
}



Cute
H_cell( Cute item, Cute next )
{
    Cute p = alloc_obj( T_CELL );

    N_cell_item(p) = item;
    N_cell_next(p) = next;

    return p;
}



Cute
H_list( void )
{
    Cute p = alloc_objs( T_LIST, 2 );

    N_list_len(p)  = H_0;
    N_list_head(p) = H_None;
    N_list_tail(p) = H_None;

    return p;
}



Cute
H_list_init( Cute self, Cute args )
{
    H_list_cat( self, args );

    return self;
}



Cute
H_list_len( Cute self )
{
    return N_list_len(self);
}



Cute
H_list_append( Cute self, Cute item )
{
    Cute p = H_cell( item, H_None );

    Cute len = N_list_len(self);

    if ( len == H_0 )
    {
        N_list_head(self) = p;
        N_list_tail(self) = p;
        N_list_len(self)  = H_1;

        return self;
    }

    Cute old_tail = N_list_tail(self);
    N_list_tail(self) = p;
    if ( old_tail )
        N_cell_next(old_tail) = p;

    N_list_len(self) = H_num_inc( len );

    return self;
}



Cute
H_list_cat( Cute self, Cute list )
{
    if ( N_list_len(list) == H_0 )
        return self;

    Cute tail = N_list_tail(self);
    Cute head = N_list_head(list);

    if ( N_list_len(self) == H_0 ) {
        N_list_head(self) = head;
    } else {
        N_cell_next(tail) = head;
    }
    N_list_tail(self) = N_list_tail(list);

    N_list_len(self) = H_num_add( N_list_len(self), N_list_len(list) );

    return self;
}



Cute
H_list_getitem( Cute self, Cute key )
{
    Type type = H_get_type( key );

    if ( type != T_NUM )
        error( "__getitem__: arg1 must be num type. type [%s]", H_type_to_str( type ) );

    size_t idx = H_num_val( key );
    size_t len = H_num_val( N_list_len(self) );

    if ( idx < 0 || idx >= len )
        error( "__getitem__: out of index" );

    Cute cell = N_list_head(self);

    int i;
    for ( i = 0; i < idx; i++ )
    {
        cell = N_cell_next(cell);
    }

    return N_cell_item(cell);
}



Cute
H_list_print( Cute self )
{
    size_t len = H_num_val( N_list_len(self) );

    printf( "%zu [ ", len );
    bool first = true;

    for ( Cute p = N_list_head(self); p; p = N_cell_next(p) )
    {
        if ( first )
            first = false;
        else
            printf( ", " );

        H_print_cute( N_cell_item(p) );
    }

    printf( " ]" );

    return H_None;
}



void H_list_test( void )
{
    printf( "***** list *****\n" );

    Cute p = H_list();
    H_dbg_print( p );

    H_list_append( p, H_num( 3 ) );
    H_dbg_print( p );

    H_list_append( p, H_num( 2 ) );
    H_dbg_print( p );

    H_list_append( p, H_num( 1 ) );
    H_dbg_print( p );

    Cute p2 = H_list();
    H_list_append( p2, H_num( 4 ) );
    H_list_append( p2, H_num( 5 ) );
    H_list_append( p2, H_num( 6 ) );
    H_list_cat( p, p2 );
    H_dbg_print( p );

    printf( "\n" );
}



//=============================================================================
// 非公開関数



