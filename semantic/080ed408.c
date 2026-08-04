#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_080048b0(s32, s32);

s32 Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *sp0;
    void *sp4;
    s32 *sp8;
    s32 spC;
    u8 *temp_r5_7;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r4;
    s32 temp_r4_2;
    u8 *temp_r5;
    u8 *temp_r5_14;
    u8 *temp_r5_15;
    u8 *temp_r5_22;
    u8 *temp_r5_25;
    u8 *temp_r5_26;
    u8 *temp_r5_27;
    u8 *temp_r5_31;
    u8 *temp_r5_32;
    u8 *temp_r5_33;
    u8 *temp_r5_9;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r1_3;
    s32 var_r1_4;
    s32 var_r1_5;
    s32 var_r1_6;
    s32 var_r1_7;
    s32 var_r1_8;
    s32 var_r1_9;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r4;
    u8 *var_r5;
    u8 *var_r5_4;
    u8 *var_r5_8;
    void *temp_r3_5;
    void *temp_r3_6;
    void *temp_r3_7;
    void *temp_r5_10;
    void *temp_r5_11;
    void *temp_r5_12;
    void *temp_r5_13;
    void *temp_r5_16;
    void *temp_r5_17;
    void *temp_r5_18;
    void *temp_r5_19;
    void *temp_r5_20;
    void *temp_r5_21;
    void *temp_r5_23;
    void *temp_r5_24;
    void *temp_r5_28;
    void *temp_r5_29;
    void *temp_r5_2;
    void *temp_r5_30;
    void *temp_r5_34;
    void *temp_r5_35;
    void *temp_r5_3;
    void *temp_r5_4;
    void *temp_r5_5;
    void *temp_r5_6;
    void *temp_r5_8;
    void *temp_r6;
    void *temp_r6_10;
    void *temp_r6_11;
    void *temp_r6_12;
    void *temp_r6_13;
    void *temp_r6_14;
    void *temp_r6_15;
    void *temp_r6_16;
    void *temp_r6_17;
    void *temp_r6_18;
    void *temp_r6_19;
    void *temp_r6_20;
    void *temp_r6_21;
    void *temp_r6_22;
    void *temp_r6_23;
    void *temp_r6_24;
    void *temp_r6_25;
    void *temp_r6_26;
    void *temp_r6_27;
    void *temp_r6_28;
    void *temp_r6_29;
    void *temp_r6_2;
    void *temp_r6_30;
    void *temp_r6_31;
    void *temp_r6_32;
    void *temp_r6_33;
    void *temp_r6_3;
    void *temp_r6_4;
    void *temp_r6_5;
    void *temp_r6_6;
    void *temp_r6_7;
    void *temp_r6_8;
    void *temp_r6_9;
    void *var_r0;
    void *var_r0_2;
    void *var_r0_3;
    void *var_r0_4;
    void *var_r0_5;
    void *var_r0_6;
    void *var_r0_7;
    void *var_r0_8;
    void *var_r4_2;
    void *var_r4_3;
    void *var_r5_10;
    void *var_r5_2;
    void *var_r5_3;
    void *var_r5_5;
    void *var_r5_6;
    void *var_r5_7;
    void *var_r5_9;

    spC = arg2;
    var_r1 = 3;
    if (arg4 == 3) {
        var_r1 = 6;
    }
    temp_r3_2 = 0xC & arg3;
    if (temp_r3_2 == 4) {
        var_r1 += 3;
    }
    if (temp_r3_2 == 8) {
        var_r1 += 4;
    }
    if (temp_r3_2 == 0xC) {
        var_r1 += 3;
    }
    if (temp_r3_2 == 0) {
        var_r1 += 1;
    }
    if (2 & arg3) {
        var_r1 += 7;
    }
    temp_r3_3 = 1 & arg3;
    var_r1_2 = var_r1 + 2;
    if (temp_r3_3 == 0) {
        var_r1_2 += 1;
    }
    if (temp_r3_3 != 0) {
        var_r1_2 = var_r1_2 + 2 + 2 + 5;
    }
    temp_r3_4 = 1 & arg3;
    var_r1_3 = var_r1_2 + 9;
    if (temp_r3_4 == 0) {
        var_r1_3 += 1;
    }
    var_r1_4 = var_r1_3 + 9;
    if (temp_r3_4 == 0) {
        var_r1_4 += 1;
    }
    temp_r1 = var_r1_4 + 3;
    switch (arg4) {
    default:
        var_r1_5 = temp_r1 + 2;
        break;
    case 1:
    case 2:
        var_r1_5 = temp_r1 + 4;
        break;
    case 3:
        var_r1_5 = temp_r1 + 6;
        break;
    }
    var_r1_6 = var_r1_5 + 3;
    if (!(1 & arg3)) {
        var_r1_6 += 2;
    }
    var_r1_7 = var_r1_6 + 2;
    switch (arg4) {
    default:
        var_r3 = 0;
        do {
            var_r3 += 1;
            var_r1_7 += 2;
        } while (var_r3 <= 7);
        break;
    case 1:
        var_r1_7 += 0x19;
        break;
    case 2:
        var_r1_7 += 0x20;
        break;
    case 3:
        var_r1_7 += 0x1C;
        break;
    }
    var_r1_8 = var_r1_7 + 1;
    if (!(1 & arg3)) {
        var_r1_8 += 2;
    }
    temp_r1_2 = var_r1_8 + 5;
    switch (arg4) {
    default:
        var_r1_9 = temp_r1_2 + 2;
        break;
    case 1:
    case 2:
        var_r1_9 = temp_r1_2 + 4;
        break;
    case 3:
        var_r1_9 = temp_r1_2 + 6;
        break;
    }
    temp_r5 = Func_080048b0(arg0, (var_r1_9 + 8) * 4);
    temp_r1_3 = (s32) temp_r5;
    M2C_FIELD((void *)0x040000D4, void **, 0) = (void *)0x080EDCC4;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = temp_r1_3;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000003;
    temp_r3_5 = (void *)0x040000D4;
    var_r5 = temp_r5 + 0xC;
    if (arg4 == 3) {
        M2C_FIELD(temp_r3_5, s32 *, 0) = 0x080EDCB8;
        M2C_FIELD(temp_r3_5, s32 *, 4) = (s32) var_r5;
        M2C_FIELD(temp_r3_5, s32 *, 8) = 0x84000003;
        var_r5 += 0xC;
    }
    temp_r4 = 0xC & arg3;
    if (temp_r4 == 4) {
        M2C_FIELD((void *)0x040000D4, void **, 0) = (void *)0x080EDCD0;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000003;
        var_r5 += 0xC;
    }
    if (temp_r4 == 8) {
        M2C_FIELD((void *)0x040000D4, void **, 0) = (void *)0x080EDCDC;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000004;
        var_r5 += 0x10;
    }
    if (temp_r4 == 0xC) {
        M2C_FIELD((void *)0x040000D4, void **, 0) = (void *)0x080EDCEC;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000003;
        var_r5 += 0xC;
    }
    if (temp_r4 == 0) {
        M2C_FIELD((void *)var_r5, s32 *, 0) =
            (s32) M2C_FIELD((void *)0x080EDCF8, s32 *, 0);
        var_r5 += 4;
    }
    temp_r6 = (void *)0x080EDCF8 + 4;
    if (2 & arg3) {
        M2C_FIELD(var_r5, s32 *, 0) = (s32) M2C_FIELD((void *)0x080EDCF8, s32 *, 4);
        temp_r5_2 = var_r5 + 4;
        M2C_FIELD(var_r5, s32 *, 4) = (s32) M2C_FIELD(temp_r6, s32 *, 4);
        temp_r5_3 = temp_r5_2 + 4;
        if (8 & arg3) {
            var_r3_2 = M2C_FIELD(temp_r6, s32 *, 8);
        } else {
            var_r3_2 = M2C_FIELD(temp_r6, s32 *, 0xC);
        }
        M2C_FIELD(temp_r5_2, s32 *, 4) = var_r3_2;
        temp_r5_4 = temp_r5_3 + 4;
        M2C_FIELD(temp_r5_3, s32 *, 4) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
        temp_r5_5 = temp_r5_4 + 4;
        M2C_FIELD(temp_r5_4, s32 *, 4) = (s32) M2C_FIELD(temp_r6, s32 *, 0x14);
        temp_r5_6 = temp_r5_5 + 4;
        temp_r2 = 1 << spC;
        M2C_FIELD(temp_r5_5, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0x18) + temp_r2);
        M2C_FIELD(temp_r5_6, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0x1C) + temp_r2);
        var_r5 = temp_r5_6 + 4 + 4;
    }
    temp_r6_2 = temp_r6 + 0x20;
    temp_r6_3 = temp_r6_2 + 4;
    M2C_FIELD(var_r5, s32 *, 0) = (s32) M2C_FIELD(temp_r6, s32 *, 0x20);
    temp_r5_7 = var_r5 + 4;
    sp8 = (s32 *) temp_r5_7;
    temp_r6_4 = temp_r6_3 + 4;
    M2C_FIELD(var_r5, s32 *, 4) = (s32) M2C_FIELD(temp_r6_2, s32 *, 4);
    var_r5_2 = temp_r5_7 + 4;
    temp_r1_4 = arg3 & 1;
    if (temp_r1_4 == 0) {
        M2C_FIELD(temp_r5_7, s32 *, 4) = (s32) ((M2C_FIELD(temp_r6_3, s32 *, 4) + (1 << arg1)) - 1);
        var_r5_2 += 4;
    }
    temp_r6_5 = temp_r6_4 + 4;
    if (temp_r1_4 != 0) {
        M2C_FIELD(var_r5_2, s32 *, 0) = (s32) M2C_FIELD(temp_r6_4, s32 *, 4);
        temp_r5_8 = var_r5_2 + 4;
        M2C_FIELD(var_r5_2, s32 *, 4) = (s32) M2C_FIELD(temp_r6_5, s32 *, 4);
        temp_r5_9 = temp_r5_8 + 4;
        if (4 & arg3) {
            M2C_FIELD(temp_r5_8, s32 *, 4) = (s32) M2C_FIELD(temp_r6_5, s32 *, 8);
            var_r5_3 = temp_r5_9 + 4;
            var_r3_3 = M2C_FIELD(temp_r6_5, s32 *, 0xC);
        } else {
            M2C_FIELD(temp_r5_8, s32 *, 4) = (s32) M2C_FIELD(temp_r6_5, s32 *, 0x10);
            var_r5_3 = temp_r5_9 + 4;
            var_r3_3 = M2C_FIELD(temp_r6_5, s32 *, 0x14);
        }
        M2C_FIELD(var_r5_3, s32 *, 0) = var_r3_3;
        temp_r5_10 = var_r5_3 + 4;
        M2C_FIELD(var_r5_3, s32 *, 4) = (s32) M2C_FIELD(temp_r6_5, s32 *, 0x18);
        temp_r5_11 = temp_r5_10 + 4;
        M2C_FIELD(temp_r5_10, s32 *, 4) = (s32) M2C_FIELD(temp_r6_5, s32 *, 0x1C);
        temp_r5_12 = temp_r5_11 + 4;
        temp_r1_5 = arg1 * 2;
        M2C_FIELD(temp_r5_11, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_5, s32 *, 0x20) + M2C_FIELD(temp_r1_5, u16 *, 0x080EF034));
        temp_r5_13 = temp_r5_12 + 4;
        if (4 & arg3) {
            var_r3_4 = M2C_FIELD(temp_r6_5, s32 *, 0x24);
        } else {
            var_r3_4 = M2C_FIELD(temp_r6_5, s32 *, 0x28);
        }
        M2C_FIELD(temp_r5_12, s32 *, 4) = var_r3_4;
        M2C_FIELD(temp_r5_13, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_5, s32 *, 0x2C) + M2C_FIELD(temp_r1_5, u16 *, 0x080EF034));
        var_r5_2 = temp_r5_13 + 4 + 4;
    }
    temp_r6_6 = temp_r6_5 + 0x30;
    M2C_FIELD(var_r5_2, s32 *, 0) = (s32) M2C_FIELD(temp_r6_5, s32 *, 0x30);
    temp_r5_14 = var_r5_2 + 4;
    temp_r6_7 = temp_r6_6 + 4 + 4;
    M2C_FIELD(var_r5_2, s32 *, 4) = (s32) M2C_FIELD(temp_r6_6, s32 *, 4);
    temp_r5_15 = temp_r5_14 + 4;
    M2C_FIELD((void *)0x040000D4, void **, 0) = temp_r6_7;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_15;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000006;
    temp_r5_16 = temp_r5_15 + 0x18;
    temp_r6_8 = temp_r6_7 + 0x18;
    M2C_FIELD(temp_r5_15, s32 *, 0x18) = (s32) M2C_FIELD(temp_r6_7, s32 *, 0x18);
    var_r5_4 = temp_r5_16 + 4;
    temp_r4_2 = arg3 & 1;
    if (temp_r4_2 == 0) {
        M2C_FIELD(temp_r5_16, s32 *, 4) = (s32) M2C_FIELD(temp_r6_8, s32 *, 4);
        var_r5_4 += 4;
    }
    temp_r6_9 = temp_r6_8 + 4 + 4;
    M2C_FIELD((void *)0x040000D4, void **, 0) = temp_r6_9;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5_4;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000005;
    temp_r5_17 = var_r5_4 + 0x14;
    temp_r6_10 = temp_r6_9 + 0x14;
    sp4 = temp_r5_17;
    M2C_FIELD(var_r5_4, s32 *, 0x14) = (s32) M2C_FIELD(temp_r6_9, s32 *, 0x14);
    temp_r5_18 = temp_r5_17 + 4;
    M2C_FIELD(temp_r5_17, s32 *, 4) = (s32) ((M2C_FIELD(temp_r6_10, s32 *, 4) + (1 << (spC - 3))) - 1);
    temp_r5_19 = temp_r5_18 + 4;
    M2C_FIELD(temp_r5_18, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_10, s32 *, 8) + ((arg1 - 3) << 7));
    temp_r5_20 = temp_r5_19 + 4;
    M2C_FIELD(temp_r5_19, s32 *, 4) = (s32) M2C_FIELD(temp_r6_10, s32 *, 0xC);
    var_r5_5 = temp_r5_20 + 4;
    temp_r6_11 = temp_r6_10 + 0x10;
    if (temp_r4_2 == 0) {
        M2C_FIELD(temp_r5_20, s32 *, 4) = (s32) M2C_FIELD(temp_r6_10, s32 *, 0x10);
        var_r5_5 += 4;
    }
    temp_r6_12 = temp_r6_11 + 4;
    temp_r6_13 = temp_r6_12 + 4;
    M2C_FIELD(var_r5_5, s32 *, 0) = (s32) M2C_FIELD(temp_r6_11, s32 *, 4);
    temp_r5_21 = var_r5_5 + 4;
    temp_r6_14 = temp_r6_13 + 4;
    M2C_FIELD(var_r5_5, s32 *, 4) = (s32) M2C_FIELD(temp_r6_12, s32 *, 4);
    if (4 & arg3) {
        var_r3_5 = M2C_FIELD(temp_r6_13, s32 *, 4);
    } else {
        var_r3_5 = M2C_FIELD(temp_r6_14, s32 *, 4);
    }
    M2C_FIELD(temp_r5_21, s32 *, 4) = var_r3_5;
    temp_r5_22 = temp_r5_21 + 4 + 4;
    temp_r6_15 = temp_r6_14 + 8;
    switch (arg4) {
    default:
        if (4 & arg3) {
            var_r0 = (void *)0x080EDAF0;
        } else {
            var_r0 = (void *)0x080EDAF8;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_22;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000002;
        var_r5_6 = temp_r5_22 + 8;
        break;
    case 1:
        if (4 & arg3) {
            var_r0_2 = (void *)0x080EDB10;
        } else {
            var_r0_2 = (void *)0x080EDB00;
        }
block_94:
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_2;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_22;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000004;
        var_r5_6 = temp_r5_22 + 0x10;
        break;
    case 2:
        if (4 & arg3) {
            var_r0_2 = (void *)0x080EDBF8;
        } else {
            var_r0_2 = (void *)0x080EDBE8;
        }
        goto block_94;
    case 3:
        if (4 & arg3) {
            var_r0_3 = (void *)0x080EDCA0;
        } else {
            var_r0_3 = (void *)0x080EDC88;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_3;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_22;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000006;
        var_r5_6 = temp_r5_22 + 0x18;
        break;
    }
    temp_r6_16 = temp_r6_15 + 0x10;
    temp_r6_17 = temp_r6_16 + 4;
    M2C_FIELD(var_r5_6, s32 *, 0) = (s32) M2C_FIELD(temp_r6_15, s32 *, 0x10);
    temp_r5_23 = var_r5_6 + 4;
    temp_r6_18 = temp_r6_17 + 4;
    M2C_FIELD(var_r5_6, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_16, s32 *, 4) + (((u32) (((s32) temp_r5_22 - (s32) temp_r5_23) - 8) >> 2) & 0xFFFFFF));
    temp_r5_24 = temp_r5_23 + 4;
    temp_r6_19 = temp_r6_18 + 4;
    M2C_FIELD(temp_r5_23, s32 *, 4) = (s32) M2C_FIELD(temp_r6_17, s32 *, 4);
    var_r5_7 = temp_r5_24 + 4;
    if (!(1 & arg3)) {
        M2C_FIELD(temp_r5_24, s32 *, 4) = (s32) M2C_FIELD(temp_r6_18, s32 *, 4);
        M2C_FIELD(var_r5_7, s32 *, 4) = (s32) M2C_FIELD(temp_r6_19, s32 *, 4);
        var_r5_7 = var_r5_7 + 4 + 4;
    }
    temp_r6_20 = temp_r6_19 + 8;
    M2C_FIELD(var_r5_5, s32 *, 4) = (s32) (M2C_FIELD(var_r5_5, s32 *, 4) | (((u32) ((var_r5_7 - temp_r5_21) - 8) >> 2) & 0xFFFFFF));
    M2C_FIELD(var_r5_7, s32 *, 0) = (s32) M2C_FIELD(temp_r6_19, s32 *, 8);
    temp_r5_25 = var_r5_7 + 4;
    temp_r6_21 = temp_r6_20 + 4 + 4;
    M2C_FIELD(var_r5_7, s32 *, 4) = (s32) M2C_FIELD(temp_r6_20, s32 *, 4);
    var_r5_8 = temp_r5_25 + 4;
    sp0 = var_r5_8;
    switch (arg4) {
    default:
        var_r4 = 0;
        do {
            if (4 & arg3) {
                var_r0_4 = (void *)0x080EDAF0;
            } else {
                var_r0_4 = (void *)0x080EDAF8;
            }
            M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_4;
            M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5_8;
            M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000002;
            var_r5_8 += 8;
            var_r4 += 1;
        } while (var_r4 <= 7);
        break;
    case 1:
        if (4 & arg3) {
            var_r0_5 = (void *)0x080EDB84;
        } else {
            var_r0_5 = (void *)0x080EDB20;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_5;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5_8;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000019;
        var_r5_8 += 0x64;
        break;
    case 2:
        if (4 & arg3) {
            var_r4_2 = (void *)0x080EDC48;
        } else {
            var_r4_2 = (void *)0x080EDC08;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r4_2;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5_8;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000010;
        temp_r3_6 = (void *)0x040000D4;
        temp_r5_26 = var_r5_8 + 0x40;
        M2C_FIELD(temp_r3_6, void **, 0) = var_r4_2;
        M2C_FIELD(temp_r3_6, s32 *, 4) = (s32) temp_r5_26;
        M2C_FIELD(temp_r3_6, s32 *, 8) = 0x84000010;
        var_r5_8 = temp_r5_26 + 0x40;
        break;
    case 3:
        if (4 & arg3) {
            var_r4_3 = (void *)0x080EEFDC;
        } else {
            var_r4_3 = (void *)0x080EEFA4;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r4_3;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) var_r5_8;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x8400000E;
        temp_r3_7 = (void *)0x040000D4;
        temp_r5_27 = var_r5_8 + 0x38;
        M2C_FIELD(temp_r3_7, void **, 0) = var_r4_3;
        M2C_FIELD(temp_r3_7, s32 *, 4) = (s32) temp_r5_27;
        M2C_FIELD(temp_r3_7, s32 *, 8) = 0x8400000E;
        var_r5_8 = temp_r5_27 + 0x38;
        break;
    }
    temp_r6_22 = temp_r6_21 + 0x10;
    temp_r6_23 = temp_r6_22 + 4;
    M2C_FIELD(var_r5_8, s32 *, 0) = (s32) M2C_FIELD(temp_r6_21, s32 *, 0x10);
    var_r5_9 = var_r5_8 + 4;
    if (!(1 & arg3)) {
        M2C_FIELD(var_r5_8, s32 *, 4) = (s32) M2C_FIELD(temp_r6_22, s32 *, 4);
        M2C_FIELD(var_r5_9, s32 *, 4) = (s32) M2C_FIELD(temp_r6_23, s32 *, 4);
        var_r5_9 = var_r5_9 + 4 + 4;
    }
    temp_r6_24 = temp_r6_23 + 8;
    temp_r6_25 = temp_r6_24 + 4;
    M2C_FIELD(var_r5_9, s32 *, 0) = (s32) M2C_FIELD(temp_r6_23, s32 *, 8);
    temp_r5_28 = var_r5_9 + 4;
    temp_r6_26 = temp_r6_25 + 4;
    M2C_FIELD(var_r5_9, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_24, s32 *, 4) + (((u32) ((sp0 - (u8 *)temp_r5_28) - 8) >> 2) & 0xFFFFFF));
    temp_r5_29 = temp_r5_28 + 4;
    M2C_FIELD(var_r5_7, s32 *, 4) = (s32) (M2C_FIELD(var_r5_7, s32 *, 4) | (((u32) (((u8 *)temp_r5_29 - temp_r5_25) - 8) >> 2) & 0xFFFFFF));
    temp_r6_27 = temp_r6_26 + 4;
    M2C_FIELD(temp_r5_28, s32 *, 4) = (s32) M2C_FIELD(temp_r6_25, s32 *, 4);
    temp_r5_30 = temp_r5_29 + 4;
    temp_r6_28 = temp_r6_27 + 4;
    M2C_FIELD(temp_r5_29, s32 *, 4) = (s32) M2C_FIELD(temp_r6_26, s32 *, 4);
    if (4 & arg3) {
        var_r3_6 = M2C_FIELD(temp_r6_27, s32 *, 4);
    } else {
        var_r3_6 = M2C_FIELD(temp_r6_28, s32 *, 4);
    }
    M2C_FIELD(temp_r5_30, s32 *, 4) = var_r3_6;
    temp_r5_31 = temp_r5_30 + 4 + 4;
    temp_r6_29 = temp_r6_28 + 8;
    switch (arg4) {
    default:
        if (4 & arg3) {
            var_r0_6 = (void *)0x080EDAF0;
        } else {
            var_r0_6 = (void *)0x080EDAF8;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_6;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_31;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000002;
        var_r5_10 = temp_r5_31 + 8;
        break;
    case 1:
        if (4 & arg3) {
            var_r0_7 = (void *)0x080EDB10;
        } else {
            var_r0_7 = (void *)0x080EDB00;
        }
block_142:
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_7;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_31;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000004;
        var_r5_10 = temp_r5_31 + 0x10;
        break;
    case 2:
        if (4 & arg3) {
            var_r0_7 = (void *)0x080EDBF8;
        } else {
            var_r0_7 = (void *)0x080EDBE8;
        }
        goto block_142;
    case 3:
        if (4 & arg3) {
            var_r0_8 = (void *)0x080EDCA0;
        } else {
            var_r0_8 = (void *)0x080EDC88;
        }
        M2C_FIELD((void *)0x040000D4, void **, 0) = var_r0_8;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_31;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000006;
        var_r5_10 = temp_r5_31 + 0x18;
        break;
    }
    temp_r6_30 = temp_r6_29 + 0x10;
    M2C_FIELD(var_r5_10, s32 *, 0) = (s32) M2C_FIELD(temp_r6_29, s32 *, 0x10);
    temp_r5_32 = var_r5_10 + 4;
    temp_r6_31 = temp_r6_30 + 4 + 4;
    M2C_FIELD(var_r5_10, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_30, s32 *, 4) + (((u32) ((temp_r5_31 - temp_r5_32) - 8) >> 2) & 0xFFFFFF));
    temp_r5_33 = temp_r5_32 + 4;
    M2C_FIELD(temp_r5_29, s32 *, 4) = (s32) (M2C_FIELD(temp_r5_29, s32 *, 4) | (((u32) (((s32) temp_r5_33 - (s32) temp_r5_30) - 8) >> 2) & 0xFFFFFF));
    M2C_FIELD((void *)0x040000D4, void **, 0) = temp_r6_31;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = (s32) temp_r5_33;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000003;
    temp_r5_34 = temp_r5_33 + 0xC;
    temp_r6_32 = temp_r6_31 + 0xC;
    temp_r6_33 = temp_r6_32 + 4;
    M2C_FIELD(temp_r5_33, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r6_31, s32 *, 0xC) + (((u32) ((sp4 - temp_r5_34) - 8) >> 2) & 0xFFFFFF));
    temp_r5_35 = temp_r5_34 + 4;
    *sp8 |= ((u32) (((s32) temp_r5_35 - (s32) sp8) - 8) >> 2) & 0xFFFFFF;
    M2C_FIELD(var_r5_2, s32 *, 4) = (s32) (M2C_FIELD(var_r5_2, s32 *, 4) | (((u32) (((u8 *)temp_r5_35 - temp_r5_14) - 8) >> 2) & 0xFFFFFF));
    M2C_FIELD(temp_r5_34, s32 *, 4) = (s32) M2C_FIELD(temp_r6_32, s32 *, 4);
    M2C_FIELD(temp_r5_35, s32 *, 4) = (s32) M2C_FIELD(temp_r6_33, s32 *, 4);
    temp_r3 = 0x080EDE48 ^ (s32) (temp_r6_33 + 4 + 4);
    return 1 - ((u32) ((0 - temp_r3) | temp_r3) >> 0x1F);
}
