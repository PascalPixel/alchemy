typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Six `bl`
 * sites land in the 0x080072e4 bank: two __call_via_r3 and four
 * __call_via_r4.
 *
 * 0x080e2ab4 -- pool 0x080e2dec = 0x03001388, the IWRAM word copy.
 * 0x080e2d88 -- pool 0x080e2e08 = 0x03000168, the IWRAM ARM fill; its fill
 * value is the SEPARATE pool word 0x080e2e0c = 0x2f2f2f2f, which is why two
 * pool loads sit next to each other there. r3 is an ARGUMENT register at
 * both, so the draft's fourth argument WAS the callee and each takes three.
 *
 * THE OTHER FOUR ARE THE FRAME-LOCAL TWO-ENTRY TABLE, and the draft had
 * already found the array without knowing what it held: `loaded_handles` at
 * sp + 88 is it. At 0x080e2b8c-0x080e2b98 `mov r0, sp; adds r0, #88` parks
 * the ADDRESS sp + 88 in [sp, #28]; entry 0 is written from
 * `[0x03001e50 + 0xb8]` = 0x03001f08 and entry 1 from pool 0x03001f0c via
 * `str r3, [r0, #4]`. m2c split that array into `sp58` and `sp1C`, so the
 * same two words appeared as a scalar and a pointer; they are one object.
 *
 * TWO ERAS, both single-publish, and the first one only looks otherwise.
 * Group one issues FOUR Func_080ed408 calls -- 0x080e2bdc (46), 0x080e2bec
 * (47), 0x080e2bfe (46), 0x080e2c0e (47) -- but 0x080e2bf0 is `b.n
 * 0x080e2c12`, so they are two ALTERNATIVE arms of one if/else and each path
 * publishes each slot exactly once. Both reads happen after the join at
 * 0x080e2c12 and nothing republishes before the releases at 0x080e2ed8 and
 * 0x080e2ede. Era two is publishes 0x080e2ef8 / 0x080e2f08, reads at
 * 0x080e2f10 / 0x080e2f18, releases 0x080e2fa8 / 0x080e2fae. Counted over
 * every Func_080ed408 in 0x080e2974..0x080e302c, not sampled.
 *
 * Both eras sit INSIDE the outer loop whose back edge is `b.n 0x080e2ba6` at
 * 0x080e2fe4, so each pass republishes and re-reads. Assigning the locals
 * inside the loop body is therefore the faithful shape, not a hoist.
 *
 * STRUCTURE CORRECTED at three sites. The draft called one renderer where the
 * ROM indexes the table:
 *   0x080e2d2a and 0x080e2d62 -- `ldr r4, [r4, r5]` with r5 = [sp, #28] and
 *   r4 = (r8 & 1) << 2, r8 being the group counter. Renderer parity.
 *   0x080e2f70 -- `ldr r4, [r0, r5]` with r0 = (((r8 + (r8 >>> 31)) >> 1) & 1)
 *   << 2, so this one alternates on HALF the counter, not on the counter.
 *   Two different index expressions in one function; neither is the other.
 * 0x080e2ebc is the only one that is plain entry 0, read straight from
 * [sp, #88] at 0x080e2eb8.
 *
 * ARITY: six at the renderer sites; r4 is above the argument registers.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here, and nothing here says the two eras hold the same pair.
 */
typedef void *(*WordCopy_080e2974)(void *destination, const void *source,
                                   s32 size);
typedef void (*ArmFill_080e2974)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080e2974)(s32 target, void *source, s32 x, s32 y,
                                  u32 width, s32 height);

void Func_08002dd8(s32);
void Func_080b5088(s16, s32);
void **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080d6888(s16, s32, s32, s32, s32);

/*
 * Run a configurable battle particle scene and its character-hit sequence.
 *
 * The selected table row controls palette, sprite groups, timing, and burst
 * counts. The scene builds a large particle pool, stages each hit group, then
 * advances and renders the remaining particles through cleanup.
 */
s32 Func_080e2974(void *arg0, s32 arg1) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    void *sp18;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    void *sp2C;
    void **sp30;
    s32 sp34;
    void *sp38;
    s32 sp3C;
    s32 sp40;
    void *sp44;
    s32 sp48;
    s32 sp4C;
    void *sp50;
    s32 sp54;
    s32 sp60;
    s32 sp64;
    s32 sp68[3];
    s32 primary_position[2];
    s32 target_position[2];
    Renderer_080e2974 renderers[2];
    s32 *var_r5_3;
    s32 temp_r0_2;
    s32 temp_r2_10;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r3;
    s32 temp_r4;
    s32 temp_r5_2;
    s32 temp_r5_4;
    s32 var_r0_2;
    s32 var_r1_3;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r5;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r7;
    s32 var_r8;
    s32 var_r8_2;
    u32 temp_r2_4;
    u32 temp_r2_5;
    u32 temp_r2_6;
    u32 temp_r2_7;
    u32 temp_r2_8;
    u32 temp_r2_9;
    u32 temp_r3_5;
    u32 temp_r3_7;
    u32 temp_r3_8;
    u32 temp_r5_3;
    u32 var_r0;
    u32 var_r8_3;
    u8 *var_r1;
    u8 temp_r2;
    u8 temp_r3_2;
    void *temp_r0;
    void *temp_r3_3;
    void *temp_r3_4;
    s32 temp_r3_6;
    void *temp_r5;
    void *temp_r6;
    void *var_r1_2;
    void *var_r5_2;
    void *var_r6_3;

    sp54 = arg1;
    temp_r0 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp50 = temp_r0;
    sp4C = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp44 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    sp40 = *(s32 *)0x03001E80;
    M2C_FIELD(temp_r0, void **, 0x7828) = arg0;
    Func_080cd594(0);
    temp_r5 = M2C_FIELD(temp_r0, void **, 0x7828);
    *(s16 *)0x04000052 = 0x1010;
    if (M2C_FIELD(temp_r5, s32 *, 0x1C) == 1) {
        Func_080de2f8(arg0, 7, M2C_FIELD(temp_r5, s32 *, 4), 2, &sp64, &sp60);
    }
    Func_080e0524((void *)0x73, sp44, 0, 0);
    Func_080e0524((void *)0x99, sp50, 1, 0);
    var_r5 = 0;
    var_r7 = 0;
    var_r6 = 0;
    do {
        var_r0 = 0;
        var_r1 = ((var_r6 + var_r5) * 8) + sp50;
loop_5:
        temp_r2 = *var_r1;
        temp_r3 = ((s32) (var_r0 + (var_r0 >> 0x1F)) >> 1) + var_r7 + 0x5100;
        var_r0 += 1;
        var_r1 += 1;
        M2C_FIELD(sp50, u8 *, temp_r3) = temp_r2;
        if (var_r0 != 0x28) {
            goto loop_5;
        }
        var_r5 += 1;
        var_r7 += 0x14;
        var_r6 += 4;
    } while (var_r5 != 0x120);
    sp3C = sp54 * 8;
    if (M2C_FIELD((sp54 * 7), u8 *, 0x080EED3E) == 0) {
        Func_080e0524((void *)0xB5, sp50, 1, 1);
    } else {
        Func_080e0524((void *)0xB6, sp50, 1, 1);
    }
    Func_080e0524((void *)0x6B, (void *)0x02015E00, 1, 0);
    temp_r3_2 = M2C_FIELD(((sp3C - sp54) + 1), u8 *, 0x080EED3E);
    switch (temp_r3_2) {
    case 0:
        var_r0_2 = 0x8D;
        break;
    case 1:
        var_r0_2 = 0xA0;
        break;
    case 2:
        var_r0_2 = 0xB6;
        break;
    default:
        var_r0_2 = 0xB4;
        break;
    }
    ((WordCopy_080e2974)0x03001388)((void *)0x05000000, Func_08002f40(var_r0_2), 0x80);
    Func_080030f8(1U);
    temp_r3_3 = primary_position;
    sp38 = temp_r3_3;
    Func_080e396c((s32) M2C_FIELD(M2C_FIELD(sp50, void **, 0x7828), s16 *, 0x24), temp_r3_3);
    M2C_FIELD(sp50, s32 *, 0x7780) = 2;
    M2C_FIELD(sp50, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    var_r5_2 = (void *)0x02010000;
    temp_r6 = *Func_080b5098((s32) M2C_FIELD(M2C_FIELD(sp50, void **, 0x7828), s16 *, 0x24));
    var_r8 = 0;
    do {
        M2C_FIELD(var_r5_2, s32 *, 0) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(var_r5_2, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + 0x190000);
        M2C_FIELD(var_r5_2, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
        M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) ((u8) Func_08004458() << 0xC);
        M2C_FIELD(var_r5_2, s32 *, 0x10) = (s32) (((u8) Func_08004458() - 0x7F) << 0xC);
        M2C_FIELD(var_r5_2, s32 *, 0x14) = (s32) (((u8) Func_08004458() - 0x7F) << 0xC);
        if ((s32) M2C_FIELD(var_r5_2, s32 *, 0) > 0) {
            M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) (0 - M2C_FIELD(var_r5_2, s32 *, 0xC));
        }
        var_r8 += 1;
        M2C_FIELD(var_r5_2, s32 *, 0x18) = -1;
        var_r5_2 += 0x1C;
    } while (var_r8 != 0x300);
    temp_r3_4 = M2C_FIELD(sp50, void **, 0x7828);
    Func_080b5078(M2C_FIELD(temp_r3_4, s32 *, 8), M2C_FIELD(temp_r3_4, s16 *, 0x24), 4, 0);
    sp48 = 0;
    if (M2C_FIELD(((sp3C - sp54) + 5), u8 *, 0x080EED3E) == 0) {

    } else {
        sp18 = target_position;
        sp14 = sp40 + 0xC;
        sp30 = sp50 + 0x7828;
        sp2C = (void *)0x03001E50;
loop_27:
        sp34 = (s32) M2C_FIELD(((sp3C - sp54) + 2), u8 *, 0x080EED3E);
        Func_080e396c(M2C_FIELD(*sp30, s32 *, 8), sp18);
        temp_r3_5 = M2C_FIELD(sp18, u32 *, 0);
        M2C_FIELD(sp18, u32 *, 0) = (u32) ((s32) (temp_r3_5 + (temp_r3_5 >> 0x1F)) >> 1);
        if (M2C_FIELD(*sp30, s32 *, 4) == 0) {
            Func_080ed408(0x2E, 7, 7, 3, 2);
            Func_080ed408(0x2F, 7, 7, 0xB, 2);
        } else {
            Func_080ed408(0x2E, 7, 7, 7, 2);
            Func_080ed408(0x2F, 7, 7, 0xF, 2);
        }
        renderers[0] = *(Renderer_080e2974 *)0x03001F08;
        renderers[1] = *(Renderer_080e2974 *)0x03001F0C;
        var_r8_2 = 0;
        if (sp34 == 0) {

        } else {
            temp_r3_6 = sp3C - sp54;
            sp28 = temp_r3_6;
            sp24 = temp_r3_6 + 6;
            sp20 = sp28 + 3;
            sp10 = 0;
loop_33:
            temp_r2_2 = var_r8_2 * M2C_FIELD((sp28 + 4), u8 *, 0x080EED3E);
            if (sp48 < temp_r2_2) {

            } else if (sp48 < (s32) (temp_r2_2 + 6)) {
                temp_r2_3 = sp48 - temp_r2_2;
                if (((s32) (3 & var_r8_2) <= 1) || (M2C_FIELD(sp28, u8 *, 0x080EED3E) == 1)) {
                    if (M2C_FIELD(*sp30, s32 *, 4) == 0) {
                        var_r1_2 = sp50 + (temp_r2_3 * 0xD80);
                        temp_r2_4 = M2C_FIELD(sp38, u32 *, 0);
                        var_r2 = ((s32) (temp_r2_4 + (temp_r2_4 >> 0x1F)) >> 1) - 0x10;
                        var_r3 = M2C_FIELD(sp38, s32 *, 4) - 0x28;
                    } else {
                        var_r1_2 = sp50 + (temp_r2_3 * 0xD80);
                        temp_r2_5 = M2C_FIELD(sp38, u32 *, 0);
                        var_r2 = ((s32) (temp_r2_5 + (temp_r2_5 >> 0x1F)) >> 1) - 0x20;
                        var_r3 = M2C_FIELD(sp38, s32 *, 4) - 0x28;
                    }
                    goto block_43;
                }
                if (M2C_FIELD(*sp30, s32 *, 4) == 0) {
                    var_r1_2 = (void *)((temp_r2_3 * 0x300) + 0x02015E00);
                    temp_r2_6 = M2C_FIELD(sp38, u32 *, 0);
                    var_r2 = ((s32) (temp_r2_6 + (temp_r2_6 >> 0x1F)) >> 1) - 0x10;
                    var_r3 = M2C_FIELD(sp18, s32 *, 4) - 8;
block_43:
                    renderers[var_r8_2 & 1](sp4C, var_r1_2, var_r2, var_r3, 0x30U, 0x10);
                } else {
                    temp_r2_7 = M2C_FIELD(sp38, u32 *, 0);
                    renderers[var_r8_2 & 1](sp4C, (void *)((temp_r2_3 * 0x300) + 0x02015E00), ((s32) (temp_r2_7 + (temp_r2_7 >> 0x1F)) >> 1) - 0x20, M2C_FIELD(sp18, s32 *, 4) - 8, 0x30U, 0x10);
                }
            }
            temp_r0_2 = temp_r2_2 + 2;
            if (sp48 != temp_r0_2) {

            } else {
                if (M2C_FIELD(sp24, u8 *, 0x080EED3E) == 1) {
                    ((ArmFill_080e2974)0x03000168)((void *)sp4C, 0x4000, 0x2F2F2F2F);
                }
                Func_080d6888(M2C_FIELD(M2C_FIELD(sp50, void **, 0x7828), s16 *, 0x24), 7, 5, 0, 4);
                if (var_r8_2 == (sp34 - 1)) {
                    Func_080b5088(M2C_FIELD(M2C_FIELD(sp50, void **, 0x7828), s16 *, 0x24), 4);
                    M2C_FIELD(sp50, s32 *, 0x77A8) = 8;
                    Func_080b50e8(0x86);
                } else {
                    if (var_r8_2 & 1) {
                        Func_080b5088(M2C_FIELD(M2C_FIELD(sp50, void **, 0x7828), s16 *, 0x24), 7);
                    }
                    M2C_FIELD(sp50, s32 *, 0x77A8) = 4;
                    Func_080f9010(0x86);
                }
                var_r6_2 = 0;
                if (M2C_FIELD(sp20, u8 *, 0x080EED3E) != 0) {
                    var_r1_3 = 0x080EED3E;
                    var_r5_3 = (s32 *)(sp10 + 0x02010018);
                    var_r2_2 = 7;
                    do {
                        spC = var_r1_3;
                        sp8 = var_r2_2;
                        var_r6_2 += 1;
                        *var_r5_3 = (Func_08004458() & var_r2_2) + 0xF;
                        var_r5_3 += 0x1C;
                    } while (var_r6_2 != M2C_FIELD((void *)var_r1_3, u8 *, (sp3C - sp54) + 3));
                }
            }
            if ((sp48 >= temp_r0_2) && (sp48 < (s32) (temp_r2_2 + 0xE))) {
                temp_r3_7 = (sp48 - temp_r2_2) - 2;
                temp_r2_8 = M2C_FIELD(sp38, u32 *, 0);
                renderers[0](sp4C, sp50 + (((s32) (temp_r3_7 + (temp_r3_7 >> 0x1F)) >> 1) * 0x3C0) + 0x5100, ((s32) (temp_r2_8 + (temp_r2_8 >> 0x1F)) >> 1) - 0xA, M2C_FIELD(sp38, s32 *, 4) - 0x18, 0x14U, 0x30);
            }
            var_r8_2 += 1;
            sp10 += 0x380;
            if (var_r8_2 != sp34) {
                goto loop_33;
            }
        }
        Func_08002dd8(0x2F);
        Func_08002dd8(0x2E);
        Func_080049ac();
        Func_080051d8(sp40, sp14);
        Func_080ed408(0x2E, 7, 7, 3, 3);
        Func_080ed408(0x2F, 7, 7, 3, 2);
        renderers[0] = *(Renderer_080e2974 *)0x03001F08;
        renderers[1] = *(Renderer_080e2974 *)0x03001F0C;
        var_r6_3 = (void *)0x02010000;
        var_r8_3 = 0;
        do {
            temp_r5_2 = M2C_FIELD(var_r6_3, s32 *, 0x18);
            if (temp_r5_2 > 0) {
                Func_080e3944((s32) var_r6_3, sp68);
                temp_r2_9 = M2C_FIELD(sp68, u32 *, 0);
                temp_r5_3 = (temp_r5_2 >> 3) + 1;
                temp_r4 = temp_r5_3 * 2;
                temp_r2_10 = (s32) (temp_r2_9 + (temp_r2_9 >> 0x1F)) >> 1;
                M2C_FIELD(sp68, u32 *, 0) = (u32) temp_r2_10;
                renderers[((s32) (var_r8_3 + ((u32) var_r8_3 >> 0x1F)) >> 1) & 1](sp4C, sp44 + M2C_FIELD((temp_r4 - 2), u16 *, 0x080EDE48), temp_r2_10 - ((s32) (temp_r5_3 + (temp_r5_3 >> 0x1F)) >> 1), M2C_FIELD(sp68, s32 *, 4) - temp_r5_3, temp_r5_3, temp_r4);
                Func_080e38b8(var_r6_3, 0x3C, -0x400);
                if ((s32) M2C_FIELD(var_r6_3, s32 *, 4) <= 0x7FFFF) {
                    temp_r3_8 = 0 - M2C_FIELD(var_r6_3, s32 *, 0x10);
                    M2C_FIELD(var_r6_3, s32 *, 0x10) = (s32) ((s32) (temp_r3_8 + (temp_r3_8 >> 0x1F)) >> 1);
                }
                M2C_FIELD(var_r6_3, s32 *, 0x18) = (s32) (M2C_FIELD(var_r6_3, s32 *, 0x18) - 1);
            }
            var_r8_3 += 1;
            var_r6_3 += 0x1C;
        } while (var_r8_3 != 0x200);
        Func_08002dd8(0x2F);
        Func_08002dd8(0x2E);
        Func_080e155c(8, 8U);
        Func_080cd52c();
        M2C_FIELD(sp50, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        temp_r5_4 = sp48 + 1;
        sp48 = temp_r5_4;
        if (temp_r5_4 != M2C_FIELD(((sp3C - sp54) + 5), u8 *, 0x080EED3E)) {
            goto loop_27;
        }
    }
    Func_08004278((void *)0x080CD261);
    return Func_080cdbc0();
}
