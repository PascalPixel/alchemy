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

s32 Func_080064f4(void) {
    s32 var_r0;

    var_r0 = 0;
    if (*(s32 *)0x02002080 != 0) {
        var_r0 = 1;
    }
    if (*(s32 *)0x020023AC != 0) {
        var_r0 |= 2;
    }
    return var_r0;
}
