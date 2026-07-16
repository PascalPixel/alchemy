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

s32 Func_08011d34(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r4;
    s32 var_r0;
    s32 var_r3;
    u32 temp_r1;

    temp_r4 = M2C_FIELD(arg0, s8 *, 0) << 0x13;
    var_r0 = M2C_FIELD(arg0, s8 *, 1) << 0x13;
    var_r3 = temp_r4;
    if (var_r0 > temp_r4) {
        var_r3 = var_r0;
    }
    temp_r1 = arg1 + arg2;
    if (temp_r1 == 0xF) {
        return var_r3;
    }
    if (temp_r1 <= 0xEU) {
        var_r0 = temp_r4;
    }
    return var_r0;
}
