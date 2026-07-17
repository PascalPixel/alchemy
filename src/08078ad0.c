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
extern u8 Data_0807b490[];

extern s32 Func_08078aa0(u32 arg0);

s32 Func_08078ad0(u32 arg0) {
    u8 value = Data_0807b490[arg0 & 0x1ff];
    register s32 result asm("r4") = 0;
    if (value != 0)
        result = Func_08078aa0(value - 1);
    return result;
}
