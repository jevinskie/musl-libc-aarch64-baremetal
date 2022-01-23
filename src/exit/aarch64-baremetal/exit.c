#include <stdlib.h>
#include <stdint.h>
#include "libc.h"

static void dummy()
{
}

/* atexit.c and __stdio_exit.c override these. the latter is linked
 * as a consequence of linking either __toread.c or __towrite.c. */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __stdio_exit);

extern void (*const *__fini_array_embcust_start)(void), (*const *__fini_array_embcust_end)(void);

static void libc_exit_fini(void)
{
    uintptr_t a = (uintptr_t)__fini_array_embcust_start;
    for (; a>(uintptr_t)__fini_array_embcust_end; a-=sizeof(void(*)()))
        (*(void (**)())(a-sizeof(void(*)())))();
}

weak_alias(libc_exit_fini, __libc_exit_fini);

_Noreturn void exit(int code)
{
    __funcs_on_exit();
    __libc_exit_fini();
    __stdio_exit();
    _Exit(code);
}
