#ifndef __APPLE__
__asm__(
".text \n"
".global " START "\n"
".type " START ",%function\n"
START ":\n"
"	mov x29, #0\n"
"	mov x30, #0\n"
"	mov x0, sp\n"
".weak _DYNAMIC\n"
".hidden _DYNAMIC\n"
"	adrp x1, _DYNAMIC\n"
"	add x1, x1, #:lo12:_DYNAMIC\n"
"	and sp, x0, #-16\n"
"	b " START "_c\n"
);
#else
__asm__(
"\t.section\t__TEXT,__text,regular,pure_instructions\n\t"
"\t.global _" START "\n"
"_" START ":\n\t"
"mov x29, #0\n\t"
"mov x30, #0\n\t"
"mov x0, sp\n\t"
"and sp, x0, #-16\n\t"
"b _" START "_c\n\t"
);
#endif