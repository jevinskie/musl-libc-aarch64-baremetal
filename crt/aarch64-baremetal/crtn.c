__attribute__((naked,section(".init")))
void __crtn_init(void) {
    __asm__ volatile(
        "ldp x29,x30,[sp],#16\n\t"
        "ret\n\t"
    );
}

__attribute__((naked,section(".fini")))
void __crtn_fini(void) {
    __asm__ volatile(
        "ldp x29,x30,[sp],#16\n\t"
        "ret\n\t"
    );
}
