#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_080152d0(s16, s32, s32, s32, s32);
void *Func_080b0744(s16, s32, s32, s32);

void Func_080b0fa4(s32 arg0, s32 arg1) {
    s32 sp4;
    s32 sp8;
    s16 *temp_r7;
    s16 *var_r8;
    s16 temp_r5_2;
    s32 var_fp;
    u32 var_r6;
    u32 var_sl;
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r5;

    sp8 = arg1;
    temp_r5 = *(void **)0x03001F2C;
    sp4 = (s32) (s8) M2C_FIELD(temp_r5, u8 *, 0x3A6);
    var_r6 = sp8 - Func_080022fc(arg1, 7);
    if (arg0 != 0) {
        Func_08015060(arg0);
        if (var_r6 != 0) {
            temp_r0 = (void *) Func_080150c8((s32) M2C_FIELD(temp_r5, u16 *, 0x392), 0x40000000, arg0, 0xD8, -0x10);
            M2C_FIELD(temp_r0, s8 *, 4) = 0;
            M2C_FIELD(temp_r0, s8 *, 5) = 0x11;
            M2C_FIELD(temp_r0, s16 *, 0xC) = 0;
        }
        if ((s32) (var_r6 + 7) < sp4) {
            temp_r0_2 = (void *) Func_080150c8((s32) M2C_FIELD(temp_r5, u16 *, 0x394), 0x40000000, arg0, 0xD8, 0x18);
            M2C_FIELD(temp_r0_2, s8 *, 4) = 0;
            M2C_FIELD(temp_r0_2, s8 *, 5) = 0xF;
            M2C_FIELD(temp_r0_2, s16 *, 0xC) = 0;
        }
        var_sl = 0;
        if (var_r6 < (u32) sp4) {
            var_r8 = (var_r6 * 2) + (temp_r5 + 0x26C);
            var_fp = 0x10;
loop_7:
            temp_r5_2 = *var_r8;
            temp_r7 = Func_08077018((s32) temp_r5_2);
            temp_r0_3 = Func_080152d0(temp_r5_2, 1, arg0, var_sl << 5, 0);
            M2C_FIELD(temp_r0_3, s8 *, 0xF) = 0xFC;
            if (var_r6 == sp8) {
                M2C_FIELD(temp_r0_3, s8 *, 5) = 9;
                M2C_FIELD(temp_r0_3, s16 *, 0xC) = 0xA;
                M2C_FIELD(temp_r0_3, s8 *, 0xF) = 0xFD;
            }
            M2C_FIELD(Func_080b0744(*temp_r7, arg0, var_fp, 0), s8 *, 0xF) = 0xFB;
            var_sl += 1;
            var_fp += 0x20;
            var_r8 += 2;
            var_r6 += 1;
            if ((var_sl <= 6U) && (var_r6 < (u32) sp4)) {
                goto loop_7;
            }
        }
    }
}
