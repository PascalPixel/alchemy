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

s32 Func_080a1bdc(s32, s32, s32);

void Func_080a1bcc(void) {
    register s32 arg0 asm("r0");
    register s32 arg1 asm("r1");
    register s32 arg2 asm("r2");

    arg0 = 0x6C;
    asm volatile("" : "+r" (arg0));
    arg1 = 0x28;
    asm volatile("" : "+r" (arg1) : "r" (arg0));
    arg2 = 8;
    asm volatile("" : "+r" (arg2) : "r" (arg1));
    Func_080a1bdc(arg0, arg1, arg2);
}
