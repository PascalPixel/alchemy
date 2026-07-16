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

s32 Func_08002dd8(s32);
s32 Func_0800307c(s32, s32, s32);

s32 Func_08005cf8(void) {
    register s32 arg0 asm("r0");
    register s32 arg1 asm("r1");
    register s32 arg2 asm("r2");

    arg0 = 5;
    asm volatile("" : "+r" (arg0));
    arg1 = 0;
    asm volatile("" : "+r" (arg1) : "r" (arg0));
    arg2 = 0;
    asm volatile("" : "+r" (arg2) : "r" (arg1));
    Func_0800307c(arg0, arg1, arg2);
    return Func_08002dd8(0x33);
}
