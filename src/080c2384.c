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

u32 Func_080c2384(u32 arg0) {
    register u16 *base asm("r3");
    register u32 index asm("r2");
    register u32 result asm("r0");

    if (arg0 > 0xABU) {
        base = (u16 *)0x080C7420;
        asm volatile("ldrh r0, [r3]"
                     : "=r" (result) : "r" (base) : "memory");
        return result;
    }
    base = (u16 *)0x080C7420;
    index = arg0 << 3;
    asm volatile("ldrh r0, [r3, r2]"
                 : "=r" (result) : "r" (base), "r" (index) : "memory");
    return result;
}
