#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080045e8(void);

void Func_0801f088(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp0;
    s32 sp4;
    u8 *sp8;
    s32 spC;
    s32 sp10;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3_2;
    s32 temp_r6;
    u32 var_ip;
    s32 var_lr;
    u32 var_r0;
    s32 var_r0_2;
    s32 var_r1;
    s32 var_r2;
    s32 var_r3;
    s32 var_r7;
    u32 var_r8;
    s32 var_r9;
    u32 var_r4;
    u8 *temp_r3;

    sp10 = arg1;
    var_r9 = arg3;
    temp_r3 = *(void **)0x03001E8C;
    sp8 = temp_r3;
    sp4 = var_r9;
    if (M2C_FIELD(temp_r3, u8 *, 0xEA5) == 0) {
        M2C_FIELD((void *)0x040000D4, s32 *, 0) = Func_080045e8();
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x050001C0;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x80000010;
        *(u16 *)0x050001DC = *(u16 *)0x050001E8;
    }
    sp10 += M2C_FIELD(arg0, u16 *, 0xC);
    sp0 = (arg2 + M2C_FIELD(arg0, u16 *, 0xE)) << 5;
    spC = 4;
    do {
        var_r8 = 0x22222222;
        var_ip = 0xCCCCCCCC;
        if (var_r9 > 7) {
            var_r0 = 0xDDDDDDDD;
            var_r8 = 0x88888888;
            goto block_7;
        }
        if (var_r9 >= 0) {
            temp_r1 = var_r9 * 4;
            temp_r2 = 0x20 - temp_r1;
            var_r8 = (0x22222222U << temp_r1) |
                (temp_r2 == 32 ? 0 : 0x88888888U >> temp_r2);
            var_r0 = (0xCCCCCCCCU << temp_r1) |
                (temp_r2 == 32 ? 0 : 0xDDDDDDDDU >> temp_r2);
block_7:
            var_ip = var_r0;
        }
        var_lr = 0;
        var_r7 = 0;
loop_18:
        if (sp4 != 0) {
            if (var_lr > 2) {

            } else {
                goto block_9;
            }
        } else if (var_lr <= 0) {
block_9:
            temp_r6 = (M2C_FIELD(sp8, u16 *, (sp0 + sp10) * 2) & 0x3FF) << 5;
            var_r4 = M2C_FIELD((u8 *)0x0600001C, u32 *, temp_r6 - var_r7);
            var_r1 = 0;
            var_r0_2 = 0;
            do {
                temp_r2_2 = var_r4 & 0xF;
                if (temp_r2_2 == 0xE) {
                    var_r3 = (s32)(0xFU << (var_r1 * 4));
                    var_r2 = var_r8;
                    goto block_14;
                }
                if (temp_r2_2 == 1) {
                    var_r3 = (s32)(0xFU << (var_r1 * 4));
                    var_r2 = var_ip;
block_14:
                    var_r0_2 |= var_r3 & var_r2;
                } else {
                    var_r0_2 |= temp_r2_2 << (var_r1 * 4);
                }
                var_r1 += 1;
                var_r4 = var_r4 >> 4;
            } while (var_r1 <= 7);
            M2C_FIELD((u8 *)0x0600001C, s32 *, temp_r6 - var_r7) = var_r0_2;
            var_r7 += 4;
            var_lr += 1;
            goto loop_18;
        }
        temp_r3_2 = spC - 1;
        var_r9 -= 8;
        spC = temp_r3_2;
        sp10 += 1;
    } while (temp_r3_2 >= 0);
}
