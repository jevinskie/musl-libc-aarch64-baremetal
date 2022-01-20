__attribute__((naked,section(".init")))
void _init(void) {
    __asm__ volatile(
        "stp x29,x30,[sp,-16]!\n\t"
        "mov x29,sp\n\t"
    );
}

__attribute__((naked,section(".fini")))
void _fini(void) {
    __asm__ volatile(
        "stp x29,x30,[sp,-16]!\n\t"
        "mov x29,sp\n\t"
    );
}
