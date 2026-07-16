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

u32 Func_080c2434(u32 arg0) {
    register u8 *base asm("r3");
    register u32 offset asm("r2");
    register u32 result asm("r0");

    if (arg0 > 0xABU) {
        return 0U;
    }
    base = (u8 *) 0x080C7420;
    offset = arg0 << 3;
    asm volatile("" : "+r" (offset) : "r" (base));
    offset += (u32) base;
    result = ((u8 *) offset)[3];
    result <<= 0x1F;
    result >>= 0x1F;
    return result;
}
