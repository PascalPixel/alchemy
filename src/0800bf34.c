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

void Func_0800be70(s32, s32);
void Func_080030f8(u32);

void Func_0800bf34(s32 *arg0, s32 arg1) {
    s32 *sp0;
    s32 *var_r5;
    s32 var_r6;
    u32 var_r7;
    u32 var_r9;
    u32 var_sl;
    u32 var_r8;

    sp0 = arg0;
    var_r7 = 0;
    do {
        if (arg1 > 0) {
            var_r9 = var_r7 + 1;
            var_sl = var_r7 + 2;
            var_r5 = sp0;
            var_r8 = var_r7 + 3;
            var_r6 = arg1;
            do {
                Func_0800be70(*var_r5, var_r7);
                Func_0800be70(*var_r5, var_r9);
                Func_0800be70(*var_r5, var_sl);
                var_r6 -= 1;
                Func_0800be70(*var_r5++, var_r8);
            } while (var_r6 != 0);
        }
        Func_080030f8(1U);
        var_r7 += 4;
    } while (var_r7 <= 0x7FU);
}
