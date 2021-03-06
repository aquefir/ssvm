/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include "lexer.h"

static const char * const keywords[MAX_K] = { "let",
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
	"typeof",
	"sizeof",
	"...",
	".",
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
	":" };

static const ptri keywords_sz[MAX_K] = { 3,
	5,
	2,
	4,
	2,
	5,
	3,
	8,
	5,
	6,
	4,
	6,
	6,
	6,
	3,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	2,
	2,
	3,
	2,
	2,
	2,
	3,
	3,
	3,
	3,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	2,
	2,
	1,
	1,
	1,
	2,
	1,
	1,
	1,
	1,
	1,
	2,
	2,
	1,
	1,
	1,
	1,
	1 };

const char * get_keyword( ptri i )
{
	if( i > MAX_K )
	{
		i = MAX_K;
	}

	return keywords[i];
}

static int nz_strequ( unsigned index, const char * b )
{
	unsigned i;

	for( i = 0; i < keywords_sz[index]; ++i )
	{
		if( b[i] == '\0' || keywords[index][i] != b[i] )
		{
			return 0;
		}
	}

	return 1;
}

static ptri scan_keyword( const char * in )
{
	unsigned i;

	for( i = 0; i < MAX_K; ++i )
	{
		if( nz_strequ( i, in ) )
		{
			return keywords_sz[i];
		}
	}

	return 0;
}

static ptri scan_ident( const char * in )
{
	/* regex := [A-Za-z_][A-Za-z0-9_]* */
	unsigned i = 0;

	if( !( ( in[i] >= 'a' && in[i] <= 'z' ) ||
		    ( in[i] >= 'A' && in[i] <= 'Z' ) || in[i] == '_' ) )
	{
		return 0;
	}

	i++;

	while( ( ( in[i] >= 'a' && in[i] <= 'z' ) ||
		( in[i] >= 'A' && in[i] <= 'Z' ) ||
		( in[i] >= '0' && in[i] <= '9' ) || in[i] == '_' ) )
	{
		i++;
	}

	return i;
}

static ptri scan_strlit( const char * in )
{
	unsigned i = 0;
	char mark  = '\0';

	if( in[i] != '"' && in[i] != '\'' )
	{
		return 0;
	}

	mark = in[i];

	i++;

	while( in[i] != mark ||
		( i > 0 && in[i] == mark && in[i - 1] == '\\' ) )
	{
		i++;
	}

	if( in[i] == mark )
	{
		return i + 1;
	}

	return 0;
}

enum
{
	NUM_DEC = 0,
	NUM_OCT,
	NUM_HEX,
	NUM_BIN
};

static ptri scan_num( const char * in )
{
	unsigned i = 0;
	unsigned numtyp;

	if( in[i] == '0' )
	{
		switch( in[i + 1] )
		{
		case 'x':
		case 'X':
			numtyp = NUM_HEX;
			break;
		case 'o':
		case 'O':
			numtyp = NUM_OCT;
			break;
		case 'b':
		case 'B':
			numtyp = NUM_BIN;
			break;
		case '\0':
			return 0;
		default:
			numtyp = NUM_DEC;
			break;
		}

		i += numtyp == NUM_DEC ? 1 : 2;
	}
	else
	{
		numtyp = NUM_DEC;
	}

	while( ( numtyp == NUM_DEC && in[i] >= '0' && in[i] <= '9' ) ||
		( numtyp == NUM_OCT && in[i] >= '0' && in[i] <= '7' ) ||
		( numtyp == NUM_BIN && in[i] >= '0' && in[i] <= '1' ) ||
		( numtyp == NUM_HEX &&
			( ( in[i] >= '0' && in[i] <= '9' ) ||
				( in[i] >= 'A' && in[i] <= 'F' ) ||
				( in[i] >= 'a' && in[i] <= 'f' ) ) ) )
	{
		i++;
	}

	return i;
}

static ptri scan_label( const char * in ) { return in[0] == ':' ? 1 : 0; }

static ptri scan_newln( const char * in )
{
	return ( in[0] == '\r' && in[1] == '\n' )    ? 2
		: ( in[0] == '\r' || in[0] == '\n' ) ? 1
						     : 0;
}

/* this is called directly by lex(), unlike other scanners */
static ptri scan_wspc( const char * in, int * n )
{
	unsigned i = 0;
	int n2     = -1;

	for( ;; )
	{
		if( in[i] == ' ' || in[i] == '\t' )
		{
			i++;
		}
		else
		{
			const unsigned j = scan_newln( &( in[i] ) );

			if( !j )
			{
				break;
			}

			if( n2 == -1 )
			{
				n2 = j > 0 ? 1 : 0;
			}

			i += j;
		}
	}

	*n = n2 >= 0 ? n2 : 0;

	return i;
}

static ptri scan_ret0( const char * in )
{
	(void)in;
	return 0;
}

typedef ptri ( *PFN_scan )( const char * );

static const PFN_scan scanners[MAX_T] = {
	scan_strlit, scan_num, scan_keyword, scan_ident, scan_ret0 };

