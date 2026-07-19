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

extern s32 Func_080770c0(s32);
extern void *Func_08077008(s32);

s32 Func_080b6ae0(s16 *arg0) {
    s16 *var_r6;
    s32 temp_r7;
    s32 var_r0;
    s32 var_r5;
    s32 var_r7;
    s32 var_r8;

    var_r6 = arg0;
    var_r8 = 0;
    var_r7 = 6;
    var_r0 = 0;
    if (var_r6 != NULL) {
        if (Func_080770c0(0x16C) != 0) {
            var_r7 = 3;
        }
        var_r5 = 0x80;
        var_r7 += 0x80;
        for (; var_r5 < var_r7; var_r5 += 1) {
            if (M2C_FIELD(Func_08077008(var_r5), u8 *, 0x12A) != 0) {
                *var_r6 = (s16) var_r5;
                var_r8 += 1;
                var_r6 += 1;
            }
        }
        *var_r6 = 0xFF;
        var_r0 = var_r8;
    }
    return var_r0;
}
