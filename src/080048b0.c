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

s32 Func_080048b0(s32 arg0, s32 arg1) {
    u32 *base;
    s32 temp_r5;
    s32 temp_r1;
    u32 temp_r0;
    u32 temp_r1_2;
    u32 temp_r2;
    u32 var_r0;

    base = (u32 *)0x03001E50;
    temp_r5 = arg0 * 4;
    var_r0 = M2C_FIELD(base, u32 *, temp_r5);
    if (var_r0 == 0) {
        var_r0 = base[1];
        temp_r1 = ((u32) (arg1 + 3) >> 2) * 4;
        temp_r2 = var_r0 + temp_r1;
        if (temp_r2 > 0x030077FFU) {
            temp_r0 = base[0];
            temp_r1_2 = temp_r0 + temp_r1;
            if (temp_r1_2 >= 0x02040000U) {
                return 0;
            }
            base[0] = temp_r1_2;
            M2C_FIELD(base, u32 *, temp_r5) = temp_r0;
            return (s32) temp_r0;
        }
        base[1] = temp_r2;
        M2C_FIELD(base, u32 *, temp_r5) = var_r0;
        return (s32) var_r0;
    }
    return (s32) var_r0;
}
