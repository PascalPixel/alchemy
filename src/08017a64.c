typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08017a64(u16 *arg0) {
    s32 var_r1;
    u16 *var_r0;
    u32 var_r2;
    u32 temp_r3;

    var_r2 = *arg0;
    var_r1 = 0;
    var_r0 = arg0 + 1;
    if (var_r2 != 0) {
        do {
            if (var_r2 == 0x20) {
                var_r1 += 4;
            } else if (var_r2 <= 0xFFU) {
                temp_r3 = var_r2 - 0xDE;
                if (temp_r3 > 1U) {
                    var_r1 += M2C_FIELD(((temp_r3 + 0xBE) << 5), u16, 0x08032224);
                }
            } else {
                var_r1 += 0xA;
            }
            var_r2 = *var_r0;
            var_r0 += 1;
        } while (var_r2 != 0);
    }
    return var_r1;
}
