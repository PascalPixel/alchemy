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

s32 Func_080915dc();

s32 Func_0809ae3c(s32 arg0) {
    if (Func_080915dc() == 0xFF) {
        return -1;
    }
    return arg0;
}
