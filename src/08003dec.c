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

void Func_08003dec(s32 *arg0, s32 arg1) {
    s32 *temp_r2;
    s32 temp_r3;
    s32 var_r1;

    var_r1 = arg1;
    if (var_r1 > 0xFF) {
        var_r1 = 0xFF;
    }
    if (var_r1 < 0) {
        var_r1 = 0;
    }
    temp_r3 = var_r1 * 4;
    temp_r2 = M2C_FIELD(temp_r3, s32 **, 0x03001400);
    M2C_FIELD(temp_r3, s32 **, 0x03001400) = arg0;
    *arg0 = temp_r2;
}
