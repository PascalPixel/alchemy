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

s32 Func_080ab21c(void);

void Func_080ab2ec(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    register s32 first asm("r0");
    register s32 second asm("r1");
    register s32 third asm("r2");
    register s32 fourth asm("r3");
    register void *base asm("r4");

    base = arg0;
    first = M2C_FIELD(base, u16 *, 0xC) + arg1;
    second = M2C_FIELD(base, u16 *, 0xE);
    third = arg2;
    fourth = arg3;
    asm volatile(
        "sub sp, #4\n\t"
        "ldr r5, [sp, #20]\n\t"
        "mov r6, r3\n\t"
        "add r1, r1, r2\n\t"
        "ldr r3, [sp, #16]\n\t"
        "add r0, #1\n\t"
        "add r1, #1\n\t"
        "mov r2, r6\n\t"
        "str r5, [sp]"
        : "+r" (first), "+r" (second), "+r" (third),
          "+r" (fourth), "+r" (base)
        :
        : "r5", "r6", "memory");
    Func_080ab21c();
    asm volatile("add sp, #4" : : : "memory");
}
