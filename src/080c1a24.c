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

s32 Func_0800307c(s32, s32, s32);

void Func_080c1a24(void) {
    register s32 first asm("r0") = 2;
    register s32 second asm("r1");
    register s32 third asm("r2");

    asm volatile("" : "+r" (first));
    second = 0;
    asm volatile("" : "+r" (second) : "r" (first));
    third = 0;
    asm volatile("" : "+r" (third) : "r" (second));
    Func_0800307c(first, second, third);
}
