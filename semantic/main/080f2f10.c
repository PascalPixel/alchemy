#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080f2f10(void) {
    u8 *var_r4_2;
    s32 var_r0;
    s32 var_r0_2;
    u8 *var_r1;
    u8 *var_r4;
    u16 temp_r1;
    u16 temp_r2_2;
    u16 temp_r2_3;
    u16 temp_r6;
    u8 temp_r2;
    u8 *temp_r0;
    u8 *temp_r3;
    u8 *temp_r3_2;
    u8 *temp_r3_3;
    u8 *temp_r3_4;
    u8 *temp_r5;
    u8 *var_r1_2;

    temp_r5 = *(void **)0x03001ED0;
    var_r4 = temp_r5 + 0x1C00;
    if (M2C_FIELD(temp_r5, s8 *, 0x3001) == 0) {
        return;
    }
    temp_r2 = M2C_FIELD(temp_r5, u8 *, 0x3002) + 1;
    M2C_FIELD(temp_r5, u8 *, 0x3002) = temp_r2;
    if ((s32) (s8) temp_r2 < (s32) M2C_FIELD(temp_r5, s8 *, 0x3001)) {
        var_r1 = temp_r5 + 0x400;
        var_r0 = 0;
        do {
            var_r0 += 1;
            M2C_FIELD(var_r1, u16 *, 0) += M2C_FIELD(var_r4, u16 *, 0);
            var_r4 += 2;
            var_r1 += 2;
        } while (var_r0 <= 0x5FF);
    } else {
        M2C_FIELD((void *)0x040000D4, void **, 0) = (void *) (temp_r5 + 0x1000);
        M2C_FIELD((void *)0x040000D4, void **, 4) = (void *) (temp_r5 + 0x400);
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000300;
        M2C_FIELD(temp_r5, s8 *, 0x3001) = 0;
    }
    var_r4_2 = temp_r5 + ((1 ^ M2C_FIELD(temp_r5, u8 *, 0x3000)) << 0xA) + 0x2800;
    var_r0_2 = 0x200;
    var_r1_2 = temp_r5 + 0x400;
    do {
        var_r0_2 -= 1;
        M2C_FIELD(var_r4_2, u16 *, 0) =
            (0x7C00 & M2C_FIELD(var_r1_2, u16 *, 0)) |
            ((M2C_FIELD(var_r1_2, u16 *, 2) >> 5) & 0x3E0) |
            ((M2C_FIELD(var_r1_2, u16 *, 4) >> 10) & 0x1F);
        var_r1_2 += 6;
        var_r4_2 += 2;
    } while (var_r0_2 != 0);
    M2C_FIELD(temp_r5, u8 *, 0x3000) = (u8) (M2C_FIELD(temp_r5, u8 *, 0x3000) ^ 1);
    temp_r0 = temp_r5 + (M2C_FIELD(temp_r5, u8 *, 0x3000) << 0xA);
    temp_r1 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x0208;
    temp_r2_2 = *(u16 *)0x02002090;
    if ((s32) temp_r2_2 <= 0x1F) {
        temp_r3 = (u8 *)0x02002090 + (temp_r2_2 * 0xC);
        temp_r3_2 = temp_r3 + 4;
        M2C_FIELD(temp_r3, void **, 4) = (void *) (temp_r0 + 0x2800);
        *(u16 *)0x02002090 = temp_r2_2 + 1;
        M2C_FIELD(temp_r3_2, s32 *, 4) = 0x05000000;
        M2C_FIELD((temp_r3_2 + 4), s32 *, 4) = 0x84000080;
    }
    *(volatile u16 *)0x04000208 = temp_r1;
    temp_r6 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x0208;
    temp_r2_3 = *(u16 *)0x02002090;
    if ((s32) temp_r2_3 <= 0x1F) {
        temp_r3_3 = (u8 *)0x02002090 + (temp_r2_3 * 0xC);
        temp_r3_4 = temp_r3_3 + 4;
        *(u16 *)0x02002090 = temp_r2_3 + 1;
        M2C_FIELD(temp_r3_3, void **, 4) = (void *) (temp_r0 + 0x2A00);
        M2C_FIELD(temp_r3_4, s32 *, 4) = 0x05000200;
        M2C_FIELD((temp_r3_4 + 4), s32 *, 4) = 0x84000080;
    }
    *(volatile u16 *)0x04000208 = temp_r6;
}
