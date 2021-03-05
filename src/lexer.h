/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#ifndef INC__SSVM_LEXER_H
#define INC__SSVM_LEXER_H

#include <stdlib.h>
#include <uni/err.h>
#include <uni/memory.h>
#include <uni/str.h>
#include <uni/types/float.h>
#include <uni/types/int.h>

#include "validstr.h"

typedef u8 tokid_t;
typedef u8 keyword_t;

enum
{
	/* String literals */
	T_STRLIT,
	/* Numeric literals */
	T_NUM,
	/* NOTE: also subsumes operators */
	T_KEYWORD,
	/* Strings of text not otherwise reserved */
	T_IDENT,
	/* this bounds the token types and also signifies the end of an array
	 * of type ‘struct tok *’ */
	MAX_T
};

enum
{
	K_LET,
	K_CONST,
	K_IF,
	K_ELSE,
	K_DO,
	K_WHILE,
	K_FOR,
	K_CONTINUE,
	K_BREAK,
	K_SWITCH,
	K_CASE,
	K_RETURN,
	K_TYPEOF,
	K_SIZEOF,
	K_LBRACE,
	K_RBRACE,
	K_LPAREN,
	K_RPAREN,
	K_LBRACK,
	K_RBRACK,
	K_DOTDOTDOT,
	K_DOT,
	K_PLUSSET,
	K_MINUSSET,
	K_STARSTARSET,
	K_STARSET,
	K_SLASHSET,
	K_PERCENTSET,
	K_LSHIFSET,
	K_RSHIFSET,
	K_ANDANDSET,
	K_ORORSET,
	K_ANDSET,
	K_ORSET,
	K_XORSET,
	K_ARROW,
	K_LSHIF,
	K_RSHIF,
	K_LE,
	K_GE,
	K_EQ,
	K_NE,
	K_NOTNOT,
	K_NOT,
	K_SET,
	K_PLUSPLUS,
	K_MINUSMINUS,
	K_PLUS,
	K_MINUS,
	K_BITNOT,
	K_STARSTAR,
	K_STAR,
	K_SLASH,
	K_PERCENT,
	K_LT,
	K_GT,
	K_ANDAND,
	K_OROR,
	K_AND,
	K_OR,
	K_XOR,
	K_QMARK,
	K_COLON,
	MAX_K
};

struct tok_strlit
{
	/* this be a string that is valid ASCII */
	char * str;
	ptri str_sz;
};

struct tok_num
{
	u8 is_int;
	union
	{
		f32 f;
		s32 i;
	};
};

struct tok_keyword
{
	keyword_t keyword;
};

struct tok_ident
{
	/* this be a string that is valid ASCII */
	char * str;
	ptri str_sz;
};

struct tok
{
	tokid_t type;
	union
	{
		struct tok_strlit strlit;
		struct tok_num num;
		struct tok_keyword keyword;
		struct tok_ident ident;
	};
};

const char * get_keyword( ptri );
struct tok * lex( const char * );

#endif /* INC__SSVM_LEXER_H */
