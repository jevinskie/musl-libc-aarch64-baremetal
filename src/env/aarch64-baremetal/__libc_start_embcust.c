#include <string.h>
#include "atomic.h"
#include "libc.h"

extern void (*const *__init_array_embcust_start)(void), (*const *__init_array_embcust_end)(void);

const size_t *_DYNAMIC = NULL;

#define EMBCUST_MAX_INIT_STACK 4096

typedef struct {
  size_t type;
  size_t val;
} auxv_t;

extern const char *__embcust_argv[];
extern const char *__embcust_environ[];
extern const auxv_t __embcust_auxv[];


static char __embcust_init_stack[EMBCUST_MAX_INIT_STACK];


static void libc_start_embcust_init(void)
{
    // unlike a regular linker where __init_array_start is the address of the array
    // the embcust llvm pass instead makes __init_array_embcust_start a pointer to the array
    uintptr_t a = (uintptr_t)__init_array_embcust_start;
    for (; a<(uintptr_t)__init_array_embcust_end; a+=sizeof(void(*)()))
        (*(void (**)(void))a)();
}

typedef int lsm2_fn();
static lsm2_fn libc_start_embcust_stage2;

int __libc_start_embcust(void)
{
    // count args and their sizes
    size_t sz_req = 0;

    int argc = 0;
    for (const char **arg = __embcust_argv; *arg; ++argc, ++arg)
        sz_req += sizeof(*arg) + strlen(*arg) + 1;
    sz_req += sizeof(char *);

    int envc = 0;
    for (const char **env = __embcust_environ; *env; ++envc, ++env)
        sz_req += sizeof(*env) + strlen(*env) + 1;
    sz_req += sizeof(char *);

    int auxc = 0;
    for (const auxv_t *aux = __embcust_auxv; aux->type; ++auxc, ++aux)
        sz_req += sizeof(*aux);
    sz_req += sizeof(auxv_t);

    if (sz_req > sizeof(__embcust_init_stack))
        a_crash();

    // first come the pointers and aux vectors
    size_t ptr_and_auxv_sz = (argc + 1 + envc + 1) * sizeof(char *) + (auxc + 1) + sizeof(auxv_t);
    // then come the copied strings
    char *strpool = __embcust_init_stack + ptr_and_auxv_sz;

    // copy args to strpool and set the arg points into the strpool
    char **argv = (char **)__embcust_init_stack;
    for (int i = 0; i < argc; ++i) {
        size_t len = strlen(__embcust_argv[i]);
        memcpy(strpool, __embcust_argv[i], len + 1);
        argv[i] = strpool;
        strpool += len + 1;
    }
    argv[argc] = NULL;

    char **envp = &argv[argc + 1];
    for (int i = 0; i < envc; ++i) {
        size_t len = strlen(__embcust_environ[i]);
        memcpy(strpool, __embcust_environ[i], len + 1);
        envp[i] = strpool;
        strpool += len + 1;
    }
    envp[envc] = NULL;

    auxv_t *auxp = (auxv_t *)&envp[envc + 1];
    for (int i = 0; i < auxc; ++i) {
        auxp[i] = __embcust_auxv[i];
    }
    auxp[auxc].type = 0;
    auxp[auxc].val = 0;

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
