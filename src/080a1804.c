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

s32 Func_080150e0(s32, s32, s32);

void Func_080a1804(s32 arg0, s32 arg1) {
    register s32 first asm("r0");
    register s32 third asm("r2");
    first = 0;
    asm volatile("" : "+r" (first));
    third = 0;
    asm volatile("" : "+r" (third) : "r" (first));
    Func_080150e0(first, arg1, third);
}
