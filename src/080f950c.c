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

s32 Func_080fb2cc(s32, s32, u16);
extern s16 Data_02003034;
extern s16 Data_02003008;

void Func_080f950c(s16 arg0) {
    register s32 value asm("r2");
    register s32 saved asm("r5");
    register s32 base asm("r0");
    register s32 kind asm("r1");

    value = arg0;
    value <<= 16;
    asm volatile("asr %0, %1, #16"
                 : "=r" (saved)
                 : "r" (value));
    base = 0x02004290;
    asm volatile("" : "+r" (base) : "r" (value), "r" (saved));
    value = (u32)value >> 16;
    kind = 0xFF;
    Func_080fb2cc(base, kind, value);
    Data_02003034 = saved;
    Data_02003008 = saved;
}
