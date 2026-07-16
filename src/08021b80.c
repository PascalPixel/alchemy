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

s32 Func_0801a4fc(u32, s32, s32 *, s32 *, s32, s32);
s32 Func_080770c0(s32);

s32 Func_08021b80(u32 arg0, s32 arg1) {
    s32 sp8;
    s32 spC;
    u32 var_r5;

    var_r5 = arg0;
    spC = arg1;
    if (var_r5 > 7U) {
        var_r5 = 0;
    }
    if (Func_080770c0(0x20) != 0) {
        switch (var_r5) {
        case 0:
            var_r5 = 0x38;
            break;
        case 1:
            var_r5 = 0x39;
            break;
        }
    }
    Func_0801a4fc(var_r5, 0, &spC, &sp8, 0xE, 1);
    return sp8;
}
