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

s32 Func_08015420(s32, s32);
s32 Func_08077008();
s32 Func_08077150(s32);

void Func_080917d0(s32 arg0, s32 arg1) {
    Func_08077008();
    Func_08077150(arg0);
    if (arg1 != 0) {
        Func_08015420(arg0, arg1);
    }
}
