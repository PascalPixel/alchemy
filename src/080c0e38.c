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

s32 Func_080030f8(s32);

void Func_080c0e38(void) {
    register s32 one asm("r0");
    s32 var_r5;

    *(s16 *)0x04000050 = 0x2044;
    var_r5 = 1;
    do {
        *(s16 *)0x04000052 = 0x1010 - var_r5;
        one = 1;
        asm volatile("" : "+r" (one));
        var_r5 += 2;
        Func_080030f8(one);
    } while (var_r5 <= 0x10);
}
