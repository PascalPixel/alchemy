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

u32 Func_08005c2c(s32 arg0) {
    u16 *var_r1;
    u16 temp_r3_2;
    u32 var_r0;
    u32 var_r4;
    void *temp_r3;
    void *var_r2;

    temp_r3 = *(void **)0x03001F1C;
    var_r4 = 0;
    var_r0 = 0;
    var_r1 = temp_r3 + 0x20;
    var_r2 = temp_r3;
    do {
        if ((M2C_FIELD(var_r2, u8 *, 0) != 0) && (arg0 == M2C_FIELD(var_r2, u8 *, 0x10))) {
            temp_r3_2 = *var_r1;
            if (var_r0 < (u32) temp_r3_2) {
                var_r0 = (u32) temp_r3_2;
            }
        }
        var_r4 += 1;
        var_r1 += 1;
        var_r2 += 1;
    } while (var_r4 <= 0xFU);
}
