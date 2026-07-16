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

s32 Func_080fb334(s32, s32, s16);

void Func_080f94e0(s16 arg0) {
    register s32 value asm("r2");
    register s32 base asm("r0");
    register s32 kind asm("r1");

    value = arg0;
    value <<= 16;
    base = 0x02004290;
    asm volatile("" : "+r" (base) : "r" (value));
    value >>= 16;
    kind = 0xFF;
    Func_080fb334(base, kind, value);
}
