#ifndef _FSCMN_H
#define _FSCMN_H

#include "fidoconf/fidoconf.h"

FCONF_EXT int move_file(const char *from, const char *to);
/* DOC
   Input:  source and destination filename
   Output: 0 if OK, != 0 and errno set on error
   FZ:     Move a file, works even over file system boundaries, or if
           the destination file already exists.
*/

FCONF_EXT int copy_file(const char *from, const char *to);

FCONF_EXT char *makeUniqueDosFileName(const char *dir, const char *ext, s_fidoconfig *config);
/*DOC
   Input:  dir: \0 terminated string designating the destination directory
           ext: \0 terminated string designating the file extension w/o dot
           config: is used to generate a node specific offset         
   Output: a pointer to a malloc'ed \0 terminated string is returned.
   FZ:     Creates a unique DOS compatible file name inside the given directory.
           See the comments in common.c for further explanations
*/

FCONF_EXT int  NCreateOutboundFileName(ps_fidoconfig config, s_link *link, e_flavour prio, e_pollType typ);
/*DOC
  Input:  link is the link whose OutboundFileName should be created.
          prio is some kind of CRASH, HOLD, NORMAL
          typ is some kind of PKT, REQUEST, FLOFILE
  Output: a pointer to a char is returned.
  FZ:     1 is returned if link is busy
         -1 can't create bsy file
          0 else
          */

// fileBoxes support

FCONF_EXT int needUseFileBoxForLink (ps_fidoconfig config, s_link *link);
FCONF_EXT char *makeFileBoxName     (ps_fidoconfig config, s_link *link);
FCONF_EXT void fillCmdStatement(char *cmd, const char *call, const char *archiv, const char *file, const char *path);
FCONF_EXT char* changeFileSuffix(char *fileName, char *newSuffix);


#endif