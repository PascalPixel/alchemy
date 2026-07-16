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

s32 Func_080162d4(s32, s32, s32, s32, s32);
s32 Func_0801e940(s32, s32, s32, s32);

s32 Func_08021c34(void) {
    register s32 first asm("r0");
    register s32 second asm("r1");
    register s32 third asm("r2");
    register s32 fourth asm("r3");
    s32 temp_r0;

    temp_r0 = Func_080162d4(0, 0, 6, 4, 6);
    second = temp_r0;
    asm volatile("" : "+r" (second));
    first = 0x08037300;
    asm volatile("" : "+r" (first) : "r" (second));
    third = 0;
    asm volatile("" : "+r" (third) : "r" (first));
    fourth = 0;
    asm volatile("" : "+r" (fourth) : "r" (third));
    Func_0801e940(first, second, third, fourth);
    return temp_r0;
}
