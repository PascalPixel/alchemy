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

s32 Func_08003fa4(s32, u16, s32);
extern u16 Data_03001b10[];

s32 Func_080040d0(s32 arg0, s32 arg1) {
    register u16 *base asm("r1");
    register s32 third asm("r2");
    register s32 offset asm("r3");
    third = arg1;
    asm volatile("" : "+r" (third));
    base = Data_03001b10;
    asm volatile("" : "+r" (base) : "r" (third));
    offset = arg0 * 4;
    asm volatile("" : "+r" (offset) : "r" (base), "r" (third));
    return Func_08003fa4(
        arg0, *(u16 *)((u8 *)base + offset), third);
}
