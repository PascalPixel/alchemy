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

s32 Func_080a4110(s32 arg0, s32 arg1) {
    register s32 *base asm("r3");
    register s32 index asm("r2");
    if ((arg0 > 2) || (arg1 > 2) || (arg0 < 0) || (arg1 < 0)) {
        return 0;
    }
    index = (arg1 * 3) + arg0;
    base = (s32 *)0x080AF2E4;
    asm volatile("" : "+r" (index), "+r" (base));
    index <<= 2;
    asm volatile("ldr r0, [r3, r2]" : "=r" (arg0)
                 : "r" (index), "r" (base));
    return arg0;
}
