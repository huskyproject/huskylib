/* $Id$
 *
 * HUSKYLIB: common defines, types and functions for HUSKY
 *
 * This is part of The HUSKY Fidonet Software project:
 * see http://husky.sourceforge.net for details
 *
 *
 * HUSKYLIB is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * HUSKYLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see file COPYING. If not, write to the
 * Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * See also http://www.gnu.org, license may be found here.
 */

/* standard headers */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* huskylib: compiler.h */
#include <compiler.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

#include <ftnaddr.h>


/*
 *	Function parse fidonet frl-1002.001 address.
 *	*netAddr must be initialized by zero or default values.
 *	mask gives mandatory values in address string.
 *	Returns mask of explicitly given values, or'ed with
 *	FTNADDR_ERROR if can't parse or if not all mandatory values are given
 *  *end -- pointer to character that stops scan
 */

int parseFtnAddr(const char *str, hs_addr *netAddr, int len, int mask, const char **end)
{
char *allocated = NULL;
const char *end_loc;
int  result = 0;

	assert(str);
	assert(netAddr);


	allocated = (char*)malloc(len + 1);
	if (!allocated)
	{
		return FTNADDR_ERROR;
	}

	memcpy(allocated, str, len);
	allocated[len] = '\0';

	result = parseFtnAddrZ(allocated, netAddr, mask, &end_loc);
	if(end != NULL)
		*end = str + (end_loc - allocated);

	free(allocated);

return result;
}

int parseFtnAddrZ(const char *str, hs_addr *netAddr, int mask, const char **end)
{
const char s[] = {'\0', ':', '/', '.', '@', '\0'};
const char *ptr, *tmp;
int result = 0;
size_t sym = 0;
long i = 0, imax = LONG_MAX/10;
unsigned digit, lastdigmax=LONG_MAX%10;

	assert(str);
	assert(netAddr);

	tmp = str;

/*	while (*tmp && isspace(*tmp)) ++tmp;*/ 
	/* isspace may be too much, it skips cr&lf which may be objectionable */
	while (*tmp && (*tmp == ' ' || *tmp == '\t')) ++tmp;

	ptr = tmp;

	if(*tmp != '-')
	{
		while( (digit = (unsigned)(*tmp - '0')) <= 9)
		{
			if(i < imax || (i == imax && digit <= lastdigmax))
				i = i * 10 + digit;
			else
				goto Fail; /* overflow */
			++tmp;
		}
	}
	else
	{
		++tmp;
		if(*tmp != '1')
			goto Fail;
		++tmp;
		i = -1;
	}

	while(++sym < sizeof(s) && *tmp != s[sym]);

	if(ptr != tmp)
	{
		if(sym < 4)
			--sym; /* found next symbol, assume omitted symbol */
		else if(sym < sizeof(s) || isspace(*tmp))
			sym = 2; /* single number or number followed by '@' is node number */
		else
			goto Fail;

		goto Sw;
	}

	while(sym < 4)
	{
		ptr = ++tmp;
		i = 0;

		if(*tmp != '-')
		{
			while( (digit = (unsigned)(*tmp - '0')) <= 9)
			{
				if(i < imax || (i == imax && digit <= lastdigmax))
					i = i * 10 + digit;
				else
					goto Fail; /* overflow */
				++tmp;
			}
			if(ptr == tmp)
				goto Fail;
		}
		else
		{
			++tmp;
			if(*tmp != '1')
				goto Fail;
			++tmp;
			i = -1;
		}
Sw:
		switch(sym)
		{
			case 0:
				if(i > 0 && i <= 32767)
				{
					netAddr->zone = (sword)i;
					result = FTNADDR_ZONE;
				}
				else
					goto Fail;
				break;
			case 1:
				if(i > 0 && i <= 32767)
				{
					netAddr->net = (sword)i;
					result |= FTNADDR_NET;
				}
				else
					goto Fail;
				break;
			case 2:
				if(i >= -1 && i <= 32767)
				{
					netAddr->node = (sword)i;
					result |= FTNADDR_NODE;
				}
				else
					goto Fail;
				break;
			case 3:
				if(i >= 0 && i <= 32767)
				{
					netAddr->point = (sword)i;
					result |= FTNADDR_POINT;
				}
				else
					goto Fail;
				break;
		}

		if(*tmp != s[++sym])
		{
			while(++sym <= sizeof(s) && *tmp != s[sym]);
			if(sym < 4)
				goto Fail;
		}
	}

	if(sym == 4)
	{
		ptr = ++tmp;
		i = 0;
		while((i < 8) && *tmp && (isdigit(*tmp) || isalpha(*tmp)))
		{
			netAddr->domain[i] = *tmp;
			++tmp;
			++i;
		}
		netAddr->domain[i] = 0;
		result |= FTNADDR_DOMAIN;
	}
#if 0
/* Lets be loose for now, if we parsed enough from string then
 * it's ok and we don't really care which character stops scan. */	
	else if(sym >= sizeof(s) && !isspace(*tmp) && 
			/* any symbols that can rightfully terminate address */
			!(*tmp == ',' || *tmp == ')')) 
		goto Fail;
#endif
	if(~result & mask)
		goto Fail;

	if(end != NULL)
		*end = tmp;
return result;
Fail:
	if(end != NULL)
		*end = str; /* in case of error *end points on the beginning of string */
return FTNADDR_ERROR | result;
}
