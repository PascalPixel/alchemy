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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080150d0(s32, s32, s32, s32, s32);

s32 Func_080a9cf8(s32 arg0) {
    s32 *var_r6;
    s32 temp_r0;
    s32 var_r5;
    s32 base;
    s32 arg5;

    base = *(s32 *)0x03001F2C;
    var_r5 = 0;
    arg5 = 0xA8;
    var_r6 = base + 0xC8;
    do {
        temp_r0 = Func_080150d0(2, var_r5, arg0, 0xF8, arg5);
        var_r5 += 1;
        *var_r6 = temp_r0;
        var_r6 += 1;
    } while (var_r5 <= 7);
    return 1;
}
