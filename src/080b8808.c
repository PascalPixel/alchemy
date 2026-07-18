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

s32 Func_080b8808(u32 arg0) {
    if (arg0 <= 7) {
        return 0;
    }
    if (arg0 - 128 <= 5) {
        return 0;
    }
    return -1;
}
