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

s32 Func_080773d8(s32 arg0) {
    u32 var_r0;

    var_r0 = arg0 - 8;
    if (var_r0 > 0xF9U) {
        var_r0 = 0;
    }
    return (var_r0 * 0x54) + 0x08080EC8;
}
