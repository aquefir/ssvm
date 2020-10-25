/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#ifndef INC__SSVM_LEXER_H
#define INC__SSVM_LEXER_H

#include <uni/types/float.h>
#include <uni/types/int.h>

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
	/* this bounds the token types and also signifies the end of an array of
	 * type ‘struct tok *’ */
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
	K_LBRACE,
	K_RBRACE,
	K_LPAREN,
	K_RPAREN,
	K_LBRACK,
	K_RBRACK,
	K_UPLUS,
	K_UMINUS,
	K_INCR,
	K_DECR,
	K_BWNOT,
	K_NOT,
	K_BLNOT,
	K_TYPEOF,
	K_SIZEOF,
	K_ADD,
	K_SUB,
	K_MUL,
	K_DIV,
	K_MOD,
	K_POW,
	K_LT,
	K_GT,
	K_LE,
	K_GE,
	K_EQ,
	K_NE,
	K_LSHIF,
	K_RSHIF,
	K_AND,
	K_OR,
	K_XOR,
	K_ANDAND,
	K_OROR,
	K_QMARK,
	K_COLON,
	K_SET,
	K_ADDSET,
	K_SUBSET,
	K_MULSET,
	K_DIVSET,
	K_MODSET,
	K_POWSET,
	K_LSHIFSET,
	K_RSHIFSET,
	K_ANDSET,
	K_ORSET,
	K_XORSET,
	K_ANDANDSET,
	K_ORORSET,
	K_ARROW,
	MAX_K
};

struct tok_strlit
{
	/* this be a NUL-terminated string that is valid ASCII */
	char* str;
};

struct tok_num
{
	u8 is_int;
	union
	{
		f32 f;
		s32 i;
	}
	num;
};

struct tok_keyword
{
	keyword_t keyword;
};

struct tok
{
	tokid_t type;
	union
	{
		struct tok_strlit strlit;
		struct tok_num num;
		struct tok_keyword keyword;
	}
	data;
};

struct tok* lex( const char* );

#endif /* INC__SSVM_LEXER_H */
