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

#ifndef __STREXT_H__
#define __STREXT_H__

#include "huskyext.h"  /* compiler see directory of this .h file */

HUSKYEXT char *_fast Strip_Trailing(char *str, char strip);
HUSKYEXT char *_fast Add_Trailing(char *str, char add);

/* Copy src to dst; return src
 */
HUSKYEXT char *_fast strocpy(char *dst, const char *src);

/* Return pointer to 1st char of specified word in string (by word number)
 */
HUSKYEXT char* _fast firstchar(const char *strng, const char *delim, int findword);

/* Extract RCS/CVS keyword value from "$Keyword: value$"
 */
HUSKYEXT char *extract_CVS_keyword(char *str);

HUSKYEXT char *strrstr(const char *HAYSTACK, const char *NEEDLE);

/*DOC
  Input:  two constant null-terminated strings
  Output: NULL or the point to a null-terminated string
  FZ:     finds the LAST occurence of NEEDLE in HAYSTACK
          (same as strstr but last occurence
*/

HUSKYEXT char *fc_stristr(const char *str, const char *find);
/*
 * Find the first occurrence of find in s ignoring case
 */

HUSKYEXT char   *stripLeadingChars(char *str, const char *chr);
/*DOC
  Input:  str is a \0-terminated string
          chr contains a list of characters.
  Output: stripLeadingChars returns a pointer to a string
  FZ:     all leading characters which are in chr are deleted.
          str is changed and returned.
*/


/*DOC
  Input:  str is a \0-terminated string
          chr contains a list of characters.
  Output: stripTrailingChars returns a pointer to a string
  FZ:     all trailing characters which are in chr are deleted.
          str is changed and returned (not reallocated, simply shorted).
*/
HUSKYEXT char *stripTrailingChars(char *str, const char *chr);


#define stripRoundingChars(str,chrs) (stripTrailingChars(stripLeadingChars((str),(chrs)),(chrs)))


HUSKYEXT char   *strUpper(char *str);
/*DOC
  Input:  str is a \0 terminated string
  Output: a pointer to a \0 terminated string is returned.
  FZ:     strUpper converts the string from lower case to upper case.
  */

HUSKYEXT char   *strLower(char *str);
/*DOC
  Input:  str is a \0 terminated string
  Output: a pointer to a \0 terminated string is returned.
  FZ:     strLower converts the string from upper case to lower case.
*/

HUSKYEXT char *sstrdup(const char *src);

/* safe strdup for line part */
#define sstrndup(src,len) strncpy(smalloc(len),src,len)

/* safe strlen: if src==NULL return 0 */
#define sstrlen(src) ( src ? strlen(src) : 0 )

/* safe strcpy: if src==NULL or dst==NULL don't copy and return NULL */
#define sstrcpy(dst,src) ( (src)&&(dst) ? strcpy(dst,src) : NULL )

/* safe strncpy: if src==NULL or dst==NULL don't copy and return NULL */
#define sstrncpy(dst,src,len) ( (src)&&(dst) ? strncpy(dst,src,len) : NULL )

/* Copyes not more than len chars from src into dst, but, unlike strncpy(),
 * it appends '\0' even if src is longer than len.
 * Return dst
 * Prevent memory faults:
 *  - if dst is NULL doing nothing and return NULL
 *  - if src is NULL and dst not NULL store '\0' into dst[0] and return it.
 */
HUSKYEXT char *strnzcpy (char *dst, const char *src, size_t len);

/* Concantenates not more than len chars from src into dst, but, unlike
 * strncat(), it appends '\0' even if src is longer than len.
 * Return dst
 * Prevent memory faults:
 *  - if dst is NULL doing nothing and return NULL
 *  - if src is NULL doing nothing and return dst.
 */
HUSKYEXT char *strnzcat (char *dst, const char *src, size_t len);

/* safe strstr: if str==NULL or find==NULL return NULL */
#define sstrstr(str,find) ( (str)&&(find) ? strstr(str,find) : NULL )

/* safe stristr: if str==NULL or find==NULL return NULL */
#define sstristr(str,find) ( (str)&&(find) ? fc_stristr(str,find) : NULL )

/* safe strcmp */
HUSKYEXT int sstrcmp(const char *str1, const char *str2);

/* safe strncmp */
HUSKYEXT int sstrncmp(const char *str1, const char *str2, size_t length);

/* safe stricmp (case-insencitive) */
HUSKYEXT int sstricmp(const char *str1, const char *str2);

/* safety strnicmp (case-insencitive) */
HUSKYEXT int sstrnicmp(const char *str1, const char *str2, size_t length);

/* safety string envelope (use this in functions calls) */
#define sstr(ss) ( ss ? ss : "" )

/* strsep.c */
HUSKYEXT char *strseparate(register char **stringp, register const char *delim);

HUSKYEXT char *extract_CVS_keyword(char *str);

#endif
