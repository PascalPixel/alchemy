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
extern s32 Func_08004080();
extern s32 Func_08003fa4();
extern s32 Func_080150c8();
s32 Func_080a172c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;
    s32 var_r6;
    var_r6 = 0;
    temp_r0 = Func_08004080();
    if (temp_r0 != 0) {
        Func_08003fa4(temp_r0, 0x80, 0x080AEA4C);
        var_r6 = Func_080150c8(temp_r0, 0x40000000, arg0, arg2, arg3);
    }
    return var_r6;
}
