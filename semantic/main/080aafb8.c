typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_08077208(u32, u32, s32);
s32 Func_08077210(u32, u32, s32);
s8 Func_080ac8fc(u16 *, u16, s32);

/* Builds and renders the categorized ability grid for each active entry. */
void Func_080aafb8(u16 *arg0) {
    u16 *sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    void *sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    u16 *sp30;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 var_r8;
    s32 var_sl;
    u16 *var_r5;
    u16 *var_r5_2;
    u16 *var_r6;
    u8 *var_r7;
    u16 temp_r2;
    u16 temp_r3_2;
    u16 temp_r4;
    void *temp_r0;
    void *temp_r3;

    sp30 = arg0;
    temp_r0 = *(void **)0x03001F2C;
    temp_r3 = *(void **)0x03001E8C;
    sp20 = temp_r3;
    M2C_FIELD(temp_r3, s8 *, 0xEA6) = 1;
    sp2C = 0;
    if (M2C_FIELD(temp_r0, u8 *, 0x219) != 0) {
        var_r5 = sp30;
        var_r7 = (u8 *)sp30 + 0xA0;
        var_r6 = (u16 *)((u8 *)temp_r0 + 0x208);
        do {
            *var_r7 = Func_080ac8fc(var_r5, *var_r6, -1);
            sp2C += 1;
            var_r6 += 2;
            var_r7 += 1;
            var_r5 += 0xA;
        } while (sp2C < (s32) M2C_FIELD(temp_r0, u8 *, 0x219));
    }
    Func_08015270(M2C_FIELD(temp_r0, s32 *, 0x30));
    Func_08015080(0xBAD, M2C_FIELD(temp_r0, s32 *, 0x30), 0, 0x50);
    sp28 = 0;
    if ((s32) M2C_FIELD(temp_r0, u8 *, 0x219) <= 0) {

    } else {
        sp10 = 0xA0;
        spC = 0;
        sp8 = 0;
        sp4 = sp30;
loop_6:
        sp1C = 0;
        sp24 = 0;
loop_7:
        sp2C = 0;
        if ((s32)*(s8 *)((u8 *)sp30 + sp10) <= 0) {

        } else {
            sp14 = spC;
            sp18 = sp8;
            var_r5_2 = sp4;
            var_sl = (sp1C * 8) + 0x10;
loop_10:
            temp_r4 = *var_r5_2;
            if (sp24 == ((u32) (0xE0 & temp_r4) >> 5)) {
                if (!(0x8000 & temp_r4)) {
                    Func_080150b8(2);
                }
                var_r8 = 0;
                if ((Func_08077210((u32) (0xF00 & *var_r5_2) >> 8, (u32) (0xE0 & *var_r5_2) >> 5, 0x1F & *var_r5_2) != 0) || (temp_r3_2 = *var_r5_2, (Func_08077208((u32) (0xF00 & temp_r3_2) >> 8, (u32) (0xE0 & temp_r3_2) >> 5, 0x1F & temp_r3_2) != 0))) {
                    var_r8 = 1;
                }
                if (var_r8 == 0) {
                    Func_080150b8(4);
                }
                Func_08015280(M2C_FIELD(temp_r0, s32 *, 0x30), ((u32) (0xE0 & *var_r5_2) >> 5) + 0x5001, sp18 + 1, sp1C + 2, 0);
                temp_r2 = *var_r5_2;
                Func_08015080((((u32) (0xE0 & temp_r2) >> 5) * 0x14) + (0x1F & temp_r2) + 0x45F, M2C_FIELD(temp_r0, s32 *, 0x30), sp14 + 0x10, var_sl);
                var_sl += 8;
                sp1C += 1;
                Func_080150b8(0xF);
            }
            temp_r2_2 = sp2C + 1;
            sp2C = temp_r2_2;
            var_r5_2 += 2;
            if (temp_r2_2 < (s32)*(s8 *)((u8 *)sp30 + sp10)) {
                goto loop_10;
            }
        }
        temp_r2_3 = sp24 + 1;
        sp24 = temp_r2_3;
        if (temp_r2_3 <= 3) {
            goto loop_7;
        }
        sp10 += 1;
        sp28 += 1;
        spC += 0x38;
        sp8 += 7;
        sp4 += 0xA;
        if (sp28 < (s32) M2C_FIELD(temp_r0, u8 *, 0x219)) {
            goto loop_6;
        }
    }
    Func_08015070(M2C_FIELD(temp_r0, s32 *, 0x30), 0, 0xA, 0x1C, 0xA);
    M2C_FIELD(*(void **)0x03001E8C, s8 *, 0xEA3) = 1;
    M2C_FIELD(sp20, s8 *, 0xEA6) = 0;
}
