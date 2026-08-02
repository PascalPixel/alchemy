#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct Output_0807905c {
    s16 index;
    s16 sentinel;
    u16 values[6];
};

s32 Func_080022ec(s32, s32);
s32 Func_080022f4(s32, s32);
void Func_08002df0(void *);
u32 Func_08004458(void);
u8 *Func_08004970(s32);
u8 *Func_08077394(s32);
void Func_08077428(s32);
void Func_08078bf0(s32);
u8 *Func_08078ed8(s32);
u32 Func_08079008(s32, s32);

s32 Func_0807905c(s32 arg0, struct Output_0807905c *arg1) {
    u8 *temp_r0;
    s16 var_r5;
    s32 temp_r0_4;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 temp_r7_3;
    s32 temp_r7_4;
    s32 temp_r7_5;
    s32 temp_r7_6;
    u8 *temp_sl;
    u16 temp_r1_3;
    u32 temp_r0_2;
    u8 temp_r1;
    u8 *temp_r0_3;
    u8 *temp_r1_2;
    u8 *temp_r2;
    u8 *temp_r2_2;
    u8 *temp_r2_3;
    u8 *temp_r2_4;
    u8 *temp_r2_5;

    temp_sl = Func_08077394(arg0);
    temp_r0 = Func_08004970(0x2C);
    M2C_FIELD(temp_r0, s32 *, 0) = (s32) M2C_FIELD(temp_sl, u8 *, 0x129);
    temp_r1 = M2C_FIELD(temp_sl, u8 *, 0xF);
    M2C_FIELD(temp_r0, s32 *, 4) = (s32) temp_r1;
    arg1->index = (s16) temp_r1;
    arg1->sentinel = 0xFFFF;
    arg1->values[0] = 0U;
    arg1->values[1] = 0U;
    arg1->values[2] = 0U;
    arg1->values[3] = 0U;
    arg1->values[4] = 0U;
    arg1->values[5] = 0U;
    if ((s32) temp_r1 > 0x62) {

    } else {
        M2C_FIELD(temp_sl, u8 *, 0xF) = (u8) (M2C_FIELD(temp_sl, u8 *, 0xF) + 1);
        arg1->index = (s16) (temp_r1 + 1);
        temp_r0_2 = Func_08079008(arg0, (s32) M2C_FIELD(temp_sl, u8 *, 0xF));
        if ((temp_r0_2 != -1U) && ((u32) M2C_FIELD(temp_sl, u32 *, 0x124) < temp_r0_2)) {
            M2C_FIELD(temp_sl, u32 *, 0x124) = temp_r0_2;
        }
        temp_r0_3 = Func_08078ed8(arg0);
        M2C_FIELD(temp_r0, void **, 8) = temp_r0_3;
        if (arg1->index == 1) {
            arg1->values[0] = (u16) (arg1->values[0] + M2C_FIELD(temp_r0_3, u16 *, 0x50));
            arg1->values[1] = (u16) (arg1->values[1] + M2C_FIELD(temp_r0_3, u16 *, 0x5C));
            arg1->values[2] = (u16) (arg1->values[2] + M2C_FIELD(temp_r0_3, u16 *, 0x68));
            arg1->values[3] = (u16) (arg1->values[3] + M2C_FIELD(temp_r0_3, u16 *, 0x74));
            arg1->values[4] = (u16) (arg1->values[4] + M2C_FIELD(temp_r0_3, u16 *, 0x80));
            arg1->values[5] = (u16) (arg1->values[5] + M2C_FIELD(temp_r0_3, u8 *, 0x8C));
        }
        var_r5 = (s16) Func_080022ec((s32) (s16) (u16) arg1->index, 0x14);
        if ((s32) var_r5 < 0) {
            var_r5 = 0;
        }
        if ((s32) var_r5 > 4) {
            var_r5 = 4;
        }
        temp_r0_4 = var_r5 * 2;
        temp_r1_2 = M2C_FIELD(temp_r0, void **, 8);
        temp_r3 = temp_r0_4 + 0x52;
        temp_r7 = M2C_FIELD(temp_r1_2, s16 *, temp_r3) -
            M2C_FIELD(temp_r1_2, s16 *, temp_r3 - 2);
        arg1->values[0] = (u16) (arg1->values[0] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7, 0x14));
        temp_r2 = M2C_FIELD(temp_r0, void **, 8);
        temp_r3_2 = temp_r0_4 + 0x5E;
        temp_r7_2 = M2C_FIELD(temp_r2, s16 *, temp_r3_2) -
            M2C_FIELD(temp_r2, s16 *, temp_r3_2 - 2);
        arg1->values[1] = (u16) (arg1->values[1] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7_2, 0x14));
        temp_r2_2 = M2C_FIELD(temp_r0, void **, 8);
        temp_r3_3 = temp_r0_4 + 0x6A;
        temp_r7_3 = M2C_FIELD(temp_r2_2, u16 *, temp_r3_3) -
            M2C_FIELD(temp_r2_2, u16 *, temp_r3_3 - 2);
        temp_r2_3 = M2C_FIELD(temp_r0, void **, 8);
        arg1->values[2] = (u16) (arg1->values[2] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7_3, 0x14));
        temp_r3_4 = temp_r0_4 + 0x76;
        temp_r7_4 = M2C_FIELD(temp_r2_3, u16 *, temp_r3_4) -
            M2C_FIELD(temp_r2_3, u16 *, temp_r3_4 - 2);
        arg1->values[3] = (u16) (arg1->values[3] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7_4, 0x14));
        temp_r2_4 = M2C_FIELD(temp_r0, void **, 8);
        temp_r3_5 = temp_r0_4 + 0x82;
        temp_r7_5 = M2C_FIELD(temp_r2_4, u16 *, temp_r3_5) -
            M2C_FIELD(temp_r2_4, u16 *, temp_r3_5 - 2);
        temp_r2_5 = M2C_FIELD(temp_r0, void **, 8);
        arg1->values[4] = (u16) (arg1->values[4] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7_5, 0x14));
        temp_r3_6 = var_r5 + 0x8D;
        temp_r7_6 = M2C_FIELD(temp_r2_5, u8 *, temp_r3_6) -
            M2C_FIELD(temp_r2_5, u8 *, temp_r3_6 - 1);
        temp_r1_3 = arg1->values[5] + Func_080022f4(((u32) (Func_08004458() * 0x14) >> 0x10) + temp_r7_6, 0x14);
        M2C_FIELD(temp_sl, u16 *, 0x10) = (u16) (M2C_FIELD(temp_sl, u16 *, 0x10) + M2C_FIELD(arg1, u16 *, 4));
        M2C_FIELD(temp_sl, u16 *, 0x12) = (u16) (M2C_FIELD(temp_sl, u16 *, 0x12) + M2C_FIELD(arg1, u16 *, 6));
        M2C_FIELD(temp_sl, u16 *, 0x18) = (u16) (M2C_FIELD(temp_sl, u16 *, 0x18) + M2C_FIELD(arg1, u16 *, 8));
        M2C_FIELD(temp_sl, u16 *, 0x1A) = (u16) (M2C_FIELD(temp_sl, u16 *, 0x1A) + M2C_FIELD(arg1, u16 *, 0xA));
        M2C_FIELD(temp_sl, u16 *, 0x1C) = (u16) (M2C_FIELD(temp_sl, u16 *, 0x1C) + M2C_FIELD(arg1, u16 *, 0xC));
        M2C_FIELD(temp_sl, u8 *, 0x1E) = (u8) (M2C_FIELD(temp_sl, u8 *, 0x1E) + temp_r1_3);
        M2C_FIELD(temp_sl, s8 *, 0x1F) = 1;
        M2C_FIELD(temp_sl, s8 *, 0x20) = 0;
        arg1->values[5] = temp_r1_3;
        M2C_FIELD((temp_sl + 0x20), s8 *, 1) = 0;
        Func_08078bf0(arg0);
        Func_08077428(arg0);
    }
    Func_08002df0(temp_r0);
    return (s32) arg1;
}
