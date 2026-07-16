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

s32 Func_08015080(void);
s32 Func_080150a8(void);

void Func_080a23c0(s32 arg0) {
    register s32 *base asm("r3");
    register s32 first asm("r0");
    register s32 saved asm("r5");

    base = (s32 *)0x02000240;
    asm volatile("sub sp, #4\n\tmov r5, r0"
                 : "+r" (base), "=r" (saved)
                 : "r" (arg0) : "memory");
    first = base[4];
    asm volatile(
        "mov r3, #0\n\t"
        "str r3, [sp]\n\t"
        "mov r2, r5\n\t"
        "mov r1, #7\n\t"
        "mov r3, #8"
        : "+r" (first), "+r" (saved)
        : : "r1", "r2", "r3", "memory");
    Func_080150a8();
    first = 0xB0B;
    asm volatile(
        "mov r1, r5\n\t"
        "mov r2, #64\n\t"
        "mov r3, #0"
        : "+r" (first), "+r" (saved)
        : : "r1", "r2", "r3");
    Func_08015080();
    asm volatile("add sp, #4" : : : "memory");
}
