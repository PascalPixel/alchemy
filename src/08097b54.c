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

u16 Func_08097b54(void) {
    register u32 value asm("r3");
    register u32 mask asm("r2");
    register u16 *table asm("r1");

    value = *(u32 *) 0x03001AE8;
    mask = 0xF;
    value >>= 4;
    table = (u16 *) 0x0809F0F8;
    asm volatile("" : "+r" (value), "+r" (mask), "+r" (table));
    value &= mask;
    value <<= 1;
    return *(u16 *) ((u8 *) table + value);
}
