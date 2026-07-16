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

s32 Func_080b8808(u32 arg0) {
    if (arg0 <= 7U) {
        return 0;
    }
    asm volatile(
        "mov r3, %0\n"
        "sub r3, #128\n"
        "mov %0, #0\n"
        "cmp r3, #5\n"
        "bls 1f\n"
        "mov %0, #1\n"
        "neg %0, %0\n"
        "1:"
        : "+r" (arg0)
        :
        : "r3", "cc");
    return (s32) arg0;
}
