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

s32 Func_080c0700(s32, s32);

void Func_080c1a14(void) {
    register s32 arg0 asm("r0") = 0;
    register s32 arg1 asm("r1");

    asm volatile("" : "+r" (arg0));
    arg1 = 0;
    asm volatile("" : "+r" (arg1) : "r" (arg0));
    Func_080c0700(arg0, arg1);
}
