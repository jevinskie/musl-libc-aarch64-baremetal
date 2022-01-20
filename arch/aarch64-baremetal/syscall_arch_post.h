
typedef long (*__syscall1_t)(long a);
typedef long (*__syscall2_t)(long a, long b);
typedef long (*__syscall3_t)(long a, long b, long c);
typedef long (*__syscall4_t)(long a, long b, long c, long d);
typedef long (*__syscall5_t)(long a, long b, long c, long d, long e);
typedef long (*__syscall6_t)(long a, long b, long c, long d, long e, long f);
typedef long (*__syscall7_t)(long a, long b, long c, long d, long e, long f, long g);

#define __syscall1(fn, a) (((__syscall1_t)fn)(__scc(a)))
#define __syscall2(fn, a, b) (((__syscall2_t)fn)(__scc(a), __scc(b)))
#define __syscall3(fn, a, b, c) (((__syscall3_t)fn)(__scc(a), __scc(b), __scc(c)))
#define __syscall4(fn, a, b, c, d) (((__syscall4_t)fn)(__scc(a), __scc(b), __scc(c), __scc(d)))
#define __syscall5(fn, a, b, c, d, e) (((__syscall5_t)fn)(__scc(a), __scc(b), __scc(c), __scc(d), __scc(e)))
#define __syscall6(fn, a, b, c, d, e, f) (((__syscall6_t)fn)(__scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f)))
#define __syscall7(fn, a, b, c, d, e, f, g) (((__syscall6_t)fn)(__scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f), __scc(g)))
