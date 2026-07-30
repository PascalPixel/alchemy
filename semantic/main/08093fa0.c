#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
void Func_08009080(void *, s32);
void Func_08009150(void *, s32, s32, s32);
void Func_080091e0(void *, u32);
s8 Func_08009220(const s32 *);
void Func_0809163c(s32);
void Func_080916b0(void);
void Func_08091750(void);
u8 *Func_08092054(s32);
void Func_08092158(s32, s32, s32);
void Func_080923c4(s32);

s32 Func_08093fa0(void) {
    s32 temp_r3;
    s32 temp_r5;
    s32 temp_r6;
    s32 temp_r8;
    s32 temp_sl;
    s32 var_r3;
    s32 var_r3_2;
    s8 temp_r0;
    u8 var_fp;
    u8 *temp_r7;
    s32 position[3];

    temp_r7 = Func_08092054(*(s32 *)0x02000434);
    temp_r5 = M2C_FIELD(temp_r7, s16 *, 0xA) & 0xFFF0;
    temp_r6 = M2C_FIELD(temp_r7, s16 *, 0x12) & 0xFFF0;
    var_fp = 1;
    temp_r8 = temp_r5 + 8;
    temp_sl = temp_r6 + 8;
    Func_080916b0();
    if (M2C_FIELD(temp_r7, u8 *, 0x54) == 1) {
        var_fp = M2C_FIELD(M2C_FIELD(temp_r7, void **, 0x50), u8 *, 0x26);
    }
    if (*(u8 *)0x02000432 == 0) {
        var_r3 = temp_r8;
        if (var_r3 < 0) {
            var_r3 = temp_r5 + 0x17;
        }
        var_r3_2 = temp_sl;
        if (var_r3_2 < 0) {
            var_r3_2 = temp_r6 + 0x17;
        }
        temp_r3 = ((var_r3 >> 4) + ((var_r3_2 >> 4) << 7)) * 4;
        if (M2C_FIELD((u8 *)0x02010000 + temp_r3, u8 *, 2) !=
            M2C_FIELD((u8 *)0x0200FE00 + temp_r3, u8 *, 2)) {
            goto block_13;
        }
        position[0] = M2C_FIELD(temp_r7, s32 *, 8);
        position[1] = M2C_FIELD(temp_r7, s32 *, 0xC);
        position[2] = M2C_FIELD(temp_r7, s32 *, 0x10);
        temp_r0 = Func_08009220(position);
        if (temp_r0 == 0) {
            M2C_FIELD(temp_r7, s8 *, 0x5A) = temp_r0;
            Func_08092158(*(s32 *)0x02000434, temp_r8, temp_sl);
            Func_08009080(temp_r7, 6);
            Func_080030f8(4U);
            Func_08009080(temp_r7, 7);
            M2C_FIELD(temp_r7, s32 *, 0x28) = 0x40000;
            Func_080030f8(4U);
            M2C_FIELD(temp_r7, s8 *, 0x55) = temp_r0;
            Func_080091e0(temp_r7, var_fp & 0xFE);
            M2C_FIELD(temp_r7, s32 *, 0x30) = 0x10000;
            M2C_FIELD(temp_r7, s32 *, 0x28) = (s32) temp_r0;
            Func_08009080(temp_r7, 0xC);
            Func_080030f8(4U);
            *(u8 *)0x02000432 = 1;
            M2C_FIELD(temp_r7, s8 *, 0x5A) = 1;
            Func_080030f8(8U);
            goto block_12;
        }
block_13:
        Func_08091750();
        return -1;
    }
    M2C_FIELD(temp_r7, s8 *, 0x55) = 0;
    Func_08009080(temp_r7, 0xB);
    Func_08009150(temp_r7, temp_r8 << 0x10, M2C_FIELD(temp_r7, s32 *, 0xC) + 0x80000, (temp_sl << 0x10) + 0xFFF00000);
    Func_080923c4(*(s32 *)0x02000434);
    M2C_FIELD(temp_r7, s8 *, 0x55) = 3;
    M2C_FIELD(temp_r7, s32 *, 0x14) = (s32) M2C_FIELD(temp_r7, s32 *, 0xC);
    Func_080091e0(temp_r7, var_fp | 1);
    Func_0809163c(4);
    *(u8 *)0x02000432 = 0;
    M2C_FIELD(temp_r7, s8 *, 0x5A) = 1;
block_12:
    Func_08091750();
    return 0;
}
