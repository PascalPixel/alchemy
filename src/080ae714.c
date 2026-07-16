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

s32 Func_080ae778(s32, s32);

s32 Func_080ae714(s8 *arg0, s32 arg1) {
    s32 sp0;
    s32 var_r2;
    s32 var_r6;
    s8 *var_r5;
    void *temp_r3;

    temp_r3 = *(void **)0x03001F2C;
    var_r2 = 0;
    var_r6 = 0;
    if (var_r2 < (s32) M2C_FIELD(temp_r3, u8 *, 0x219)) {
        var_r5 = arg0;
        do {
            *var_r5 = 0;
            if (var_r6 != arg1) {
                sp0 = var_r2;
                if (Func_080ae778(arg1, var_r6) == 0) {
                    *var_r5 = 1;
                    var_r2 += 1;
                }
            }
            var_r6 += 1;
            var_r5 += 1;
        } while (var_r6 < (s32) M2C_FIELD(temp_r3, u8 *, 0x219));
    }
    return var_r2;
}
