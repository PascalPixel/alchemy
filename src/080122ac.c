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

s32 Func_08012204(s32);

s32 Func_080122ac(s32 arg0, s32 arg1) {
    if ((u32)(Func_08012204(arg1) - 5) <= 7U) {
        return 0;
    }
    return -1;
}
