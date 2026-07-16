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

s16 Func_080b2764(s32 arg0) {
    register s16 *base asm("r2");
    register s32 index asm("r3");
    index = arg0 << 5;
    index += arg0;
    base = (s16 *)0x080B41AC;
    asm volatile("" : "+r" (base), "+r" (index));
    index <<= 1;
    index += 0x40;
    return *(s16 *)((u8 *)base + index);
}
