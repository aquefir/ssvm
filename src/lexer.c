/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include "lexer.h"

static const char* keywords[MAX_K] = {
	"let",
	"const",
	"if",
	"else",
	"do",
	"while",
	"for",
	"continue",
	"break",
	"switch",
	"case",
	"return",
	"{",
	"}",
	"(",
	")",
	"[",
	"]",
	"+=",
	"-=",
	"**=",
	"*=",
	"/=",
	"%=",
	"<<=",
	">>=",
	"&&=",
	"||=",
	"&=",
	"|=",
	"^=",
	"=>",
	"<<",
	">>",
	"<=",
	">=",
	"==",
	"!=",
	"!!",
	"!",
	"=",
	"++",
	"--",
	"+",
	"-",
	"~",
	"typeof",
	"sizeof",
	"**",
	"*",
	"/",
	"%",
	"<",
	">",
	"&&",
	"||",
	"&",
	"|",
	"^",
	"?",
	":"
};