static int mapkeyword( const char * in, ptri in_sz )
{
	int i;
	char str[in_sz + 1];

	uni_memcpy( str, in, in_sz );
	str[in_sz] = '\0';

	for( i = 0; i < MAX_K; ++i )
	{
		if( uni_strequ( str, keywords[i] ) )
		{
			return i;
		}
	}

	return -1;
}

static s32 strtos32( const char * in, ptri in_sz )
{
	long ret;
	char str[in_sz + 1];

	uni_memcpy( str, in, in_sz );
	str[in_sz] = '\0';

	if( in_sz >= 3 && str[0] == '0' )
	{
		switch( str[1] )
		{
		case 'x':
		case 'X':
			ret = strtol( str + 2, NULL, 16 );
			break;
		case 'o':
		case 'O':
			ret = strtol( str + 2, NULL, 8 );
			break;
		case 'b':
		case 'B':
			ret = strtol( str + 2, NULL, 2 );
			break;
		case '\0':
			return 0;
		default:
			ret = strtol( str, NULL, 10 );
			break;
		}
	}
	else
	{
		ret = strtol( str, NULL, 10 );
	}

	return ret >= S32_MAX ? S32_MAX : ret <= S32_MIN ? S32_MIN : (s32)ret;
}

#define LEX_ASSERT( _cnd, _msg ) \
	do \
	{ \
		if( _cnd ) \
		{ \
		} \
		else \
		{ \
			uni_perror( "LEXER ERROR: %s\n", _msg ); \
			return NULL; \
		} \
	} while( 0 )

struct tok * lex( const char * in )
{
	struct tok * ret;
	u8 * str;
	ptri ret_sz = 0, ret_cap = 16;
	const ptri in_sz = uni_strlen( in );
	ptri i;

	if( !in )
	{
		uni_die( );
	}

	ret = uni_alloc( sizeof( struct tok ) * ret_cap );
	str = (u8 *)in;

	for( i = 0; i < in_sz; ++i )
	{
		ptri j;
		int n = 0;

		if( ret_sz >= ret_cap )
		{
			ret_cap <<= 1; /* *= 2 */
			ret = uni_realloc(
				ret, sizeof( struct tok ) * ret_cap );
		}

		/* scan ahead for any whitespace first */
		i += scan_wspc( (const char *)( str + i ), &n );

		if( n )
		{
			/* there was a newline. that’s the token */
			struct tok curtok;

			curtok.type = T_NEWLINE;

			uni_memcpy( &( ret[ret_sz] ),
				&curtok,
				sizeof( struct tok ) );
			ret_sz++;

			i--;

			continue;
		}

		for( j = 0; j < MAX_T; ++j )
		{
			/* run through token-producing scanners */
			const ptri len =
				scanners[j]( (const char *)( str + i ) );

			if( len > 0 )
			{
				struct tok curtok;

				/* set this before accessing into the union */
				curtok.type = j;

				if( j == T_NUM )
				{
					s32 num = strtos32(
						(const char *)( str + i ),
						len );
					curtok.num.i      = num;
					curtok.num.is_int = 1;
				}
				else if( j == T_STRLIT )
				{
					LEX_ASSERT(
						!chk_ascii(
							(const char *)( str +
								1 + i ),
							len - 1 ),
						"String has invalid (non-ASCII) characters" );
					curtok.strlit.str_sz =
						len - 1; /* - 1 for quotes */
					curtok.strlit.str = uni_alloc(
						curtok.strlit.str_sz );
					uni_memcpy( curtok.strlit.str,
						(u8 *)str + i + 1,
						len - 1 );
					curtok.strlit.str[len - 2] = '\0';
				}
				else if( j == T_KEYWORD )
				{
					const int key = mapkeyword(
						(const char *)( str + i ),
						len );
					LEX_ASSERT(
						key >= 0, "Invalid keyword" );
					curtok.keyword.keyword = key;
				}
				else if( j == T_IDENT )
				{
					LEX_ASSERT(
						!chk_ascii(
							(const char *)( str +
								i ),
							len ),
						"String has invalid (non-ASCII) characters" );
					curtok.ident.str_sz = len + 1;
					curtok.ident.str    = uni_alloc(
                                                curtok.ident.str_sz );
					uni_memcpy( curtok.ident.str,
						(u8 *)str + i,
						len );
					curtok.ident.str[len] = '\0';
				}
				/* for now no else is necessary, because of the
				 * loop condition */

				uni_memcpy( &( ret[ret_sz] ),
					&curtok,
					sizeof( struct tok ) );

				ret_sz++;
				i += len - 1; /* - 1 because of ++i */

				break;
			}
		}
	}

	{
		struct tok end;

		end.type = MAX_T;

		if( ret_sz >= ret_cap )
		{
			ret_cap += 1;
			ret = uni_realloc(
				ret, sizeof( struct tok ) * ret_cap );
		}

		uni_memcpy( &( ret[ret_sz] ), &end, sizeof( struct tok ) );

		ret_sz++;
	}

	return ret;
}
