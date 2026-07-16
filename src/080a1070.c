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

s32 Func_0800430c();
s32 Func_08009290();
s32 Func_080770c8(s32);

s32 Func_080a1070(void) {
    Func_080770c8(0x152);
    Func_080770c8(0x166);
    Func_08009290();
    return Func_0800430c();
}
