/****************************************************************************\
 *                       SundaScript Virtual Machine™                       *
 *                                                                          *
 *                         Copyright © 2020 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include "validstr.h"

#include <uni/types/int.h>

int chk_ascii( const char * str, ptri str_sz )
{
	ptri i;

	for( i = 0; i < str_sz; ++i )
	{
		if( ( u8 )( str[i] ) > 0x7F )
		{
			/* NOTE: the public API allows us to return anything
			 * besides 0 */
			return 1;
		}
	}

	return 0;
}
