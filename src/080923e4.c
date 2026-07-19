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

void Func_080923e4(u32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r1;
    s32 var_r2;
    s32 sel;

    temp_r0 = Func_0808ba1c(arg0);
    if (temp_r0 != 0) {
        Func_08009140((void *) temp_r0);
        M2C_FIELD(temp_r0, s32 *, 0x24) = 0;
        M2C_FIELD(temp_r0, s32 *, 0x28) = 0;
        M2C_FIELD(temp_r0, s32 *, 0x2C) = 0;
        M2C_FIELD(temp_r0, s32 *, 0x3C) = 0x80000000;
        M2C_FIELD(temp_r0, s32 *, 0x38) = 0x80000000;
        M2C_FIELD(temp_r0, s32 *, 8) = arg1;
        M2C_FIELD(temp_r0, s32 *, 0x10) = arg2;
        if (1 & M2C_FIELD(temp_r0, u8 *, 0x55)) {
            sel = M2C_FIELD(temp_r0, u8 *, 0x22);
            var_r1 = arg1;
            if (var_r1 < 0) {
                var_r1 += 0xFFFF;
            }
            var_r1 = var_r1 >> 0x10;
            var_r2 = arg2;
            if (var_r2 < 0) {
                var_r2 += 0xFFFF;
            }
            var_r2 = var_r2 >> 0x10;
            temp_r0_2 = Func_080091a8(sel, var_r1, var_r2) << 0x10;
            M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) ((M2C_FIELD(temp_r0, s32 *, 0xC) - M2C_FIELD(temp_r0, s32 *, 0x14)) + temp_r0_2);
            M2C_FIELD(temp_r0, s32 *, 0x14) = temp_r0_2;
        }
    }
}
