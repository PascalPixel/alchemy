typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_0800b074(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5) {
    s32 sp0;
    s32 sp4;
    s32 temp_r1;
    s32 temp_r5;
    s32 var_r9;
    s32 var_r7;
    s32 var_r6;
    u32 var_r0;
    u32 var_r4;
    s32 *temp_r2;
    void *temp_r3;

    var_r0 = M2C_FIELD(arg0, u8 *, 0x20) >> 1;
    var_r4 = M2C_FIELD(arg0, u8 *, 0x21) >> 1;
    sp4 = 8;
    sp0 = 4;
    var_r9 = 1;
    temp_r2 = arg5;
    temp_r1 = *temp_r2++;
    temp_r5 = *temp_r2;
    if (temp_r1 > 0x10000 || temp_r5 > 0x10000) {
        var_r9 = 3;
        sp4 = 16;
        sp0 = 8;
        var_r0 *= 2;
        var_r4 *= 2;
    }
    temp_r1 = arg1 >> 16;
    var_r7 = temp_r1 - var_r0;
    var_r6 = ((arg3 - arg2) >> 16) - var_r4;
    var_r6 -= (((M2C_FIELD(arg0, u8 *, 0x21) >> 1) - M2C_FIELD(arg0, s8 *, 0x23)) * temp_r5 + 0xFFFF) >> 16;
    M2C_FIELD(arg0, u8 *, 5) = (M2C_FIELD(arg0, u8 *, 5) & -4) | var_r9;
    M2C_FIELD(arg0, u16 *, 6) = (M2C_FIELD(arg0, u16 *, 6) & 0xFFFFFE00) | (var_r7 & 0x1FF);
    M2C_FIELD(arg0, s8 *, 4) = var_r6;
    var_r7 = temp_r1 - sp4;
    var_r6 = ((arg3 - arg4) >> 16) - sp0;
    temp_r3 = arg0 + 0xC;
    M2C_FIELD(temp_r3, u8 *, 5) = (M2C_FIELD(temp_r3, u8 *, 5) & -4) | var_r9;
    M2C_FIELD(temp_r3, u16 *, 6) = (M2C_FIELD(temp_r3, u16 *, 6) & 0xFFFFFE00) | (var_r7 & 0x1FF);
    M2C_FIELD(temp_r3, s8 *, 4) = var_r6;
}
