#include <compiler.h>
#include <typedefs.h>

#ifndef __LITTLE_ENDIAN__
/*
 *  put_dword
 *
 *  Writes a 4 byte word in little endian notation, independent of the local
 *  system architecture.
 */

void put_dword(byte *ptr, dword value)
{
    ptr[0] = (value & 0xFF);
    ptr[1] = (value >> 8) & 0xFF;
    ptr[2] = (value >> 16) & 0xFF;
    ptr[3] = (value >> 24) & 0xFF;
}

/*
 *  put_word
 *
 *  Writes a 4 byte word in little endian notation, independent of the local
 *  system architecture.
 */

void put_word(byte *ptr, word value)
{
    ptr[0] = (value & 0xFF);
    ptr[1] = (value >> 8) & 0xFF;
}
#endif
