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

s32 Func_0801a910(s32 arg0) {
    s32 _v1d22;
    s32 _v1t91;
    s32 _v1d14;
    s32 temp_r4;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    u32 var_r2;
    u16 *var_r0_2;
    u16 *var_r2_2;
    u32 temp_r3;

    (s32) _v1t91 = (*(s32 *)0x03001E98); temp_r4 = _v1t91;
    if (arg0 != 0) {
        (s32) var_r1 = 0;
        var_r2_2 = temp_r4 + 0x1DE;
        var_r0 = 0;
loop_2:
        if (*var_r2_2 == 0) {
            return temp_r4 + var_r0 + 0x1D4;
        }
        _v1d22 = 0; var_r1 = var_r1 + 1;
        (s32) var_r2_2 += 0x34;
        var_r0 = var_r0 + 0x34;
        if (var_r1 == 5) {
            goto block_10;
        }
        goto loop_2;
    }
    var_r1_2 = 0;
    var_r2 = temp_r4 + 0x68;
    var_r0_2 = temp_r4 + 0x72;
loop_7:
    temp_r3 = *var_r0_2;
    (s32) var_r0_2 += 0x34;
    if (temp_r3 == 0) {
        return var_r2;
    }
    _v1d14 = 0; var_r2 += 0x34;
    var_r1_2 += 1;
    if (var_r1_2 == 7) {
block_10:
        return 0;
    }
    goto loop_7;
}
