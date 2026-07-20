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

s32 Func_080056cc();
s32 Func_08005c68();
void Func_08005cf8();

s32 Func_0801f730(s32 arg0) {
    s32 t;
    s32 var_r1;
    s32 var_r5;
    volatile u8 *var_r2;

    if (Func_080056cc() != 0) {
        var_r5 = -9;
    } else {
        var_r5 = Func_08005c68();
        if (arg0 != 0) {
            var_r2 = (volatile u8 *)(*(s32 *)0x03001F1C + 0x1071);
            var_r1 = 2;
            do {
                t = *var_r2 << 0x18;
                var_r2 += 0x40;
                if (t != 0) {
                    var_r5 -= 1;
                }
                var_r1 -= 1;
            } while (var_r1 >= 0);
        }
    }
    Func_08005cf8();
    return var_r5;
}
