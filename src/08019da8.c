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

extern s32 Func_08019d2c();
extern s32 Func_080162d4();
extern s32 Func_0801ec6c();

s32 Func_08019da8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_r5;
    s32 var_r9;
    s32 var_r8;
    void *temp_r5;

    temp_r5 = *(void **)0x03001E8C;
    if (Func_08019d2c(arg0) == -1) {
        return 0;
    }
    var_r9 = -4;
    var_r8 = var_r9;
    if (M2C_FIELD(temp_r5, u8, 0xEA4) != 0) {
        var_r5 = Func_080162d4(arg2, arg3, 6, 5, 2);
        var_r8 = 0;
    } else {
        var_r5 = Func_080162d4(arg2, arg3, 5, 5, 2);
    }
    if (var_r5 != 0) {
        Func_0801ec6c(arg0, arg1, -1, var_r5, var_r8, var_r9);
    }
    return var_r5;
}
