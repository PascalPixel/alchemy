typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0801e71c(s32 arg0) {
    asm volatile(
        "ldr r3, .Lbase_0801e71c\n\t"
        "ldr r2, .Lmask_0801e71c\n\t"
        "ldr r3, [r3]\n\t"
        "and r0, r2\n\t"
        "ldr r2, .Loffset_0801e71c\n\t"
        "add r3, r3, r2\n\t"
        "strh r0, [r3]"
        : "+r" (arg0) : : "r2", "r3", "memory");
}

asm(
    ".align 2\n"
    ".Lmask_0801e71c:\n\t.word 0x0000000f\n"
    ".Lbase_0801e71c:\n\t.word 0x03001e8c\n"
    ".Loffset_0801e71c:\n\t.word 0x00000eae\n"
    ".size Func_0801e71c, .-Func_0801e71c");
