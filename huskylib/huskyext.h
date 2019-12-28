/* $Id$
 *

 * This file may be included many times.
 *
 * Declare compiler-specific macro:
 *
 * HUSKYEXT - external variables & external functions call modifier
 *            used for export/import functions for dll (shared library) build
 *            (May by set to 'extern' or stay empty for static linkage)
 *            Use DLLEXPORT macro to switch export or import functions
 *            modifier set.
 */

#include "compiler.h"  /* compiler see directory of this .h file */

#if defined(DLLEXPORT) && defined(DLLIMPORT)
    # error "DLLEXPORT and DLLIMPORT can't be defined simultaneously!"
#endif

#undef HUSKYEXT
#ifdef _MAKE_DLL
    #if defined(__MSVC__) || defined(__MINGW32__)
        #if defined(DLLEXPORT) && !defined(DLLIMPORT)
            #define HUSKYEXT __declspec(dllexport)
        #else
            #define HUSKYEXT __declspec(dllimport)
        #endif /* DLLEXPORT */
    #else
        #define HUSKYEXT extern
    #endif
#else
    #define HUSKYEXT extern
#endif
