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

extern s32 Func_0801f77c();
extern void Func_080284dc();
extern void Func_080287a8();
extern void Func_08028808();
extern s32 Func_08028574();
extern void Func_0802851c();
extern s8 Data_0803740f[];

s32 Func_080289e8(void) {
    s32 temp_r0;
    s32 var_r5;
    s32 var_r5_2;
    u32 var_r6;

    var_r6 = 0;
    var_r5_2 = 0;
    temp_r0 = Func_0801f77c();
    if (temp_r0 < 0) {
        return -1;
    }
    if (temp_r0 == 0) {
        return 0;
    }
    if (temp_r0 == 3) {
        var_r6 = 1;
    } else if (temp_r0 == 0x67) {
        var_r6 = 2;
    } else if (temp_r0 > 0x64) {
        var_r6 = 3;
    } else {
        var_r5_2 = 1;
    }
    Func_080284dc();
    if ((var_r6 == 0) || (var_r6 == 3)) {
        Func_080287a8(0x15);
    }
    if (var_r6 <= 1U) {
        Func_080287a8(0x16);
    }
    if ((var_r6 == 0) || (var_r6 == 3)) {
        Func_080287a8(0x17);
    }
    Func_080287a8(0x18);
    if ((*(s16 *)0x0200200C) != 0) {
        Func_080287a8(0x1D);
    }
    if ((*(s16 *)0x02002010) != 0) {
        Func_080287a8(0x1E);
    }
    Func_08028808(0x11, 7, 0);
    var_r5 = Func_08028574(var_r5_2);
    Func_0802851c();
    if (var_r5 >= 0) {
        var_r5 = Data_0803740f[var_r5 + (var_r6 * 6)];
    }
    return var_r5;
}
