static inline uintptr_t __get_tp()
{
    return (uintptr_t)TP_ADJ(&__builtin_tls[0].pt);
}
