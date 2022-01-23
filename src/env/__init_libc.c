#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "syscall.h"
#include "atomic.h"
#include "libc.h"

static void dummy1(void *p) {}
weak_alias(dummy1, __init_ssp);

#define AUX_CNT 38

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
void __init_libc(char **envp, char *pn)
{
    size_t i, *auxv, aux[AUX_CNT] = { 0 };
    __environ = envp;
    for (i=0; envp[i]; i++);
    libc.auxv = auxv = (void *)(envp+i+1);
    for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
    __hwcap = aux[AT_HWCAP];
    if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
    libc.page_size = aux[AT_PAGESZ];

    if (!pn) pn = (void*)aux[AT_EXECFN];
    if (!pn) pn = "";
    __progname = __progname_full = pn;
    for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

    __init_tls(aux);
    __init_ssp((void *)aux[AT_RANDOM]);

    if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
        && !aux[AT_SECURE]) return;

    struct pollfd pfd[3] = { {.fd=0}, {.fd=1}, {.fd=2} };
    int r =
#ifdef SYS_poll
    __syscall(SYS_poll, pfd, 3, 0);
#else
    __syscall(SYS_ppoll, pfd, 3, &(struct timespec){0}, 0, _NSIG/8);
#endif
    if (r<0) a_crash();
    for (i=0; i<3; i++) if (pfd[i].revents&POLLNVAL)
        if (__sys_open("/dev/null", O_RDWR)<0)
            a_crash();
    libc.secure = 1;
}
