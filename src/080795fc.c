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

s32 Func_08079338(s32);

s32 Func_080795fc(void) {
    s32 var_r5;
    s32 var_r6;

    var_r6 = 0;
    var_r5 = 0;
    do {
        if (Func_08079338(var_r5) != 0) {
            var_r6 += 1;
        }
        var_r5 += 1;
    } while (var_r5 <= 7);
    return var_r6;
}
