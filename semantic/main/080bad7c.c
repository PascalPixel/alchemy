#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern void *Data_03001e74;
void *Func_08077008();
u32 Func_08004458();

u16 Func_080bad7c(s32 arg0)
{
    s32 sp0;
    u16 sp4[6];
    u8 *var_r1;
    s16 *var_r5;
    s16 temp_r0;
    s32 temp_r3;
    s32 var_r4;
    s32 var_r7;
    s32 var_r7_2;
    u8 *var_r0_2;
    u8 *var_r2;
    u8 *var_r6;
    u16 var_r0;
    void *temp_r1;

    var_r4 = 0;
    temp_r1 = Data_03001e74;
    if (arg0 != 0) {
        var_r7 = 0;
        if (M2C_FIELD(temp_r1, s16 *, 0x58) != 0xFF) {
            var_r5 = (s16 *)((u8 *)temp_r1 + 0x58);
            var_r6 = (u8 *)sp4;
            do {
                temp_r0 = *var_r5;
                if (temp_r0 != 0xFE) {
                    sp0 = var_r4;
                    if (M2C_FIELD((void *)Func_08077008((s32)temp_r0),
                                  s16 *, 0x38) != 0) {
                        *(u16 *)var_r6 = var_r7 | 0x100;
                        var_r4 += 1;
                        var_r6 += 2;
                    }
                }
                var_r5 += 1;
                var_r7 += 1;
            } while (*var_r5 != 0xFF);
        }
    } else {
        var_r7_2 = 0;
        if (M2C_FIELD(((u8 *)temp_r1 + 2), s16 *, 0x64) != 0xFF) {
            var_r0_2 = (u8 *)temp_r1 + 0x66;
            var_r2 = var_r0_2;
            var_r1 = (u8 *)&sp4[var_r4];
            do {
                temp_r3 = *(u16 *)var_r2 << 0x10;
                var_r2 += 2;
                if (temp_r3 != 0xFE0000) {
                    *(u16 *)var_r1 = var_r7_2 | 0x180;
                    var_r4 += 1;
                    var_r1 += 2;
                }
                var_r0_2 += 2;
                var_r7_2 += 1;
            } while (*(s16 *)var_r0_2 != 0xFF);
        }
    }
    var_r0 = 0;
    if (var_r4 != 0) {
        sp0 = var_r4;
        var_r0 = sp4[(u32)(var_r4 * Func_08004458()) >> 0x10];
    }
    return var_r0;
}
