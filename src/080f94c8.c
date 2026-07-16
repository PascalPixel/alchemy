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

s32 Func_080fb2a4(s32, s32);
extern u8 Data_02004290[];

void Func_080f94c8(u16 arg0) {
    register u32 value asm("r1") = arg0;
    register u8 *base asm("r0");

    value <<= 16;
    asm volatile("" : "+r" (value));
    base = Data_02004290;
    asm volatile("" : "+r" (base) : "r" (value));
    value >>= 16;
    Func_080fb2a4((s32)base, value);
}
