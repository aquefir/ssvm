/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include <tes/battery.h>

#include <uni/log.h>
#include <uni/memory.h>

#include "lexer.h"

static void print_strlit( struct tok_strlit lit )
{
	char bla[lit.str_sz + 1];

	uni_memcpy( bla, (const void *)lit.str, lit.str_sz );
	bla[lit.str_sz] = '\0';

	uni_print( "[STRLIT] %s", bla );
}

static void print_ident( struct tok_ident id )
{
	char bla[id.str_sz + 1];

	uni_memcpy( bla, (const void *)id.str, id.str_sz );
	bla[id.str_sz] = '\0';

	uni_print( "[IDENT] %s", bla );
}

TES_OPEN( );

const char * script =
	"const foo = 'Ha\\'ha!'\nconst fun = () => {\n\treturn 0\n}\n";
struct tok * ret;
ptri i;

ret = lex( script );
TES_ASSERT_NE( ret, NULL );

for(i = 0; ret[i].type != MAX_T; ++i)
{
	switch(ret[i].type)
	{
	case T_STRLIT:
		print_strlit( ret[i].strlit );
		break;
	case T_NUM:
		uni_print( "[NUM] %i\n", ret[i].num.i );
		TES_ASSERT_EQ( ret[i].num.i, 0 );
		break;
	case T_KEYWORD:
		uni_print( "[KEYWORD] %s", get_keyword( ret[i].keyword.keyword ) );
		break;
	case T_IDENT:
		print_ident( ret[i].ident );
		break;
	case T_NEWLINE:
		uni_print( "[NEWLINE]" );
		break;
	default:
		break;
	}
}

printf( "\n\n" );

TES_CLOSE( );
