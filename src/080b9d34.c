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

void *Func_08077008(u8);

s32 Func_080b9d34(void *arg0, void *arg1) {
    s32 sp0;
    void *sp4;
    s16 *var_r7;
    s32 temp_r2;
    s32 var_r4;
    s32 var_r8;
    u8 *var_r5;
    void *var_r1;

    var_r1 = arg1;
    var_r4 = 0;
    M2C_FIELD(var_r1, s32 *, 0x1C) = 0;
    temp_r2 = M2C_FIELD(arg0, s32 *, 0x58);
    M2C_FIELD(var_r1, s32 *, 0) = (s32) (0xFFF & temp_r2);
    M2C_FIELD(var_r1, u32 *, 0x18) = (u32) ((u32) (temp_r2 & 0x3000) >> 0xC);
    M2C_FIELD(var_r1, s32 *, 8) = (s32) M2C_FIELD(arg0, u8 *, 0);
    var_r8 = 0;
    if (var_r8 < (s32) M2C_FIELD(arg0, s8 *, 1)) {
        var_r5 = arg0 + 2;
        var_r7 = var_r1 + 0x24;
        do {
            sp4 = var_r1;
            sp0 = var_r4;
            if ((M2C_FIELD(Func_08077008(*var_r5), s16 *, 0x38) != 0) || (M2C_FIELD(arg0, s32 *, 0x58) & 0x10000)) {
                var_r4 += 1;
                *var_r7 = (s16) *var_r5;
                var_r7 += 1;
            }
            var_r8 += 1;
            var_r5 += 1;
        } while (var_r8 < (s32) M2C_FIELD(arg0, s8 *, 1));
    }
    if (var_r4 == 0) {
        M2C_FIELD(var_r1, s16 *, 0x24) = (s16) M2C_FIELD(arg0, u8 *, 2);
        var_r4 = 1;
    }
    M2C_FIELD(var_r1, s32 *, 0xC) = (s32) M2C_FIELD(arg0, u8 *, 2);
    M2C_FIELD(var_r1, s32 *, 0x14) = var_r4;
    M2C_FIELD(var_r1, s32 *, 0x10) = 1;
}
