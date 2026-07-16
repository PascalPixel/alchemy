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

s32 Func_080043e0();
s32 Func_08009298();
s32 Func_080770d0(s32);

s32 Func_080a1050(void) {
    Func_080043e0();
    Func_08009298();
    Func_080770d0(0x166);
    return Func_080770d0(0x152);
}
