#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_080090c8(s32, s32, s32);
void Func_08009098(void *, void *);
s32 Func_080044d0(s32, s32);
u8 *Func_08077008(s32);
s32 Func_08077078(void *, s32);
s16 Func_080b6d30(s32);
s16 Func_080b6eb4(s32);
s32 Func_080c2384(s32);
s32 Func_080c23a0(s32);

void Func_080b6f44(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s16 temp_r0;
    s16 temp_r7;
    s32 temp_r1;
    s32 temp_r2;
    u8 *temp_sl;
    s32 var_fp;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r1_3;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    u32 temp_r3_2;
    u8 temp_r3;
    void *temp_r8;

    sp10 = arg2;
    temp_r2 = arg2 << 0x10;
    spC = arg3;
    sp8 = temp_r2;
    sp4 = arg3 << 0x10;
    temp_r8 = Func_080090c8(0xF000, temp_r2, 0);
    temp_sl = Func_08077008(arg1);
    var_fp = 0;
    temp_r7 = Func_080b6d30(arg1);
    if (M2C_FIELD(temp_sl, u8 *, 0x129) == 0) {
        temp_r1 = Func_080c2384((s32) M2C_FIELD(temp_sl, u8 *, 0x128));
        if (temp_r7 == 0) {
            sp0 = temp_r1;
            var_fp = Func_080c23a0((s32) M2C_FIELD(temp_sl, u8 *, 0x128));
            var_r1 = temp_r1;
        } else {
            var_r1 = (s32) temp_r7;
        }
    } else {
        temp_r3 = M2C_FIELD(temp_sl, u8 *, 0x128);
        switch (temp_r3) {
        case 1:
            var_r1 = 0x12D;
            break;
        case 3:
            var_r1 = 0x12F;
            break;
        case 2:
            var_r1_2 = 0x97;
block_11:
            var_r1 = var_r1_2 * 2;
            break;
        case 5:
            var_r1 = 0x131;
            break;
        default:
            var_r1_2 = 0x96;
            goto block_11;
        }
        if ((u32) arg1 > 7U) {
            var_fp = 1;
        }
    }
    M2C_FIELD(arg0, s32 *, 0x18) = 0x10000;
    temp_r3_2 = M2C_FIELD(temp_sl, u8 *, 0x128) - 0x12;
    switch (temp_r3_2) {
    case 0x3C:
        var_r3 = 0x19999;
block_58:
        M2C_FIELD(arg0, s32 *, 0x18) = var_r3;
        break;
    case 0x47:
        var_r3 = 0x18CCC;
        goto block_58;
    case 0x70:
        var_r3 = 0x13333;
        goto block_58;
    case 0x71:
        var_r3 = 0x19999;
        goto block_58;
    case 0x78:
        var_r3 = 0x18000;
        goto block_58;
    case 0x81:
        var_r3 = 0x1CCCC;
        goto block_58;
    case 0x83:
        var_r3 = 0x1CCCC;
        goto block_58;
    case 0xB:
        var_r3 = 0x10000;
        goto block_58;
    case 0x67:
        var_r3 = 0x1B333;
        goto block_58;
    case 0x82:
        var_r3 = 0x18000;
        goto block_58;
    case 0x84:
        var_r3 = 0x18000;
        goto block_58;
    case 0x85:
        var_r3 = 0x18000;
        goto block_58;
    case 0x86:
        var_r3 = 0x18000;
        goto block_58;
    case 0x87:
        var_r3 = 0x18000;
        goto block_58;
    case 0x88:
        var_r3 = 0x18000;
        goto block_58;
    case 0x89:
        var_r3 = 0x18000;
        goto block_58;
    case 0x8A:
        var_r3 = 0x18000;
        goto block_58;
    case 0x8B:
        var_r3 = 0x18000;
        goto block_58;
    case 0x1D:
        var_r3 = 0x13333;
        goto block_58;
    case 0x1E:
        var_r3 = 0x13333;
        goto block_58;
    case 0x1F:
        var_r3 = 0x16666;
        goto block_58;
    case 0x42:
        var_r3 = 0x10000;
        goto block_58;
    case 0x43:
        var_r3 = 0x14000;
        goto block_58;
    case 0x6E:
        var_r3 = 0x16666;
        goto block_58;
    case 0x6F:
        var_r3 = 0x16666;
        goto block_58;
    case 0x4C:
        var_r3 = 0x18000;
        goto block_58;
    case 0x50:
        var_r3 = 0x14CCC;
        goto block_58;
    case 0x5C:
        var_r3 = 0x13333;
        goto block_58;
    case 0x72:
        var_r3 = 0x10CCC;
        goto block_58;
    case 0x73:
        var_r3 = 0x10CCC;
        goto block_58;
    case 0x74:
        var_r3 = 0x11999;
        goto block_58;
    case 0x75:
        var_r3 = 0x11999;
        goto block_58;
    case 0x76:
        var_r3 = 0x13333;
        goto block_58;
    case 0x77:
        var_r3 = 0x13333;
        goto block_58;
    case 0x7B:
        var_r3 = 0x18000;
        goto block_58;
    case 0x7E:
        var_r3 = 0x13333;
        goto block_58;
    case 0x7F:
        var_r3 = 0x18000;
        goto block_58;
    case 0x80:
        var_r3 = 0x18CCC;
        goto block_58;
    case 0x22:
        var_r3 = 0x14000;
        goto block_58;
    case 0x57:
        var_r3 = 0x14000;
        goto block_58;
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
    case 0xC:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x4A:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
    case 0x6C:
        var_r3 = 0xE666;
        goto block_58;
    }
    M2C_FIELD(arg0, void **, 0) = temp_r8;
    M2C_FIELD(arg0, s32 *, 0xC) = sp8;
    M2C_FIELD(arg0, s32 *, 0x10) = sp4;
    M2C_FIELD(arg0, s32 *, 0x14) = var_fp;
    M2C_FIELD(arg0, s16 *, 4) = (s16) var_r1;
    temp_r0 = Func_080b6eb4(arg1);
    M2C_FIELD(arg0, s16 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
    M2C_FIELD(arg0, s32 *, 0x24) = 0;
    M2C_FIELD(arg0, s16 *, 0x28) = 0;
    M2C_FIELD(arg0, s16 *, 0x2A) = 0;
    M2C_FIELD(arg0, s16 *, 0xA) = 0x1FE;
    M2C_FIELD(arg0, s16 *, 6) = temp_r0;
    if (((u32) M2C_FIELD(temp_sl, u8 *, 0x128) <= 1U) && (Func_08077078(temp_sl, 1) == 0xF)) {
        if (M2C_FIELD(temp_sl, u8 *, 0x128) == 0) {
            var_r1_3 = 0xF0;
        } else {
            var_r1_3 = 0xF1;
        }
        M2C_FIELD(arg0, s16 *, 4) = (s16) (var_r1_3 * 2);
        M2C_FIELD(arg0, s16 *, 6) = 0;
    }
    var_r3_2 = spC;
    if (var_r3_2 < 0) {
        var_r3_2 += 7;
    }
    M2C_FIELD(temp_r8, s16 *, 6) = (s16) (Func_080044d0(var_r3_2 >> 3, sp10) + 0x8000);
    M2C_FIELD(temp_r8, s8 *, 0x59) = 3;
    M2C_FIELD(temp_r8, s8 *, 0x55) = 2;
    if (M2C_FIELD(temp_sl, u8 *, 0x129) == 0) {
        var_r3_3 = 0x14CCC;
    } else {
        var_r3_3 = 0x10000;
    }
    M2C_FIELD(temp_r8, s32 *, 0x18) = var_r3_3;
    M2C_FIELD(temp_r8, s32 *, 0x1C) = var_r3_3;
    Func_08009098(temp_r8, (void *)0x080C5938);
}
