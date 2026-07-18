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

s32 Func_080022ec(s32, s32);

s16 Func_080f40d0(s16 arg0, s16 arg1) {
    return Func_080022ec(arg0 << 8, arg1);
}
