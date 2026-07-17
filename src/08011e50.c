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
s32 Func_08011e50(u8 *arg0, u32 arg1) {
    s32 value;
    if (arg1 <= 7)
        value = (s8)arg0[0];
    else
        value = (s8)arg0[1];
    return value << 19;
}
