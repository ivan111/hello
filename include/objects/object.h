#ifndef HEADER_H_OBJECT
#define HEADER_H_OBJECT


    // システム ヘッダ --------------------------------------------------------



    // ローカル ヘッダ --------------------------------------------------------



    // 定数 -------------------------------------------------------------------

#define H_NUM_FLAG  1
#define H_ID_FLAG   2

#define H_None   ( (Cute) NULL )

#define H_VAR_ARGS  (1 << 15)

#define H_0      ( (Cute) (0 | H_NUM_FLAG) )
#define H_1      ( (Cute) ((1 << 1) | H_NUM_FLAG) )
#define H_2      ( (Cute) ((2 << 1) | H_NUM_FLAG) )
#define H_3      ( (Cute) ((3 << 1) | H_NUM_FLAG) )
#define H_1_VAR  ( (Cute) ((1 << 1) | H_NUM_FLAG | H_VAR_ARGS) )
#define H_2_VAR  ( (Cute) ((2 << 1) | H_NUM_FLAG | H_VAR_ARGS) )
#define H_3_VAR  ( (Cute) ((3 << 1) | H_NUM_FLAG | H_VAR_ARGS) )

#define H_False  H_None
#define H_True   H_1

#define H_FUNC_USER  H_1
#define H_FUNC_C     H_2


enum ObjectType {
    T_NONE,
    T_NUM,
    T_ID,
    T_CLASS,
    T_STR,
    T_LIST,
    T_CELL,
    T_DICT,
    T_FRAME,
    T_USER_OBJ,

    // node
    T_ASSIGN,
    T_WHILE,
    T_FOR,
    T_IF,
    T_RETURN,
    T_FUNC,
    T_CALL,
    T_OP_REF,
    T_OP_ADD,
    T_OP_SUB,
    T_OP_MUL,
    T_OP_DIV,
    T_OP_EQ,
    T_OP_NE,
    T_OP_LE,
    T_OP_GE,
    T_OP_LT,
    T_OP_GT,

    T_MAX_BUILTIN,
};

typedef enum ObjectType Type;


    // 構造体 -----------------------------------------------------------------

struct TAG_Object {
    union {
        enum ObjectType type;
        struct TAG_Object *airi;
    };

    struct TAG_Object *chisato;
    struct TAG_Object *mai;
};

typedef struct TAG_Object *Cute;



    // マクロ -----------------------------------------------------------------

#define IS_H_NUM(p)  ( (uintptr_t)(p) & H_NUM_FLAG )
#define IS_H_ID(p)   ( (uintptr_t)(p) & H_ID_FLAG )


#define N_type(p)  ( (p)->type )

#define N_assign_id(p)    ( (p)->chisato )
#define N_assign_val(p)   ( (p)->mai )

#define N_print_arg(p)    ( (p)->chisato )

#define N_uobj_class(p)   ( (p)->chisato )
#define N_uobj_iv(p)      ( (p)->mai )

#define N_op_left(p)      ( (p)->chisato )
#define N_op_right(p)     ( (p)->mai )

#define N_while_cond(p)   ( (p)->chisato )
#define N_while_block(p)  ( (p)->mai )

#define N_for_id(p)       ( (p)->chisato )
#define N_for_iter(p)     ( (p)->mai->chisato )
#define N_for_block(p)    ( (p)->mai->mai )

#define N_if_cond(p)      ( (p)->chisato )
#define N_if_if(p)        ( (p)->mai->chisato )
#define N_if_else(p)      ( (p)->mai->mai )

#define N_func_env(p)     ( (p)->chisato )
#define N_func_type(p)    ( (p)->mai->airi )
#define N_func_params(p)  ( (p)->mai->chisato )
#define N_func_body(p)    ( (p)->mai->mai )

#define N_call_id(p)      ( (p)->chisato )
#define N_call_args(p)    ( (p)->mai )

#define N_return_exp(p)   ( (p)->chisato )


static inline Type
H_get_type( Cute p )
{
    if ( p == H_None )
    {
        return T_NONE;
    }
    else if ( IS_H_NUM(p) )
    {
        return T_NUM;
    }
    else if ( IS_H_ID(p) )
    {
        return T_ID;
    }
    else
    {
        return N_type(p);
    }
}



static inline bool
to_c_cond( Cute p )
{
    if ( p == H_None )
        return false;
    else if ( p == H_0 )
        return false;
    else
        return true;
}



    // 変数宣言 ---------------------------------------------------------------

//extern struct H_Class *g_cObject;


    // 関数宣言 ---------------------------------------------------------------

void init_free_objs( void );
Cute alloc_obj( Type type );
Cute alloc_objs( Type type, int num );
void free_obj( Cute obj );

Cute H_print_cute( Cute p );
Cute H_print( Cute vargs );
Cute H_dbg_print( Cute p );

void init_type_name_table( void );
char *H_get_type_str( Cute p );
char *H_type_to_str( Type t );


#endif
