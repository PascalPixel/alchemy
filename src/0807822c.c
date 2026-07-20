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

s32 Func_08077394(s32);
s32 Func_080022ec(s32, s32);

void Func_0807822c(s32 arg0) {
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r3;
    s32 var_r3_2;
    void *temp_r0;

    temp_r0 = (void *) Func_08077394(arg0);
    temp_r0_2 = Func_080022ec(M2C_FIELD(temp_r0, s16, 0x38) << 0xE, (s32) M2C_FIELD(temp_r0, s16, 0x34));
    var_r3 = 0x4000;
    if (temp_r0_2 <= 0x4000) {
        var_r3 = 0;
        if (temp_r0_2 >= 0) {
            var_r3 = temp_r0_2;
        }
    }
    M2C_FIELD(temp_r0, s16, 0x14) = (s16) var_r3;
    if (((var_r3 << 0x10) == 0) && (M2C_FIELD(temp_r0, s16, 0x38) != 0)) {
        var_r3 = 1;
        M2C_FIELD(temp_r0, s16, 0x14) = (s16) var_r3;
    }
    temp_r0_3 = Func_080022ec(M2C_FIELD(temp_r0, s16, 0x3A) << 0xE, (s32) M2C_FIELD(temp_r0, s16, 0x36));
    var_r3_2 = 0x4000;
    if (temp_r0_3 <= 0x4000) {
        var_r3_2 = 0;
        if (temp_r0_3 >= 0) {
            var_r3_2 = temp_r0_3;
        }
    }
    M2C_FIELD(temp_r0, s16, 0x16) = (s16) var_r3_2;
    if (((var_r3_2 << 0x10) == 0) && (M2C_FIELD(temp_r0, s16, 0x3A) != 0)) {
        var_r3_2 = 1;
        M2C_FIELD(temp_r0, s16, 0x16) = (s16) var_r3_2;
    }
}
