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

s32 Func_08078b9c(s32 arg0) {
    u32 var_r0;

    var_r0 = arg0 & 0x3FFF;
    if (var_r0 >= 0x208U) {
        var_r0 = 0;
    }
    return (var_r0 * 0x10) + 0x0807EE58;
}
