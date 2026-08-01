typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. This file is
 * still a raw m2c draft; the veneer pass below is the only thing corrected.
 *
 * 0x080d2dea -- `bl 0x080072f0` = __call_via_r3, r3 from pool 0x080d2e1c =
 * 0x03001388, the relocated word copy (destination, source, size) established
 * in the EXACT src/080d40ec.c. r0 = 0x05000000, r1 = the Func_08002f40(0x6e)
 * result, r2 = 0x80. r3 is an argument register, so the draft's fourth
 * argument WAS the callee; the call takes three.
 *
 * THE RENDERER PAIR ARRIVES HERE AS AN OUT-PARAMETER, not as two loads. At
 * 0x080d2dca r0 = sp + 52 and that address is kept in [sp, #36]; then
 * `Func_080cef64(0, sp + 52)` at 0x080d2dd4 fills sp+52 with allocator slot 46
 * and sp+56 with slot 47 -- read straight off the byte-exact src/080cef64.c,
 * whose two stores are `output[0] = *(state + 184)` and
 * `output[1] = *(state + 188)`, i.e. 46 * 4 and 47 * 4 off 0x03001e50. So
 * m2c's single `u32 projection_context` is really a two-element renderer
 * array, and [sp, #36] is its base.
 *
 * 0x080d3108, 0x080d3120, 0x080d3164 -- `bl 0x080072f4` = __call_via_r4, with
 * r4 = `ldr r4, [sp, #52]` immediately before each branch: entry 0, slot 46.
 *
 * 0x080d323c -- r4 = `ldr r4, [r6, r0]` at 0x080d3238, r0 = [sp, #36] (the
 * base) and r6 set at 0x080d320c-0x080d3212 as `r6 = 4; if (r3 <= 0) r6 = 0;`
 * where r3 is field 0x10 of the strip record. So renderers[field_0x10 > 0].
 *
 * AND THAT IS WHERE THE EMPTY IF CAME FROM. The draft carried
 * `if (temp_r3 <= 0) { }` with nothing in the body -- m2c kept the branch and
 * discarded its only effect, because that effect was choosing the callee and
 * the callee register looked dead. The empty block is now the index.
 *
 * ARITY: six at every site. r0..r3 are set and two more words go out at
 * [sp, #0] and [sp, #4]. r4 is above the argument registers, so no argument
 * slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents.
 */
typedef void (*Renderer_080d2d98)(
    s32 target, void *source, s32 x, s32 y, u32 width, s32 height);
typedef void *(*WordCopy_080d2d98)(void *destination, const void *source,
                                   s32 size);

void Func_08002dd8(s32);
void **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd594(s32);
void Func_080d6750(void *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e6d3c(s32, s32, s32);
void Func_080e6eac(s32, s32, s32);

/*
 * Run a long battle particle sequence around a moving scene anchor.
 *
 * The owner seeds and recycles a 1,024-entry particle pool, animates layered
 * strips, drives character hits and sounds, then restores the scene anchor and
 * releases all temporary objects.
 */
void Func_080d2d98(s32 *arg0) {
    s32 sp8;
    u32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    u32 *sp24;
    s32 sp28;
    void *sp2C;
    s32 sp30;
    s32 *var_r3;
    s32 *var_r3_2;
    s32 temp_r0;
    s32 temp_r0_4;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r1_2;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 var_fp;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r7_3;
    s32 var_r8;
    s32 var_r8_10;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r8_4;
    s32 var_r8_5;
    s32 var_r8_6;
    s32 var_r8_7;
    s32 var_r8_8;
    s32 var_r8_9;
    s32 var_r9;
    u16 temp_r0_2;
    u16 temp_r0_3;
    u32 temp_r1;
    void **var_r5_3;
    void *temp_r0_5;
    void *temp_r0_9;
    void *temp_r2;
    void *temp_r5;
    void *var_r5;
    void *var_r5_2;
    void *var_r6;
    void *var_r7;
    void *var_r7_2;
    Renderer_080d2d98 renderers[2];

    sp30 = M2C_FIELD((void *)0x03001EF0, s32 *, 0);
    temp_r2 = *(void **)0x03001EEC;
    sp2C = temp_r2;
    sp28 = M2C_FIELD((void *)0x03001EF0, s32 *, 4);
    M2C_FIELD(temp_r2, s32 **, 0x7828) = arg0;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;
    sp24 = (u32 *)renderers;
    Func_080cef64(0, sp24);
    temp_r0 = Func_08002f40(0x6E);
    ((WordCopy_080d2d98)0x03001388)((void *)0x05000000, temp_r0, 0x80);
    Func_08005340(temp_r0 + 0x80, (s32) sp2C);
    Func_08005340(Func_08002f40(0x85) + 0x80, (s32) (temp_r2 + 0x6E4));
    Func_08005340(Func_08002f40(0x73), sp28);
    M2C_FIELD(sp2C, s32 *, 0x7780) = 2;
    M2C_FIELD(sp2C, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    sp20 = 0x580000;
    sp14 = -0x100000;
    sp18 = -0x40000;
    sp10 = 0;
    sp1C = 0x01000000;
    var_r8 = 0;
    var_r3 = sp2C + 0x7098;
    do {
        var_r8 += 1;
        *var_r3 = -1;
        var_r3 += 0x1C;
    } while (var_r8 != 0x40);
    var_r8_2 = 0;
    var_r5 = sp2C + 0x7320;
    do {
        M2C_FIELD(var_r5, s32 *, 0) = (s32) (0x7F & Func_08004458());
        M2C_FIELD(var_r5, s32 *, 4) = (s32) ((7 & Func_08004458()) + 0x38);
        M2C_FIELD(var_r5, s32 *, 0x18) = (s32) (0 - (0xF & Func_08004458()));
        var_r8_2 += 1;
        var_r5 += 0x1C;
    } while (var_r8_2 != 0x10);
    var_r3_2 = (s32 *)0x02010018;
    var_r8_3 = 0;
    do {
        var_r8_3 += 1;
        *var_r3_2 = -1;
        var_r3_2 += 0x1C;
    } while (var_r8_3 != 0x400);
    Func_080d6750(M2C_FIELD(sp2C, void **, 0x7828));
    Func_080030f8(1U);
    Func_080dbb24(0xC, 0x17C, 2U);
    var_r9 = 0;
loop_8:
    if ((*(s32 *)0x03001B04 & 3) && (var_r9 > 0x20) && (var_r9 <= 0x61)) {
        var_r9 = 0x62;
    }
    if (var_r9 == 0x78) {
        Func_080b50e8(0x86);
    }
    if (var_r9 <= 0xF) {
        sp10 += 2;
    }
    if (var_r9 <= 0x63) {
        sp1C += sp14;
        var_r3_3 = 0x3A * sp14;
        sp20 += sp18;
        if (var_r3_3 < 0) {
            var_r3_3 += 0x3F;
        }
        sp14 = var_r3_3 >> 6;
        var_r3_4 = sp18 * 0x38;
        if (var_r3_4 < 0) {
            var_r3_4 += 0x3F;
        }
        sp18 = var_r3_4 >> 6;
        if (sp1C <= 0x77FFFF) {
            sp14 += 0x8000;
        }
    }
    Func_080e6d3c(1, sp1C, sp20);
    if (var_r9 == 0x1C) {
        var_r7 = (void *)0x02010000;
        var_r8_4 = 0;
        do {
            if (M2C_FIELD(var_r7, s32 *, 0x18) == -1) {
                temp_r6 = Func_08004458() & 0x3F;
                temp_r0_2 = (u16) Func_08004458();
                M2C_FIELD(var_r7, s32 *, 0) = (s32) (((s32) (temp_r6 * Func_08002322((s32) temp_r0_2)) >> 3) + 0x200000);
                M2C_FIELD(var_r7, s32 *, 4) = (s32) (((s32) (temp_r6 * Func_0800231c((s32) temp_r0_2)) >> 2) + 0x600000);
                M2C_FIELD(var_r7, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
                M2C_FIELD(var_r7, s32 *, 0x10) = (s32) (((0 - (Func_08004458() & 0x3F)) - 8) << 0xD);
                M2C_FIELD(var_r7, s32 *, 0x18) = 0;
            }
            var_r8_4 += 1;
            var_r7 += 0x1C;
        } while (var_r8_4 != 0x100);
    }
    temp_r1 = var_r9 - 0x20;
    spC = temp_r1;
    if (temp_r1 <= 0x2FU) {
        var_r7_2 = (void *)0x02010000;
        var_fp = 0;
        var_r8_5 = 0;
loop_30:
        if ((M2C_FIELD(var_r7_2, s32 *, 0x18) != -1) || (temp_r6_2 = Func_08004458() & 0x3F, temp_r0_3 = (u16) Func_08004458(), M2C_FIELD(var_r7_2, s32 *, 0) = (s32) (((s32) (temp_r6_2 * Func_08002322((s32) temp_r0_3)) >> 3) + 0x200000), M2C_FIELD(var_r7_2, s32 *, 4) = (s32) (((s32) (temp_r6_2 * Func_0800231c((s32) temp_r0_3)) >> 2) + 0x600000), M2C_FIELD(var_r7_2, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE), temp_r0_4 = (0 - (Func_08004458() & 0x3F)) - 8, M2C_FIELD(var_r7_2, s32 *, 0x18) = 0, var_fp += 1, M2C_FIELD(var_r7_2, s32 *, 0x10) = (s32) (temp_r0_4 << 0xD), (var_fp != 0x10))) {
            var_r8_5 += 1;
            var_r7_2 += 0x1C;
            if (var_r8_5 != 0x400) {
                goto loop_30;
            }
        }
    }
    if (var_r9 == 0) {
        Func_080f9010(0xA4);
    }
    if (var_r9 == 0x20) {
        Func_080f9010(0x91);
    }
    if (var_r9 == 0x50) {
        Func_080f9010(0x90);
    }
    if (spC <= 0x2FU) {
        temp_r0_5 = sp2C + 0x6E4;
        var_r8_6 = 0;
        var_r6 = (void *)0x080EE1AC;
        var_r7_3 = (var_r9 * 0x10) + 0xFFFFFF00;
        do {
            temp_r0_6 = Func_080022fc(var_r7_3, 0x68);
            renderers[0](sp30, temp_r0_5, M2C_FIELD(var_r6, u8 *, 0) - 0x11, (M2C_FIELD(var_r6, u8 *, 1) - temp_r0_6) - 0x68, 0x22U, 0x68);
            renderers[0](sp30, temp_r0_5, M2C_FIELD(var_r6, u8 *, 0) - 0x11, M2C_FIELD(var_r6, u8 *, 1) - temp_r0_6, 0x22U, temp_r0_6);
            var_r8_6 += 1;
            var_r6 += 2;
            var_r7_3 += 0x19;
        } while (var_r8_6 != 3);
    }
    if (var_r9 <= 0x5F) {
        var_r8_7 = 0;
        do {
            var_r2 = var_r9;
            if (var_r2 < 0) {
                var_r2 += 3;
            }
            renderers[0](sp30, sp2C, ((var_r8_7 << 5) + ((var_r2 >> 2) & 0x1F)) - 0x20, 0x78 - sp10, 0x20U, 0x20);
            var_r8_7 += 1;
        } while (var_r8_7 != 5);
    }
    var_r5_2 = (void *)0x02010000;
    var_r8_8 = 0;
loop_48:
    if ((s32) M2C_FIELD(var_r5_2, s32 *, 0x18) < 0) {

    } else {
        temp_r3 = M2C_FIELD(var_r5_2, s32 *, 0x10);
        var_r4 = Func_080022fc(var_r8_8, 3) + 2;
        if (temp_r3 > 0) {
            var_r4 += 2;
        }
        if ((var_r9 > 0x44) && (var_r4 <= 5)) {
            var_r4 = 6;
        }
        if ((var_r9 > 0x46) && (var_r4 <= 6)) {
            var_r4 = 7;
        }
        if ((var_r9 > 0x48) && (var_r4 <= 7)) {
            var_r4 = 8;
        }
        if ((var_r9 > 0x4A) && (var_r4 <= 8)) {
            var_r4 = 9;
        }
        if (var_r9 > 0x4C) {
            var_r4 = 0xA;
        }
        temp_r0_7 = var_r4 * 2;
        renderers[temp_r3 > 0](sp30, (void *)(sp28 + M2C_FIELD((temp_r0_7 - 2), u16 *, 0x080EDE48)), M2C_FIELD(var_r5_2, s16 *, 2) - ((s32) (var_r4 + ((u32) var_r4 >> 0x1F)) >> 1), M2C_FIELD(var_r5_2, s16 *, 6) - var_r4, (u32) var_r4, temp_r0_7);
        M2C_FIELD(var_r5_2, s32 *, 0) = (s32) (M2C_FIELD(var_r5_2, s32 *, 0) + M2C_FIELD(var_r5_2, s32 *, 0xC));
        temp_r1_2 = M2C_FIELD(var_r5_2, s32 *, 0x10);
        M2C_FIELD(var_r5_2, s32 *, 4) = (s32) (M2C_FIELD(var_r5_2, s32 *, 4) + temp_r1_2);
        if (var_r9 > 0x50) {
            var_r3_5 = temp_r1_2 + 0xFFFF8000;
        } else {
            var_r3_5 = temp_r1_2 + M2C_FIELD(((3 & var_r8_8) * 4), s32 *, 0x080EE1B4);
        }
        M2C_FIELD(var_r5_2, s32 *, 0x10) = var_r3_5;
        var_r3_6 = M2C_FIELD(var_r5_2, s32 *, 0xC) * 0x3E;
        if (var_r3_6 < 0) {
            var_r3_6 += 0x3F;
        }
        M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) (var_r3_6 >> 6);
        var_r2_2 = M2C_FIELD(var_r5_2, s32 *, 0x10) * 0x3E;
        if (var_r2_2 < 0) {
            var_r2_2 += 0x3F;
        }
        temp_r2_2 = var_r2_2 >> 6;
        M2C_FIELD(var_r5_2, s32 *, 0x10) = temp_r2_2;
        M2C_FIELD(var_r5_2, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_2, s32 *, 0x18) + 1);
        if ((temp_r2_2 > 0) && ((s32) M2C_FIELD(var_r5_2, s16 *, 6) > 0x68)) {
            M2C_FIELD(var_r5_2, s32 *, 0x18) = -1;
        }
    }
    var_r8_8 += 1;
    var_r5_2 += 0x1C;
    if (var_r8_8 != 0x400) {
        goto loop_48;
    }
    if (var_r9 <= 0x4F) {
        var_r8_9 = 0;
        if (M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r4_2 = 0x24;
            do {
                if (var_r9 > 0x1D) {
                    sp8 = var_r4_2;
                    temp_r0_8 = Func_080022fc(var_r9, 0xC);
                    if (temp_r0_8 == 0) {
                        temp_r5 = *Func_080b5098((s32) M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s16 *, var_r4_2));
                        Func_080d6888(M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s16 *, sp8), 7, 5, -1, temp_r0_8);
                        M2C_FIELD(temp_r5, s32 *, 0x28) = 0x48000;
                        var_r4_2 = sp8;
                        M2C_FIELD(temp_r5, s32 *, 0x48) = 0xAB85;
                    }
                    if (temp_r0_8 == 6) {
                        sp8 = var_r4_2;
                        Func_080d6888(M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s16 *, var_r4_2), 0, 5, -1, 0);
                    }
                }
                var_r8_9 += 1;
                var_r4_2 += 2;
            } while (var_r8_9 != M2C_FIELD(M2C_FIELD(sp2C, void **, 0x7828), s32 *, 0x14));
        }
    }
    M2C_FIELD(sp2C, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    var_r9 += 1;
    if (var_r9 != 0x7C) {
        goto loop_8;
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080e6eac(1, sp1C, sp20);
    var_r8_10 = 0;
    var_r5_3 = sp2C + 0x77D8;
    do {
        temp_r0_9 = *var_r5_3;
        var_r5_3 += 4;
        Func_08009038(temp_r0_9);
        var_r8_10 += 1;
    } while (var_r8_10 != 0xC);
    Func_080cdbc0();
}
