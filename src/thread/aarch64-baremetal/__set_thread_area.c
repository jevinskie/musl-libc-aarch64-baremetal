#include "pthread_impl.h"

int __set_thread_area(void *p)
{
    if (p != __builtin_tls) {
        return -ESRCH;
    }
    return 0;
}
