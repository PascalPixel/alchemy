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

s32 Func_080770c8(s32);
s32 Func_080771a8(s32, s32, s32);

void Func_080294d4(s32 arg0, s32 arg1, s32 arg2) {
    Func_080770c8((arg1 * 0x14) + arg2 + 0x30);
    Func_080771a8(arg0, arg1, arg2);
}
