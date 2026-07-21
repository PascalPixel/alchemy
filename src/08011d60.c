typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
#define M2C_FIELD(p, t, o) (*(t)((char *)(p) + (o)))

s32 Func_08011d60(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r4, var_r0, var_r2, d; u32 temp_r1 = arg1;
    temp_r4 = M2C_FIELD(arg0, s8 *, 0) << 0x13;
    var_r0 = M2C_FIELD(arg0, s8 *, 1) << 0x13;
    var_r2 = temp_r4;
    if (var_r0 > temp_r4) { var_r2 = var_r0; }
    d = arg2 - temp_r1; temp_r1 = d + 0xF;
    if (temp_r1 == 0xF) { return var_r2; }
    if (temp_r1 <= 0xEU) { var_r0 = temp_r4; }
    return var_r0;
}
