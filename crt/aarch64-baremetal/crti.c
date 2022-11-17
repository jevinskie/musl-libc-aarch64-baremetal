#ifndef __APPLE__
#define INIT_SECT ".init"
#define FINI_SECT ".fini"
#else
#define INIT_SECT "__TEXT,__embcust_init"
#define FINI_SECT "__TEXT,__embcust_fini"
#endif

__attribute__((naked,section(INIT_SECT)))
void _init(void) {
    __asm__ volatile(
        "stp x29,x30,[sp,-16]!\n\t"
        "mov x29,sp\n\t"
    );
}

__attribute__((naked,section(FINI_SECT)))
void _fini(void) {
    __asm__ volatile(
        "stp x29,x30,[sp,-16]!\n\t"
        "mov x29,sp\n\t"
    );
}
