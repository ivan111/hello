%{

#include "hello.h"
#include "y.tab.h"

#define MAX_STR_LIT_LEN  4095


int g_line_no = 1;

bool g_colon;
int g_lv;

char g_q_char;
char g_str_lit[MAX_STR_LIT_LEN + 1];
size_t g_str_lit_len;


void add_str_lit_char( char ch );
void check_str_lit_len( void );
char conv_esc_char( char ch );
int yyerror( char *s );


%}


%x SOL
%x Q_STRING



%%


<SOL>"\n"  {
    /* skip */
    g_line_no++;
}



    /* 行頭の空白によってブロックを判断する */
<SOL>" "*[^ ]  {
    BEGIN ( INITIAL );

    int len = yyleng - 1;

    if ( len % 4 )
        yyerror( "level error" );

    int prev_lv = g_lv;
    g_lv = len / 4;

    //printf( "cur lv = %d, next lv = %d\n", prev_lv, g_lv );

    if ( g_lv > prev_lv )
    {
        if ( g_lv   >   prev_lv + 1 )
            yyerror( "lv too large" );

        if ( g_colon == false )
            yyerror( "missing colon" );

        //printf( "change lv = %d\n", g_lv );

        g_colon = false;

        yyless ( len );

        return '{';
    }
    else if ( g_lv < prev_lv )
    {
        if ( g_colon == true )
            yyerror( "colon" );

        if ( g_lv == prev_lv - 1 )
        {
            yyless ( len );
        }
        else
        {
            /* 2 段以上レベルを下げる場合は、入力を戻して、もう一度この規則をやり直す */
            g_lv = prev_lv - 1;
            BEGIN ( SOL );
            yyless ( 0 );
        }

        //printf( "change lv = %d\n", g_lv );

        return '}';
    }

    yyless ( len );

    g_colon = false;
}



^" "  {
    yyerror( "start level error" );
}



#.*  { /* skip comment */ }



    /* ---------- string begin ---------- */

\'  { 
    g_str_lit_len = 0;
    g_q_char = '\'';

    BEGIN ( Q_STRING );
}

\"  { 
    g_str_lit_len = 0;
    g_q_char = '"';

    BEGIN ( Q_STRING );
}

<Q_STRING>\n        {
    yyerror( "Unterminated string" );

    BEGIN ( INITIAL );
}

<Q_STRING><<EOF>>  {
    yyerror( "EOF in string" );

    BEGIN ( INITIAL );
}

<Q_STRING>\\\n  { /* エスケープされた改行を無視する */ }

<Q_STRING>\\[^\n]  {
    char ch = conv_esc_char( yytext[1] );

    add_str_lit_char( ch );
}

<Q_STRING>'  {
    if ( g_q_char == '"' )
    {
        add_str_lit_char( '\'' );
    }
    else
    {
        char *s = &g_str_lit[g_str_lit_len];
        *s = '\0';

        BEGIN ( INITIAL );

        yylval = H_str2( g_str_lit, g_str_lit_len );

        return STRING;
    }
}

<Q_STRING>\"  {
    if ( g_q_char == '\'' )
    {
        add_str_lit_char( '"' );
    }
    else
    {
        char *s = &g_str_lit[g_str_lit_len];
        *s = '\0';

        BEGIN ( INITIAL );

        yylval = H_str2( g_str_lit, g_str_lit_len );

        return STRING;
    }
}

<Q_STRING>[^\\\n"]  {
    add_str_lit_char( yytext[0] );
}

    /* ---------- string end ---------- */



"class"  { return CLASS; }
"def"    { return DEF; }
"while"  { return WHILE; }
"for"    { return FOR; }
"in"     { return IN; }
"if"     { return IF; }
"else"   { return ELSE; }
"return" { return RETURN; }



[a-z_]+  {
    yylval = H_id( yytext );

    return ID;
}



0  {
    yylval = H_0;

    return NUMBER;
}



[1-9][0-9]*  {
    ssize_t val = atoi( yytext );
    yylval = H_num( val );

    return NUMBER;
}



[-=+*/<>(),.\[\]]  {
    return yytext[0];
}



" <= "   { return OP_LE; }
" >= "   { return OP_GE; }
" == "   { return OP_EQ; }
" != "   { return OP_NE; }



":"$  {
    g_colon = true;

    return ':';
}



"\n"  {
    BEGIN ( SOL );

    g_line_no++;

    return EOL;
}



<<EOF>>  {
    if ( g_lv > 0 )
    {
        g_lv--;

        return '}';
    }

    return 0;
}



" "



.  {
    printf( "**** ERROR **** :  %s\n", yytext );

    exit( 1 );
}



%%


void
add_str_lit_char( char ch )
{
    char *s = &g_str_lit[g_str_lit_len];

    *s = ch;

    g_str_lit_len += 1;

    check_str_lit_len();
}



void
check_str_lit_len( void )
{
    if ( g_str_lit_len > MAX_STR_LIT_LEN )
        yyerror( "too long string literal" );
}



char
conv_esc_char( char ch )
{
    if ( ch == 'b' )
        return '\b';
    else if ( ch == 't' )
        return '\t';
    else if ( ch == 'n' )
        return '\n';
    else if ( ch == 'v' )
        return '\v';
    else if ( ch == 'f' )
        return '\f';
    else if ( ch == 'r' )
        return '\r';
    else
        return ch;
}



int
yywrap( void )
{
    return 1;
}
