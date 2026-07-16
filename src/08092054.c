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

s32 Func_0808ba1c();

s32 Func_08092054(void) {
    s32 var_r0;

    var_r0 = Func_0808ba1c();
    asm volatile(
        "cmp %0, #0\n"
        "bne 1f\n"
        "mov %0, #0\n"
        "1:"
        : "+r" (var_r0));
    return var_r0;
}
