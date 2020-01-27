#if !defined(STDLIB_FILES_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   ======================================================================== */

enum StandardLibraryFile
{
    STDLIB_FILE_ASSERT,		
    STDLIB_FILE_COMPLEX,	
    STDLIB_FILE_CTYPE,		
    STDLIB_FILE_ERRNO,		
    STDLIB_FILE_FENV,	
    STDLIB_FILE_FLOAT,		
    STDLIB_FILE_INTTYPES,	
    STDLIB_FILE_ISO646,	
    STDLIB_FILE_LIMITS,		
    STDLIB_FILE_LOCALE,		
    STDLIB_FILE_MATH,		
    STDLIB_FILE_SETJMP,		
    STDLIB_FILE_SIGNAL,		
    STDLIB_FILE_STDALIGN,	
    STDLIB_FILE_STDARG,		
    STDLIB_FILE_STDATOMIC,	
    STDLIB_FILE_STDBOOL,	
    STDLIB_FILE_STDDEF,		
    STDLIB_FILE_STDINT,	
    STDLIB_FILE_STDIO,		
    STDLIB_FILE_STDLIB,		
    STDLIB_FILE_STDNORETURN,
    STDLIB_FILE_STRING,		
    STDLIB_FILE_TGMATH,	
    STDLIB_FILE_THREADS,
    STDLIB_FILE_TIME,	
    STDLIB_FILE_UCHAR,	
    STDLIB_FILE_WCHAR,	
    STDLIB_FILE_WCTYPE,
    STDLIB_FILE_WINDOWS,
    STDLIB_FILE_XINPUT,
    STDLIB_FILE_PSAPI,
    STDLIB_FILE_DSOUND,
    STDLIB_FILE_COUNT
};

char* stdlib_filenames[STDLIB_FILE_COUNT] =
{
    "<assert.h>",		
    "<complex.h>",	
    "<ctype.h>",		
    "<errno.h>",		
    "<fenv.h>",	
    "<float.h>",		
    "<inttypes.h>",	
    "<iso646.h>",	
    "<limits.h>",		
    "<locale.h>",		
    "<math.h>",		
    "<setjmp.h>",		
    "<signal.h>",		
    "<stdalign.h>",	
    "<stdarg.h>",		
    "<stdatomic.h>",	
    "<stdbool.h>",	
    "<stddef.h>",		
    "<stdint.h>",	
    "<stdio.h>",		
    "<stdlib.h>",		
    "<stdnoreturn.h>",
    "<string.h>",		
    "<tgmath.h>",	
    "<threads.h>",
    "<time.h>",	
    "<uchar.h>",	
    "<wchar.h>",	
    "<wctype.h>",
    "<windows.h>",
    "<xinput.h>",
    "<psapi.h>",
    "<dsound.h>"
};    
#define STDLIB_FILES_H
#endif
