static inline uintptr_t __get_tp()
{
    return (uintptr_t)&__builtin_tls[0].pt;
}
