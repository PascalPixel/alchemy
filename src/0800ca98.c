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

s32 Func_0800ca98(void *arg0) {
    s32 var_r2;
    s32 var_r3;

    if (M2C_FIELD(arg0, u8 *, 0x55) == 0) {
        var_r3 = M2C_FIELD(arg0, s32 *, 0x38);
        if (var_r3 == 0x80000000) {
            var_r2 = M2C_FIELD(arg0, s32 *, 0x3C);
            goto block_4;
        }
        goto block_6;
    }
    var_r2 = M2C_FIELD(arg0, s32 *, 0x38);
    var_r3 = 0x80000000;
block_4:
    if ((var_r2 != var_r3) || (M2C_FIELD(arg0, s32 *, 0x40) != var_r2)) {
block_6:
        return 0;
    }
    return 1;
}
