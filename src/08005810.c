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

s32 Func_08004458(void);
s32 Func_08002304(s32, s32);
s32 Func_08005b24(s32);

u32 Func_08005810(s32 arg0) {
    u32 sp0[16];
    s32 var_r6;
    u32 var_r5;
    s32 temp_r0;
    u8 *var_r2;
    u8 temp_r3;

    var_r2 = *(u8 **)0x03001F1C;
    var_r6 = 0;
    var_r5 = 0;
    do {
        temp_r3 = *var_r2++;
        if (temp_r3 == 0) {
            sp0[var_r6++] = var_r5;
        }
        var_r5 += 1;
    } while (var_r5 <= 0xFU);
    var_r5 = 0x10;
    if (var_r6 != 0) {
        if (var_r6 == 1) {
            var_r5 = sp0[0];
            if (Func_08005b24(arg0) == 0x10) {
                var_r5 = 0x10;
            }
        } else {
            var_r5 = Func_08002304(Func_08004458(), var_r6);
            var_r5 = sp0[var_r5];
        }
    }
    return var_r5;
}
