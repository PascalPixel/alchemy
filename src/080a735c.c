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

void *Func_08077080(u32);

s32 Func_080a735c(s32 arg0) {
    register s32 var_r0 asm("r0");

    var_r0 = (s32)Func_08077080((u32) (arg0 << 0x12) >> 0x12);
    asm volatile(
        "ldrb r3, [r0, #12]\n\t"
        "cmp r3, #0\n\t"
        "bne 1f\n\t"
        "ldrb r2, [r0, #1]\n\t"
        "mov r3, #192\n\t"
        "and r3, r3, r2\n\t"
        "mov r0, #1\n\t"
        "cmp r3, #192\n\t"
        "bne 2f\n\t"
        "1:\n\t"
        "mov r0, #0\n\t"
        "2:"
        : "+r" (var_r0)
        :
        : "r2", "r3", "cc");
    return var_r0;
}
