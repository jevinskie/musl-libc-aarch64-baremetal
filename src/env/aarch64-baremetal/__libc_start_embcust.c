#include <string.h>
#include "atomic.h"
#include "libc.h"

extern weak hidden void (*const __init_array_start)(void), (*const __init_array_end)(void);

#define EMBCUST_MAX_ARGV 32
#define EMBCUST_MAX_ENVP EMBCUST_MAX_ARGV
#define EMBCUST_MAX_INIT_STRPOOL 4096

extern const char **__embcust_argv;
extern const char **__embcust_environ;

char *__embcust_argv_rw[EMBCUST_MAX_ARGV];
char *__embcust_environ_rw[EMBCUST_MAX_ENVP];
char __embcust_init_strpool[EMBCUST_MAX_INIT_STRPOOL];


static void libc_start_embcust_init(void)
{
    uintptr_t a = (uintptr_t)&__init_array_start;
    for (; a<(uintptr_t)&__init_array_end; a+=sizeof(void(*)()))
        (*(void (**)(void))a)();
}

typedef int lsm2_fn();
static lsm2_fn libc_start_embcust_stage2;

int __libc_start_embcust(void)
{
    int argc = 0;
    for (const char **arg = __embcust_argv; *arg; ++argc);
    if (argc + 1 > EMBCUST_MAX_ARGV)
        __builtin_trap();

    int envc = 0;
    for (const char **env = __embcust_environ; *env; ++envc);
    if (envc + 1 > EMBCUST_MAX_ENVP)
        __builtin_trap();

    char *strpool_end = __embcust_init_strpool + sizeof(__embcust_init_strpool);
    char *strpool = __embcust_init_strpool;
    for (int i = 0; i < argc; ++i) {
        size_t len = strlen(__embcust_argv[i]);
        if (strpool + len + 1 >= strpool_end)
            __builtin_trap();
        memcpy(strpool, __embcust_argv[i], len + 1);
        __embcust_argv_rw[i] = strpool;
        strpool += len + 1;
    }
    __embcust_argv_rw[argc] = NULL;
    for (int i = 0; i < envc; ++i) {
        size_t len = strlen(__embcust_environ[i]);
        if (strpool + len + 1 >= strpool_end)
            __builtin_trap();
        memcpy(strpool, __embcust_environ[i], len + 1);
        __embcust_environ_rw[i] = strpool;
        strpool += len + 1;
    }
    __embcust_environ_rw[envc] = NULL;

    char **argv = __embcust_argv_rw;
    char **envp = __embcust_environ_rw;

    /* External linkage, and explicit noinline attribute if available,
     * are used to prevent the stack frame used during init from
     * persisting for the entire process lifetime. */
    __init_libc(envp, argv[0]);

    /* Barrier against hoisting application code or anything using ssp
     * or thread pointer prior to its initialization above. */
    lsm2_fn *stage2 = libc_start_embcust_stage2;
    __asm__ ( "" : "+r"(stage2) : : "memory" );
    return stage2();
}

static int libc_start_embcust_stage2()
{
    libc_start_embcust_init();
    return 0;
}
