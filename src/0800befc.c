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

s32 Func_080030f8(s32);
s32 Func_0800be70(s32, u32);

void Func_0800befc(s32 arg0) {
    u32 var_r6;

    var_r6 = 0;
    do {
        Func_0800be70(arg0, var_r6);
        Func_0800be70(arg0, var_r6 + 1);
        Func_0800be70(arg0, var_r6 + 2);
        Func_0800be70(arg0, var_r6 + 3);
        var_r6 += 4;
        Func_080030f8(1);
    } while (var_r6 <= 0x7FU);
}
