static inline uintptr_t __get_tp()
{
    uintptr_t tp = &__builtin_tls[0].pt;
    uintptr_t tp_adj = TP_ADJ(tp);
    __asm__ __volatile__("nop\n\t");
    return (uintptr_t)tp_adj;
}
