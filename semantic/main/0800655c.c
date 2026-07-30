#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_0800655c(void) {
    s32 temp_r1_2;
    s32 temp_r2_3;
    s32 temp_r3;
    u8 temp_r1;
    u8 temp_r2;
    u8 temp_r2_2;
    u8 temp_r4;
    u8 var_r3;
    u8 *temp_r0;
    u8 *temp_r3_2;
    u8 *temp_r5;

    temp_r3 = (1 & ~((u32) (*(s32 *)0x04000128 << 0x1A) >> 0x1E)) * 0x18;
    temp_r3_2 = (u8 *)0x02002020 + temp_r3;
    if ((3 & *(u16 *)0x03001F64) != 3) {
        return;
    }
    temp_r5 = *(u8 **)0x020023AC;
    if (temp_r5 == NULL) {

    } else {
        if (M2C_FIELD((void *)0x02002220, u8 *, 2) != 1) {
            goto block_21;
        }
        if ((u32) ((M2C_FIELD(temp_r3_2, u8 *, 3) + 0xFF) << 0x18) <= 0x01000000U) {
            if (M2C_FIELD(temp_r3, u8 *, 0x02002020) == (0x7F & *(u8 *)0x020023A4)) {
                M2C_FIELD((void *)0x02002220, u8 *, 0) = 0U;
                temp_r4 = M2C_FIELD(temp_r3_2, u8 *, 3);
                switch (temp_r4) {
                case 1:
                    M2C_FIELD((void *)0x040000D4, void **, 0) = (void *) (temp_r3_2 + 4);
                    M2C_FIELD((void *)0x040000D4, void **, 4) = temp_r5;
                    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000005;
                    *(u8 **)0x020023AC += 0x14;
                    *(u16 *)0x02002238 = (u16) (*(u16 *)0x02002238 + 0x14);
                    M2C_FIELD((void *)0x02002220, u8 *, 1) = (u8) ((M2C_FIELD((void *)0x02002220, u8 *, 1) + 1) | ~0x7F);
                    break;
                case 2:
                    M2C_FIELD((void *)0x040000D4, void **, 0) = (void *) (temp_r3_2 + 4);
                    M2C_FIELD((void *)0x040000D4, void **, 4) = temp_r5;
                    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000005;
                    *(u16 *)0x02002238 += 0x14;
                    M2C_FIELD((void *)0x02002220, u8 *, 2) = temp_r4;
                    M2C_FIELD((void *)0x02002220, u8 *, 1) = 0U;
                    M2C_FIELD((void *)0x02002220, u8 *, 0) = 1U;
                    break;
                }
                *(u8 *)0x020023A4 = (u8) ((*(u8 *)0x020023A4 + 1) & 0x7F);
            } else if (0x80 & *(u8 *)0x020023A4) {
                temp_r1 = M2C_FIELD((void *)0x02002220, u8 *, 0);
                temp_r2 = 0x80 & temp_r1;
                if (temp_r2 != 0) {
                    M2C_FIELD((void *)0x02002220, u8 *, 0) = 1U;
                } else if ((temp_r1 << 0x18) == 0x01000000) {
                    M2C_FIELD((void *)0x02002220, u8 *, 0) = temp_r2;
                    *(u8 *)0x020023A4 &= 0x7F;
                }
            } else {
                M2C_FIELD((void *)0x02002220, u8 *, 0) = (u8) (*(u8 *)0x020023A4 | 0x80);
                *(u8 *)0x020023A4 |= 0x80;
            }
        } else {
block_21:
            M2C_FIELD((void *)0x02002220, u8 *, 0) = 0U;
        }
    }
    temp_r0 = *(u8 **)0x02002080;
    if (temp_r0 != NULL) {
        temp_r2_2 = M2C_FIELD(temp_r3_2, u8 *, 2);
        if (temp_r2_2 == 1) {
            if (0x80 & M2C_FIELD(temp_r3, u8 *, 0x02002020)) {
                temp_r1_2 = (*(u8 *)0x020023A4 - M2C_FIELD(temp_r3, u8 *, 0x02002020)) & 0x7F;
                temp_r2_3 = temp_r1_2 * 0x14;
                *(u8 **)0x02002080 = temp_r0 - temp_r2_3;
                *(u16 *)0x02002008 += temp_r2_3;
                *(u8 *)0x020023A4 = (u8) (*(u8 *)0x020023A4 - temp_r1_2);
                *(u8 *)0x020023A4 = (u8) (0x7F & *(u8 *)0x020023A4);
            }
            if (*(u16 *)0x02002008 != 0) {
                M2C_FIELD((void *)0x040000D4, void **, 0) = *(u8 **)0x02002080;
                M2C_FIELD((void *)0x040000D4, void **, 4) = (void *) ((void *)0x02002220 + 4);
                M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000005;
                *(u16 *)0x02002008 = (u16) (*(u16 *)0x02002008 + 0xFFEC);
                if (*(u16 *)0x02002008 != 0) {
                    M2C_FIELD((void *)0x02002220, u8 *, 3) = temp_r2_2;
                } else {
                    M2C_FIELD((void *)0x02002220, u8 *, 3) = 2U;
                }
                M2C_FIELD((void *)0x02002220, u8 *, 0) = (u8) (0x7F & *(u8 *)0x020023A4);
                *(u8 **)0x02002080 += 0x14;
                *(u8 *)0x020023A4 = (u8) ((*(u8 *)0x020023A4 + 1) & 0x7F);
            }
        }
        if ((M2C_FIELD((void *)0x02002220, u8 *, 3) == 2) && (M2C_FIELD(temp_r3_2, u8 *, 2) == 2)) {
            *(u8 **)0x02002080 = NULL;
            M2C_FIELD((void *)0x02002220, u8 *, 3) = 0U;
            M2C_FIELD((void *)0x02002220, u8 *, 0) = 1U;
        }
    }
    if (M2C_FIELD((void *)0x02002220, u8 *, 2) == 2) {
        if (M2C_FIELD(temp_r3_2, u8 *, 3) != 2) {
            var_r3 = 0;
            *(u8 **)0x020023AC = NULL;
            goto block_39;
        }
    } else {
        M2C_FIELD((void *)0x02002220, u8 *, 2) = 0U;
        if (*(u8 **)0x020023AC != NULL) {
            var_r3 = 1;
block_39:
            M2C_FIELD((void *)0x02002220, u8 *, 2) = var_r3;
        }
    }
}
