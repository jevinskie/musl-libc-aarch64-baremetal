#include "pthread_impl.h"

int __set_thread_area(void *p)
{
    if (p != TP_ADJ(&__builtin_tls[0].pt))
        return -ESRCH;
    return 0;
}
