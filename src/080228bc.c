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

s32 Func_08004620(s32);

s32 Func_080228bc(s16 *arg0, s32 arg1) {
    s16 *var_r5;
    s32 var_r2;
    u8 *var_r1;

    var_r5 = arg0;
    Func_08004620(arg1);
    var_r1 = (u8 *)0x03001F70;
    var_r2 = 0xD;
    do {
        var_r2 -= 1;
        *var_r5 = (s16) *var_r1;
        var_r1 += 1;
        var_r5 += 1;
    } while (var_r2 >= 0);
}
