typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015080(s32, s32, s32, s32);
void Func_080a1a40(s32, s32);
void Func_080a7850(void);
s32 Func_080a7f44(s32, s32);

/* Runs the cyclic entry-selection menu and commits the selected entry. */
s32 Func_080a7a34(void) {
    s32 sp4;
    s32 sp8;
    s16 *var_r3_2;
    s16 *var_r3_3;
    s16 *var_r3_4;
    s16 var_r1;
    s32 temp_r0;
    s32 temp_r2;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r6;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r2_4;
    s32 var_r7;
    s32 var_r7_2;
    s8 temp_fp;
    s8 var_sl;
    u16 var_r9;
    u8 *temp_r3;
    void *temp_r3_2;
    void *temp_r3_3;
    void *temp_r3_4;
    u8 *var_r3;

    temp_r3 = *(void **)0x03001F2C;
    sp8 = 1;
    var_sl = M2C_FIELD(temp_r3, s8 *, 0x1C);
    sp4 = 0;
    temp_fp = M2C_FIELD(temp_r3, s8 *, 0x1E);
    var_r9 = M2C_FIELD(temp_r3, u16 *, 0x220);
    Func_08077008(
        (s32)M2C_FIELD(temp_r3, u16 *, (var_sl * 2) + 0x208));
    var_r3 = temp_r3 + 0x234;
    var_r1 = 0x82;
    var_r2 = 3;
    do {
        var_r2 -= 1;
        M2C_FIELD(var_r3, s16 *, 0) = var_r1;
        M2C_FIELD(var_r3, s16 *, 8) = 0x80;
        var_r1 += 0x20;
        var_r3 += 2;
    } while (var_r2 >= 0);
    Func_080a2144(0xE);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = 0x05000200;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x05000000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x80000010;
    temp_r3_2 = (void *)0x040000D4;
    M2C_FIELD(temp_r3_2, s32 *, 0) = 0x050001C8;
    M2C_FIELD(temp_r3_2, s32 *, 4) = 0x0500001C;
    M2C_FIELD(temp_r3_2, s32 *, 8) = 0x80000001;
    temp_r3_3 = (void *)0x040000D4;
    M2C_FIELD(temp_r3_3, s32 *, 0) = 0x05000200;
    M2C_FIELD(temp_r3_3, s32 *, 4) = 0x05000020;
    M2C_FIELD(temp_r3_3, s32 *, 8) = 0x80000010;
    temp_r3_4 = (void *)0x040000D4;
    M2C_FIELD(temp_r3_4, s32 *, 0) = 0x050001E8;
    M2C_FIELD(temp_r3_4, s32 *, 4) = 0x0500003C;
    M2C_FIELD(temp_r3_4, s32 *, 8) = 0x80000001;
loop_37:
    if (Func_080770c0(0x150) == 0) {
        if (sp8 != 0) {
            sp8 = 0;
            Func_08015270(M2C_FIELD(temp_r3, s32 *, 0x10C));
            Func_08015080(0xB0D, M2C_FIELD(temp_r3, s32 *, 0x10C), 0, 0);
            if (Func_080770c0(0x30) != 0) {
                Func_08015080(0xB16, M2C_FIELD(temp_r3, s32 *, 0x10C), 0, 0x10);
            }
            Func_08015080(0xB0A, M2C_FIELD(temp_r3, s32 *, 0x10C), 0, 8);
            temp_r0 = Func_080022fc(var_sl + temp_fp, (s32) temp_fp);
            var_r7 = temp_r0 * 2;
            temp_r5 = var_r7 + 0x208;
            var_sl = (s8) temp_r0;
            Func_08077008((s32)M2C_FIELD(temp_r3, u16 *, temp_r5));
            var_r9 = (u16) Func_080022fc(var_r9 + 3, 3);
            Func_080a8088((s32)M2C_FIELD(temp_r3, u16 *, temp_r5));
            Func_080a1804(
                (s32)temp_r3, (s32)M2C_FIELD(temp_r3, u16 *, temp_r5));
            var_r2_2 = 7;
            var_r3_2 = (s16 *)(temp_r3 + 0x152);
            do {
                var_r2_2 -= 1;
                *var_r3_2 = 0x1E;
                var_r3_2 -= 1;
            } while (var_r2_2 >= 0);
            *(temp_r3 + (var_r7 + 0x144)) = 0x1A;
        } else {
            var_r7 = var_sl * 2;
        }
        Func_080a1a40(((var_r7 + var_sl) * 8) - 0xA, 0x10);
        Func_080030f8(1U);
        if (*(s32 *)0x03001C94 & 1) {
            Func_080f9010(0x70);
            sp4 = 1;
        } else {
            temp_r6 = *(s32 *)0x03001C94 & 2;
            if (temp_r6 != 0) {
                Func_080f9010(0x71);
                sp4 = -1;
            } else {
                temp_r5_2 = *(s32 *)0x03001B04 & 0x100;
                if (temp_r5_2 != 0) {
                    if (Func_080a7f44((s32) var_sl, 1) != 0) {
                        Func_080f9010(0x70);
                        var_sl += 1;
                        Func_080a195c();
                        Func_080a1870(M2C_FIELD(temp_r3, s32 *, 0x10), 2, 2, 8, temp_r6);
                        var_r2_3 = 7;
                        var_r3_3 = (s16 *)(temp_r3 + 0x152);
                        do {
                            var_r2_3 -= 1;
                            *var_r3_3 = 0x1E;
                            var_r3_3 -= 1;
                        } while (var_r2_3 >= 0);
                        var_r7_2 = var_sl * 2;
                        goto block_25;
                    }
                    goto block_26;
                    goto block_27;
                }
                if (*(s32 *)0x03001B04 & 0x200) {
                    if (Func_080a7f44((s32) var_sl, 0) != 0) {
                        Func_080f9010(0x70);
                        var_sl -= 1;
                        Func_080a195c();
                        Func_080a1870(M2C_FIELD(temp_r3, s32 *, 0x10), 2, 2, 8, temp_r5_2);
                        var_r2_4 = 7;
                        var_r3_4 = (s16 *)(temp_r3 + 0x152);
                        do {
                            var_r2_4 -= 1;
                            *var_r3_4 = 0x1E;
                            var_r3_4 -= 1;
                        } while (var_r2_4 >= 0);
                        var_r7_2 = var_sl * 2;
block_25:
                        *(temp_r3 + (var_r7_2 + 0x144)) = 0x1A;
                    } else {
block_26:
                        Func_080f9010(0x72);
                    }
block_27:
                    Func_080030f8(1U);
                } else if ((*(s32 *)0x03001C94 & 4) && (Func_080770c0(0x30) != 0)) {
                    Func_080a7850();
                    sp8 = 1;
                } else {
                    if (*(s32 *)0x03001B04 & 0x20) {
                        Func_080f9010(0x6F);
                        if ((s32) temp_fp > 1) {
                            sp8 = 1;
                            var_sl -= 1;
                        }
                    }
                    if (*(s32 *)0x03001B04 & 0x10) {
                        Func_080f9010(0x6F);
                        if ((s32) temp_fp > 1) {
                            sp8 = 1;
                            var_sl += 1;
                        }
                    }
                }
                goto loop_37;
            }
        }
    } else {
        var_r7 = var_sl * 2;
    }
    M2C_FIELD(temp_r3, s8 *, 0x1C) = var_sl;
    temp_r2 = var_r7 + 0x208;
    M2C_FIELD(temp_r3, s32 *, 8) =
        (s32)M2C_FIELD(temp_r3, u16 *, temp_r2);
    M2C_FIELD(temp_r3, s8 *, 0x21A) =
        (s8)M2C_FIELD(temp_r3, u16 *, temp_r2);
    return sp4;
}
