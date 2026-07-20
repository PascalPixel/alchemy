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

s32 Func_0800bc48(s32);

void Func_0800b93c(void *arg0, s32 arg1) {
    s32 *var_r1;
    s32 *var_r2;
    s32 temp_r3;
    s32 var_r4;
    u32 var_r0;
    u32 var_r2_2;
    u32 off;

    if ((arg0 != 0) && (arg1 != 0)) {
        Func_0800bc48(arg1);
        var_r0 = 0;
        if (arg1 != M2C_FIELD(arg0, s32, 0x28)) {
            var_r2 = (s32 *)((u8 *)arg0 + 0x28);
loop_4:
            var_r0 += 1;
            if (var_r0 <= 3U) {
                var_r2 += 1;
                if (arg1 != *var_r2) {
                    goto loop_4;
                }
            }
        }
        if (var_r0 != 4) {
            off = (var_r0 * 4) + 0x28;
            M2C_FIELD(arg0, s32, off) = 0;
            var_r2_2 = var_r0 + 1;
            var_r4 = 0;
            if (var_r2_2 <= 3U) {
                var_r1 = (s32 *)((var_r2_2 * 4) + (u32)arg0 + 0x28);
                do {
                    temp_r3 = *var_r1++;
                    if (temp_r3 != 0) {
                        var_r4 += 1;
                    }
                    var_r2_2 += 1;
                } while (var_r2_2 <= 3U);
            }
            if (var_r4 == 0) {
                M2C_FIELD(arg0, s8, 0x27) = (s8) var_r0;
            }
        }
    }
}
