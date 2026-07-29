#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define ANIMATION_ROWS ((const u8 *)0x080f39ab)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define REG_DMA3SAD (*(volatile u32 *)0x040000d4)
#define REG_DMA3DAD (*(volatile u32 *)0x040000d8)
#define REG_DMA3CNT (*(volatile u32 *)0x040000dc)

s32 Func_080022ec(s32 value, s32 shift);

/*
 * Build the frame's 120-entry OAM image, then transfer it to object memory.
 * Six ROM bytes select alternating rows of animated tiles.  Coordinates wrap
 * modulo 256 exactly as the reference loops do; unused OAM entries are hidden
 * before the DMA transfers.
 */
void Func_080f2028(void)
{
    u32 temp_r0;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_5;
    s32 temp_r1;
    u32 temp_r1_2;
    u32 temp_r1_3;
    u32 temp_r1_4;
    u32 temp_r1_5;
    u32 temp_r1_6;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_6;
    s32 temp_r3;
    s32 temp_r3_10;
    s32 temp_r3_8;
    s32 temp_r3_9;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 temp_r7_3;
    s32 temp_r7_4;
    s32 temp_r7_5;
    s32 var_r0;
    u32 var_r1;
    u32 var_r2;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r8_2;
    u32 temp_r5;
    u32 entryCount;
    u8 temp_r2_3;
    u8 temp_r2_4;
    u8 temp_r2_5;
    u8 temp_r2_7;
    u8 temp_r2_8;
    u8 temp_r3_2;
    u8 temp_r3_3;
    u8 temp_r3_4;
    u8 temp_r3_5;
    u8 temp_r3_6;
    u8 temp_r3_7;
    u8 *oamWork;
    u8 *oamEntries;

    oamWork = *(u8 **)0x03001EFC;
    entryCount = 0;
    if (*(u8 *)0x03001D20 == 0) {
        temp_r2 = M2C_FIELD(oamWork, s32 *, 0xC) + 1;
        M2C_FIELD(oamWork, s32 *, 0xC) = temp_r2;
        if (!(3 & temp_r2)) {
            M2C_FIELD(oamWork, s32 *, 0x14) = (s32) (M2C_FIELD(oamWork, s32 *, 0x14) + 1);
        }
    }
    temp_r3 = 0x30 - M2C_FIELD((void *)0x03001AD0, u16 *, 6);
    temp_r1 = 0x90 - M2C_FIELD(oamWork, s32 *, 0x14);
    if ((s32) M2C_FIELD(oamWork, s32 *, 8) >= 0x118) {

    } else {
        if (M2C_FIELD(oamWork, s32 *, 0xC) & 1) {
            temp_r3_2 = ANIMATION_ROWS[1];
            temp_r2_2 = temp_r1 - temp_r3;
            var_r5 = (Func_080022ec(temp_r2_2 * (temp_r3_2 - 0x68), 0x50) + temp_r3) - 0x10;
            temp_r7 = temp_r3_2 - 0x10;
            if (var_r5 > 0xFF) {
                do {
                    var_r5 += 0xFFFFFF00;
                } while (var_r5 > 0xFF);
            }
            if (var_r5 < 0) {
                do {
                    var_r5 += 0x100;
                } while (var_r5 < 0);
            }
            temp_r0 = (u32)(temp_r7 + 4) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x18) = (s32) (temp_r0 | var_r5 | 0x40002400);
            temp_r1_2 = (u32)(temp_r7 + 0x14) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x20) = (s32) (temp_r1_2 | var_r5 | 0x50002400);
            temp_r2_3 = var_r5 + 0x10;
            M2C_FIELD(oamWork, s32 *, 0x28) = (s32) (temp_r0 | temp_r2_3 | 0x60002400);
            M2C_FIELD(oamWork, s32 *, 0x30) = (s32) (temp_r1_2 | temp_r2_3 | 0x70002400);
            M2C_FIELD(oamWork, s32 *, 0x1C) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x24) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x2C) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x34) = 0xE8;
            temp_r3_3 = ANIMATION_ROWS[3];
            var_r5_2 = (Func_080022ec(temp_r2_2 * (temp_r3_3 - 0x68), 0x50) + temp_r3) - 0x10;
            temp_r7_2 = temp_r3_3 - 0x10;
            if (var_r5_2 > 0xFF) {
                do {
                    var_r5_2 += 0xFFFFFF00;
                } while (var_r5_2 > 0xFF);
            }
            if (var_r5_2 < 0) {
                do {
                    var_r5_2 += 0x100;
                } while (var_r5_2 < 0);
            }
            temp_r0_2 = (u32)(temp_r7_2 + 4) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x38) = (s32) (temp_r0_2 | var_r5_2 | 0x40002400);
            temp_r1_3 = (u32)(temp_r7_2 + 0x14) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x40) = (s32) (temp_r1_3 | var_r5_2 | 0x50002400);
            temp_r2_4 = var_r5_2 + 0x10;
            M2C_FIELD(oamWork, s32 *, 0x48) = (s32) (temp_r0_2 | temp_r2_4 | 0x60002400);
            M2C_FIELD(oamWork, s32 *, 0x50) = (s32) (temp_r1_3 | temp_r2_4 | 0x70002400);
            M2C_FIELD(oamWork, s32 *, 0x3C) = 0xE0;
            M2C_FIELD(oamWork, s32 *, 0x44) = 0xE0;
            M2C_FIELD(oamWork, s32 *, 0x4C) = 0xE0;
            M2C_FIELD(oamWork, s32 *, 0x54) = 0xE0;
            var_r8_2 = 8;
            temp_r3_4 = ANIMATION_ROWS[5];
            var_r5_3 = (Func_080022ec(temp_r2_2 * (temp_r3_4 - 0x68), 0x50) + temp_r3) - 0x20;
            temp_r7_3 = temp_r3_4 - 0x20;
            if (var_r5_3 > 0xFF) {
                do {
                    var_r5_3 += 0xFFFFFF00;
                } while (var_r5_3 > 0xFF);
            }
            if (var_r5_3 < 0) {
                do {
                    var_r5_3 += 0x100;
                } while (var_r5_3 < 0);
            }
            temp_r0_3 = (u32)(temp_r7_3 + 4) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x58) = (s32) (temp_r0_3 | var_r5_3 | 0x80002400);
            temp_r1_4 = (u32)(temp_r7_3 + 0x24) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x60) = (s32) (temp_r1_4 | var_r5_3 | 0x90002400);
            temp_r2_5 = var_r5_3 + 0x20;
            M2C_FIELD(oamWork, s32 *, 0x68) = (s32) (temp_r0_3 | temp_r2_5 | 0xA0002400);
            var_r1 = temp_r1_4 | temp_r2_5 | 0xB0002400;
            var_r0 = 0x40;
            var_r3 = 0x40;
            var_r2 = 0xA0;
        } else {
            temp_r3_5 = ANIMATION_ROWS[0];
            temp_r2_6 = temp_r1 - temp_r3;
            var_r5_4 = (Func_080022ec(temp_r2_6 * (temp_r3_5 - 0x68), 0x50) + temp_r3) - 0x10;
            temp_r7_4 = temp_r3_5 - 0x10;
            if (var_r5_4 > 0xFF) {
                do {
                    var_r5_4 += 0xFFFFFF00;
                } while (var_r5_4 > 0xFF);
            }
            if (var_r5_4 < 0) {
                do {
                    var_r5_4 += 0x100;
                } while (var_r5_4 < 0);
            }
            temp_r0_4 = (u32)(temp_r7_4 + 4) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x18) = (s32) (temp_r0_4 | var_r5_4 | 0x40002400);
            temp_r1_5 = (u32)(temp_r7_4 + 0x14) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x20) = (s32) (temp_r1_5 | var_r5_4 | 0x50002400);
            temp_r2_7 = var_r5_4 + 0x10;
            M2C_FIELD(oamWork, s32 *, 0x28) = (s32) (temp_r0_4 | temp_r2_7 | 0x60002400);
            M2C_FIELD(oamWork, s32 *, 0x30) = (s32) (temp_r1_5 | temp_r2_7 | 0x70002400);
            M2C_FIELD(oamWork, s32 *, 0x1C) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x24) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x2C) = 0xE8;
            M2C_FIELD(oamWork, s32 *, 0x34) = 0xE8;
            temp_r3_6 = ANIMATION_ROWS[2];
            var_r5_5 = (Func_080022ec(temp_r2_6 * (temp_r3_6 - 0x68), 0x50) + temp_r3) - 0x10;
            if (var_r5_5 > 0xFF) {
                do {
                    var_r5_5 += 0xFFFFFF00;
                } while (var_r5_5 > 0xFF);
            }
            if (var_r5_5 < 0) {
                do {
                    var_r5_5 += 0x100;
                } while (var_r5_5 < 0);
            }
            M2C_FIELD(oamWork, u32 *, 0x38) =
                ((u32)(temp_r3_6 - 0x0c) << 0x10) | var_r5_5 | 0x80002400;
            M2C_FIELD(oamWork, s32 *, 0x3C) = 0x80;
            var_r8_2 = 5;
            temp_r3_7 = ANIMATION_ROWS[4];
            var_r5_6 = (Func_080022ec(temp_r2_6 * (temp_r3_7 - 0x68), 0x50) + temp_r3) - 0x20;
            temp_r7_5 = temp_r3_7 - 0x20;
            if (var_r5_6 > 0xFF) {
                do {
                    var_r5_6 += 0xFFFFFF00;
                } while (var_r5_6 > 0xFF);
            }
            if (var_r5_6 < 0) {
                do {
                    var_r5_6 += 0x100;
                } while (var_r5_6 < 0);
            }
            temp_r0_5 = (u32)(temp_r7_5 + 4) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x40) = (s32) (temp_r0_5 | var_r5_6 | 0x80002400);
            temp_r1_6 = (u32)(temp_r7_5 + 0x24) << 0x10;
            M2C_FIELD(oamWork, s32 *, 0x48) = (s32) (temp_r1_6 | var_r5_6 | 0x90002400);
            temp_r2_8 = var_r5_6 + 0x20;
            M2C_FIELD(oamWork, s32 *, 0x50) = (s32) (temp_r0_5 | temp_r2_8 | 0xA0002400);
            var_r1 = temp_r1_6 | temp_r2_8 | 0xB0002400;
            var_r0 = 0x28;
            var_r3 = 0x28;
            var_r2 = 0xC0;
        }
        temp_r3_8 = var_r3 + 0x1C;
        *(u32 *)(oamWork + var_r0 + 0x30) = var_r1;
        *(u32 *)(oamWork + temp_r3_8) = var_r2;
        temp_r3_9 = temp_r3_8 + 8;
        *(u32 *)(oamWork + temp_r3_9) = var_r2;
        temp_r3_10 = temp_r3_9 + 8;
        *(u32 *)(oamWork + temp_r3_10) = var_r2;
        *(u32 *)(oamWork + temp_r3_10 + 8) = var_r2;
        entryCount = var_r8_2 + 4;
    }
    if (entryCount <= 0x77U) {
        var_r3_2 = (entryCount * 8) + 0x18;
        do {
            entryCount += 1;
            *(u32 *)(oamWork + var_r3_2) = 0x400020a0;
            var_r3_2 += 8;
        } while (entryCount <= 0x77U);
    }
    REG_BLDCNT = 0x3f50;
    REG_BLDALPHA = 0x0e0e;
    temp_r5 = entryCount * 8;
    oamEntries = oamWork + 0x18;
    REG_DMA3SAD = (u32)(unsigned long)oamEntries;
    REG_DMA3DAD = 0x07000000;
    REG_DMA3CNT = (temp_r5 >> 2) | 0x84000000;
    var_r2_2 = 0x20 - M2C_FIELD((void *)0x03001AD0, u16 *, 6);
    if (var_r2_2 > 0xFF) {
        do {
            var_r2_2 += 0xFFFFFF00;
        } while (var_r2_2 > 0xFF);
    }
    if (var_r2_2 < 0) {
        do {
            var_r2_2 += 0x100;
        } while (var_r2_2 < 0);
    }
    M2C_FIELD(oamWork, s32 *, 0x78) = (s32) (var_r2_2 | 0xC05C2000);
    M2C_FIELD(oamWork, s32 *, 0x7C) = 0x800;
    REG_DMA3SAD = (u32)(unsigned long)(oamWork + 0x78);
    REG_DMA3DAD = temp_r5 + 0x07000000;
    REG_DMA3CNT = 0x84000002;
    REG_DMA3SAD = (u32)(unsigned long)oamEntries;
    REG_DMA3DAD = 0x07000000;
    REG_DMA3CNT = 0x84000008;
}
