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

u8 Func_080c2454(u32 arg0) {
    register u32 index asm("r2");
    register u8 *base asm("r3");

    if (arg0 > 0xABU) {
        return 0U;
    }
    index = arg0 << 3;
    base = (u8 *) 0x080C7420;
    asm volatile("" : "+r" (index), "+r" (base));
    index += 4;
    return base[index];
}
