#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s8 Func_080799b0(u8 arg0, s32 arg1) {
    s32 *sp0;
    s32 sp4;
    s32 *var_ip_3;
    s32 *var_r2;
    s32 *var_r2_2;
    s32 temp_r0;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r1;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r5;
    s32 var_r8;
    u8 *var_r1_2;
    u8 *var_r7;
    void *var_r5_2;

    var_r8 = -1;
    var_r0 = 0;
    if (((s32) arg0 <= 7) && ((Func_080797fc(arg0, (u8 *) arg1, &sp4), (Func_08079338(0x20) == 0)) || ((var_r0 = 0xC8, (arg0 != 0)) && (var_r0 = 0xC9, (arg0 != 1))))) {
        var_r0 = 0xCA;
        if (arg0 != 5) {
            var_r0 = -1;
            if (-1 == -1) {
                var_ip = -1;
                var_r5 = -1;
                var_r0_2 = 0;
                var_r2 = &sp4;
                do {
                    temp_r3 = *var_r2;
                    var_r2 += 4;
                    if (var_ip < temp_r3) {
                        var_ip = temp_r3;
                        var_r5 = var_r0_2;
                    }
                    var_r0_2 += 1;
                } while (var_r0_2 <= 3);
                var_r4 = -1;
                var_ip_2 = -1;
                var_r0_3 = 0;
                var_r2_2 = &sp4;
                do {
                    if (var_r0_3 != var_r5) {
                        temp_r3_2 = *var_r2_2;
                        if (var_ip_2 < temp_r3_2) {
                            var_ip_2 = temp_r3_2;
                            var_r4 = var_r0_3;
                        }
                    }
                    var_r0_3 += 1;
                    var_r2_2 += 4;
                } while (var_r0_3 <= 3);
                var_r1 = var_r5;
                if ((s32) (&sp4)[var_r4] > 9) {
                    var_r1 = var_r4;
                }
                temp_r0 = Func_080797ec(var_r5, var_r1);
                sp0 = &sp4;
                var_r5_2 = (void *)0x4248;
                var_r0_4 = 0xCA;
                var_r7 = (u8 *)0x08088D68;
loop_19:
                if (M2C_FIELD(var_r5_2, s32 *, 0x08084B1C) == temp_r0) {
                    var_r4_2 = 0;
                    if ((s32) *sp0 >= (s32) (*var_r7 * 0xA)) {
                        var_ip_3 = &sp4;
                        var_r1_2 = var_r5_2 + 0x08084B1C + 4;
loop_22:
                        var_r4_2 += 1;
                        if (var_r4_2 <= 3) {
                            var_r1_2 += 1;
                            var_ip_3 += 4;
                            if ((s32) *var_ip_3 >= (s32) (*var_r1_2 * 0xA)) {
                                goto loop_22;
                            }
                        }
                    }
                    if (var_r4_2 != 4) {
                        goto block_25;
                    }
                    var_r8 = var_r0_4;
                } else {
block_25:
                    var_r0_4 -= 1;
                    var_r7 -= 0x54;
                    var_r5_2 -= 0x54;
                    if (var_r0_4 >= 0) {
                        goto loop_19;
                    }
                }
                if (var_r8 == -1) {
                    var_r8 = 0;
                }
                var_r0 = var_r8;
            }
        }
    }
    return (s8) var_r0;
}
