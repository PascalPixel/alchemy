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

s32 Func_08004620();
s32 Func_080046c4(s32);

void Func_0800473c(s32 arg0, s32 arg1) {
    s32 var_r5;

    var_r5 = arg1;
    if ((u32) (var_r5 - 1) > 9U) {
        var_r5 = 0xA;
    }
    Func_08004620();
    Func_080046c4(0x03001F7A - var_r5);
}
