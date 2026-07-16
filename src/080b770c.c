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

s32 Func_080b770c(s16 *arg0, s32 arg1) {
    s16 *var_r0;
    s32 var_r1;
    u16 temp_r3;
    u32 var_r2;

    var_r0 = arg0;
    var_r1 = arg1;
    if (var_r1 > 7) {
        var_r1 += 0x78;
    }
    var_r2 = 0;
loop_3:
    temp_r3 = (u16) *var_r0;
    var_r0 += 1;
    if (temp_r3 != 0xFF) {
        if (temp_r3 == var_r1) {
            return 1;
        }
        var_r2 += 1;
        if (var_r2 > 0xDU) {
            goto block_7;
        }
        goto loop_3;
    }
block_7:
    return 0;
}
