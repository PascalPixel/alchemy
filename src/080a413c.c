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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080a413c(s32 arg0, s32 arg1) {
    s32 var_r0;

    var_r0 = 0x1E;
    if (arg1 != 0) {
        var_r0 = 0x26;
    }
    return var_r0;
}
