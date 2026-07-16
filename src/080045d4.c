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

s32 Func_080072f0(s32, s32, s32, s32);

s32 Func_080045d4(s32 arg0, s32 arg1, s32 arg2) {
    return Func_080072f0(arg0, arg1, arg2, 0x030001D8) << 8;
}
