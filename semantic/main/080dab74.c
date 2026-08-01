typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define NULL ((void *)0)

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. This file is
 * still a raw m2c draft; the veneer pass below is the only thing corrected.
 *
 * All five `bl 0x080072f4` sites are __call_via_r4 -- 0x080072f4 is index 4 of
 * the 0x080072e4 bank -- so each is an indirect call through r4.
 *
 * THE RENDERER PAIR ARRIVES AS AN OUT-PARAMETER. At 0x080dabbe r3 = sp + 52,
 * that address is parked in [sp, #32], and `Func_080cef64(0, sp + 52)` at
 * 0x080dabc8 fills sp+52 with allocator slot 46 and sp+56 with slot 47 -- the
 * byte-exact src/080cef64.c writes `output[0] = *(state + 184)` and
 * `output[1] = *(state + 188)`, i.e. 46 * 4 and 47 * 4 off 0x03001e50. So
 * m2c's single `u32 projection_context` is really a two-element array.
 *
 * WHY THE RESOLVER CALLED ALL FIVE OF THESE "memory". Four sites read
 * `ldr r4, [rN, #4]` where rN was just loaded from [sp, #32]. That is
 * renderers[1], not an unknown struct field -- but the tool cannot know rN is
 * a table base, so it correctly refuses and reports an indirect memory load.
 * **A `[rN, #4]` load whose rN is the parked table base is entry 1.** Resolve
 * rN before treating such a site as unresolvable.
 *
 * Each site is pinned to its C statement by ARGUMENT AGREEMENT AT EVERY
 * POSITION, never by order -- all five dispatch through r4, so the register
 * cannot separate them:
 *
 * - 0x080dae0c, r4 = [r0, #4] with r0 = [sp, #32]: entry 1. Width and height
 *   are two byte-table reads indexed by r7, and x and y subtract each halved.
 *   The `0x080EEAA2` statement.
 * - 0x080daf18, r4 = [r0, #4] with r0 = [sp, #32]: entry 1. Width is the
 *   constant 0x20 and x is a byte-table lookup. The `0x080EEA62` statement.
 * - 0x080daf7c, r4 = [sp, #52] directly: ENTRY 0. Same 0x080eeab2 source
 *   table and the same 0x20 width as the site above, but x is `ldrb [r5, #0]`.
 * - 0x080db062, r4 = [r6, #4] with r6 = [sp, #32] loaded at 0x080db05a:
 *   entry 1. x and y are `ldrsh [r5, #2]` and `ldrsh [r5, #6]`.
 * - 0x080db188, r4 = [r6, #4] with r6 reloaded from [sp, #32] at 0x080db17a:
 *   entry 1. y comes from `ldrsh [r5, #18]`.
 *
 * STRUCTURE: the two arms of the `blt` at 0x080daef6 call DIFFERENT
 * renderers -- entry 1 on the fall-through, entry 0 on the taken path. The
 * draft kept both arms but they read as differing only in an x expression;
 * the renderer difference was invisible to it.
 *
 * ARITY: six at every site. r0..r3 are set and two more words go out at
 * [sp, #0] and [sp, #4]. r4 is above the argument registers, so no argument
 * slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents. This file
 * resolved cleanly because its table lives in the caller's own frame; the
 * remaining memory sites in this audit read a callee out of a heap record
 * whose contents depend on what ran before, and those SHOULD end as written
 * uncertainties rather than names. A page of bounded uncertainties is this
 * job going right, not a reconstruction giving up.
 */
typedef void (*Renderer_080dab74)(
    s32 target, void *source, u8 x, s16 y, s32 width, s32 height);

void Func_08002dd8(s32);
void Func_08004cb4(void *);
void Func_080b50e8(s32);
void Func_080cd594(s32);
void Func_080d6888(s16, s32, s32, s32, s32);

/*
 * Run the layered particle scene used by a battle transition.
 *
 * The scene stages two particle banks around the active characters, expands
 * their vertical range, seeds a second burst, and cycles projected fragments
 * until the configured effect duration expires.
 */
void Func_080dab74(s32 arg0) {
    void *sp8;
    u32 spC;
    void **sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    u32 *sp20;
    void *sp24;
    s32 sp28;
    s32 sp2C;
    void *sp30;
    s32 sp48[3];
    s32 sp54[3];
    s32 display_state[3];
    Renderer_080dab74 renderers[2];
    s32 temp_r0;
    s32 temp_r0_3;
    s32 temp_r1_3;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r2_4;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r3_6;
    s32 temp_r3_7;
    s32 temp_r4_2;
    s32 temp_r4_5;
    s32 temp_r7;
    s32 var_r1;
    s32 var_r3;
    s32 var_r5_2;
    s32 var_r6;
    s32 var_sl;
    s32 var_sl_2;
    s32 var_sl_3;
    s32 var_sl_4;
    s32 var_sl_5;
    s32 var_sl_6;
    s32 var_sl_7;
    s32 var_sl_8;
    u32 *temp_r3;
    u32 temp_r0_2;
    u32 temp_r0_4;
    u32 temp_r2;
    u32 temp_r3_10;
    u32 temp_r3_11;
    u32 temp_r3_9;
    u8 temp_r3_8;
    u8 temp_r4;
    u8 temp_r4_3;
    u8 temp_r4_4;
    u8 temp_r4_6;
    u8 temp_r5;
    void *temp_r1;
    void *temp_r1_2;
    void *var_r5;
    void *var_r5_3;
    void *var_r5_4;
    void *var_r5_5;
    void *var_r5_6;
    void *var_r5_7;
    void *var_r6_2;
    void *var_r8;

    temp_r1 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp30 = temp_r1;
    sp2C = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp24 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    M2C_FIELD(temp_r1, s32 *, 0x7828) = arg0;
    Func_080cd594(1);
    *(s16 *)0x04000020 = 0x100;
    Func_080e0524((void *)0xB8, temp_r1, 1, 1);
    Func_080e0524((void *)0xBA, sp24, 0, 0);
    temp_r3 = (u32 *)renderers;
    sp20 = temp_r3;
    Func_080cef64(0, temp_r3);
    var_sl = 0;
    var_r5 = sp30 + 0x7080;
    do {
        M2C_FIELD(var_r5, s32 *, 0) = (s32) (0xFFFF & Func_08004458());
        M2C_FIELD(var_r5, s32 *, 8) = (s32) ((0x3F & Func_08004458()) + 0x38);
        var_sl += 1;
        M2C_FIELD(var_r5, s32 *, 4) = (s32) (((0x1F & Func_08004458()) - 0x40) << 0x10);
        var_r5 += 0x1C;
    } while (var_sl != 0x40);
    M2C_FIELD(sp30, s32 *, 0x7780) = 2;
    M2C_FIELD(sp30, s32 *, 0x7784) = 0x32;
    Func_080041d8(0x080CD261, 0x480);
    temp_r1_2 = M2C_FIELD(sp30, void **, 0x7828);
    if (M2C_FIELD(temp_r1_2, s32 *, 4) == 1) {
        *(s32 *)0x04000028 = 0xFFFF9000;
    }
    sp28 = 0;
    if (M2C_FIELD(((M2C_FIELD(temp_r1_2, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) == -0x4BU) {

    } else {
        sp8 = display_state;
        sp10 = sp30 + 0x7828;
loop_7:
        sp18 = 0x780000;
        sp14 = 0;
        if (sp28 == (M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) + 0xB)) {
            Func_080b50e8(0x84);
        }
        M2C_FIELD(sp8, s32 *, 0) = sp14;
        M2C_FIELD(sp8, s32 *, 4) = sp14;
        M2C_FIELD(sp8, s32 *, 8) = 0x02000000;
        Func_080049ac();
        Func_08004cb4(sp8);
        temp_r2 = sp28 - 0x24;
        spC = temp_r2;
        if ((temp_r2 <= 0x1BU) && !(3 & sp28)) {
            Func_080f9010(0x73);
        }
        if (sp28 == 0x55) {
            Func_080f9010(0x88);
        }
        var_sl_2 = 0;
        if (M2C_FIELD(M2C_FIELD(sp30, void **, 0x7828), s32 *, 0x14) != 0) {
            var_r6 = 0x24;
            var_r5_2 = 0x28;
            do {
                if (sp28 == var_r5_2) {
                    Func_080d6888(M2C_FIELD(M2C_FIELD(sp30, void **, 0x7828), s16 *, var_r6), 9, 5, -1, 0);
                }
                var_sl_2 += 1;
                var_r6 += 2;
                var_r5_2 += 4;
            } while (var_sl_2 != M2C_FIELD(M2C_FIELD(sp30, void **, 0x7828), s32 *, 0x14));
        }
        temp_r2_2 = M2C_FIELD(*sp10, s32 *, 0x18) * 3;
        sp1C = 0x10;
        if (sp28 < (s32) M2C_FIELD((temp_r2_2 + 2), u8 *, 0x080EEA88)) {
            sp1C = (s32) M2C_FIELD(temp_r2_2, u8 *, 0x080EEA88);
        }
        if (sp28 >= (s32) (M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) + 0x23)) {

        } else {
            var_sl_3 = 0;
            if (sp1C == 0) {

            } else {
                var_r8 = sp30 + 0x7080;
loop_26:
                if (sp28 > var_sl_3) {
                    var_r1 = var_sl_3;
                    if (var_r1 < 0) {
                        var_r1 += 7;
                    }
                    temp_r7 = var_sl_3 - ((var_r1 >> 3) * 8);
                    temp_r3_2 = M2C_FIELD(var_r8, s32 *, 4);
                    if ((temp_r3_2 < (s32) ((0x30 - ((s32) (((u32) var_sl_3 >> 0x1F) + var_sl_3) >> 1)) << 0x10)) && (temp_r3_2 > 0xFFD00000)) {
                        M2C_FIELD(sp54, s32 *, 0) = (s32) (M2C_FIELD(var_r8, s32 *, 8) * Func_08002322(M2C_FIELD(var_r8, s32 *, 0)));
                        M2C_FIELD(sp54, s32 *, 4) = (s32) M2C_FIELD(var_r8, s32 *, 4);
                        M2C_FIELD(sp54, s32 *, 8) = (s32) (M2C_FIELD(var_r8, s32 *, 8) * Func_0800231c(M2C_FIELD(var_r8, s32 *, 0)));
                        Func_080e3944((s32)sp54, sp48);
                        temp_r2_3 = ((s32) M2C_FIELD(sp48, s32 *, 0) >> 0x11) + 0x40;
                        M2C_FIELD(sp48, s32 *, 0) = temp_r2_3;
                        temp_r3_3 = M2C_FIELD(sp48, s16 *, 6) + 0x3C;
                        M2C_FIELD(sp48, s32 *, 4) = temp_r3_3;
                        temp_r5 = M2C_FIELD(temp_r7, u8 *, 0x080EEA91);
                        temp_r4 = M2C_FIELD(temp_r7, u8 *, 0x080EEA99);
                        renderers[1](sp2C, sp30 + M2C_FIELD((temp_r7 * 2), u16 *, 0x080EEAA2), temp_r2_3 - (temp_r5 >> 1), temp_r3_3 - (temp_r4 >> 1), (s32) temp_r5, (s32) temp_r4);
                    }
                    if (sp28 < (s32) M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88)) {
                        if (sp28 > (s32) (var_sl_3 + 0x10)) {
                            temp_r3_4 = M2C_FIELD(var_r8, s32 *, 8);
                            if (temp_r3_4 > 4) {
                                M2C_FIELD(var_r8, s32 *, 8) = (s32) (temp_r3_4 - 2);
                            }
                            temp_r3_5 = M2C_FIELD(var_r8, s32 *, 4);
                            if (temp_r3_5 <= 0x2FFFFF) {
                                M2C_FIELD(var_r8, s32 *, 4) = (s32) (temp_r3_5 + 0x50000);
                            }
                            M2C_FIELD(var_r8, s32 *, 0) = (s32) (M2C_FIELD(var_r8, s32 *, 0) + 0x200);
                        }
                    } else {
                        M2C_FIELD(var_r8, s32 *, 8) = (s32) (M2C_FIELD(var_r8, s32 *, 8) + 8);
                        temp_r3_6 = M2C_FIELD(var_r8, s32 *, 4) - ((Func_080022fc(var_sl_3, 5) + 2) << 0x10);
                        M2C_FIELD(var_r8, s32 *, 4) = temp_r3_6;
                        if (sp18 > temp_r3_6) {
                            sp18 = temp_r3_6;
                        }
                        if (sp14 < temp_r3_6) {
                            sp14 = temp_r3_6;
                        }
                    }
                }
                var_sl_3 += 1;
                var_r8 += 0x1C;
                if (var_sl_3 != sp1C) {
                    goto loop_26;
                }
            }
        }
        sp14 += 0x400000;
        sp18 += 0x400000;
        temp_r2_4 = M2C_FIELD(*sp10, s32 *, 0x18) * 3;
        if (sp28 < (s32) M2C_FIELD((temp_r2_4 + 2), u8 *, 0x080EEA88)) {
            var_sl_4 = 0;
            if (M2C_FIELD((temp_r2_4 + 1), u8 *, 0x080EEA88) != 0) {
                var_r5_3 = (void *)0x080EEA62;
                var_r6_2 = NULL;
                do {
                    if (var_sl_4 < Func_080022ec((s32) spC, 3)) {
                        temp_r4_2 = Func_080022fc(var_sl_4, 3);
                        if (sp28 >= (s32) (M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) - 7)) {
                            temp_r4_3 = M2C_FIELD(temp_r4_2, u8 *, 0x080EEAB8);
                            renderers[1](sp2C, sp30 + M2C_FIELD((temp_r4_2 * 2), u16 *, 0x080EEAB2), M2C_FIELD(var_r6_2, u8 *, 0x080EEA62), M2C_FIELD(var_r5_3, u8 *, 1) - temp_r4_3, 0x20, (s32) temp_r4_3);
                        } else {
                            temp_r4_4 = M2C_FIELD(temp_r4_2, u8 *, 0x080EEAB8);
                            renderers[0](sp2C, sp30 + M2C_FIELD((temp_r4_2 * 2), u16 *, 0x080EEAB2), M2C_FIELD(var_r5_3, u8 *, 0), M2C_FIELD(var_r5_3, u8 *, 1) - temp_r4_4, 0x20, (s32) temp_r4_4);
                        }
                    }
                    var_sl_4 += 1;
                    var_r5_3 += 2;
                    var_r6_2 += 2;
                } while (var_sl_4 != M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 1), u8 *, 0x080EEA88));
            }
        }
        if (sp28 == M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88)) {
            var_r5_4 = (void *)0x02010000;
            var_sl_5 = 0;
            do {
                M2C_FIELD(var_r5_4, s32 *, 0) = (s32) ((0x7F & Func_08004458()) << 0x10);
                M2C_FIELD(var_r5_4, s32 *, 4) = (s32) (((Func_08004458() & 0xF) + 0x50) << 0x10);
                M2C_FIELD(var_r5_4, s32 *, 8) = (s32) (((0x3F & Func_08004458()) - 0x20) << 0xC);
                M2C_FIELD(var_r5_4, s32 *, 0x10) = (s32) ((((0 - Func_08004458()) & 0xF) - 0x10) << 0xD);
                var_sl_5 += 1;
                M2C_FIELD(var_r5_4, s32 *, 0x18) = (s32) ((Func_08004458() & 0xF) + 0x10);
                var_r5_4 += 0x1C;
            } while (var_sl_5 != 0x20);
        }
        if (sp28 >= (s32) M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88)) {
            var_r5_5 = (void *)0x02010000;
            var_sl_6 = 0;
            do {
                if ((s32) M2C_FIELD(var_r5_5, s32 *, 0x18) >= 0) {
                    var_r3 = var_sl_6;
                    if (var_r3 < 0) {
                        var_r3 += 7;
                    }
                    temp_r4_5 = var_sl_6 - ((var_r3 >> 3) * 8);
                    renderers[1](sp2C, sp30 + M2C_FIELD((temp_r4_5 * 2), u16 *, 0x080EEACC), (u8) M2C_FIELD(var_r5_5, s16 *, 2), M2C_FIELD(var_r5_5, s16 *, 6), (s32) M2C_FIELD(temp_r4_5, u8 *, 0x080EEABB), (s32) M2C_FIELD(temp_r4_5, u8 *, 0x080EEAC3));
                    M2C_FIELD(var_r5_5, s32 *, 0) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0) + M2C_FIELD(var_r5_5, s32 *, 0xC));
                    M2C_FIELD(var_r5_5, s32 *, 4) = (s32) (M2C_FIELD(var_r5_5, s32 *, 4) + M2C_FIELD(var_r5_5, s32 *, 0x10));
                    M2C_FIELD(var_r5_5, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0x18) - 1);
                }
                temp_r3_7 = M2C_FIELD(var_r5_5, s32 *, 4);
                if (sp18 > temp_r3_7) {
                    sp18 = temp_r3_7;
                }
                if (sp14 < temp_r3_7) {
                    sp14 = temp_r3_7;
                }
                var_sl_6 += 1;
                var_r5_5 += 0x1C;
            } while (var_sl_6 != 0x18);
        }
        temp_r0 = sp18 >> 0x10;
        temp_r1_3 = sp14 >> 0x10;
        sp18 = temp_r0;
        sp14 = temp_r1_3;
        if (temp_r1_3 <= temp_r0) {
            sp14 = temp_r0 + 1;
        }
        if (sp28 == M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88)) {
            var_sl_7 = 0;
            var_r5_6 = sp30 + 0x7080;
            do {
                M2C_FIELD(var_r5_6, s32 *, 0xC) = (s32) ((0x7F & Func_08004458()) << 0x10);
                if (sp14 == sp18) {
                    M2C_FIELD(var_r5_6, s32 *, 0x10) = (s32) (sp18 << 0x10);
                } else {
                    M2C_FIELD(var_r5_6, s32 *, 0x10) = (s32) ((Func_08002304((s32) Func_08004458(), sp14 - sp18) + sp18) << 0x10);
                }
                var_sl_7 += 1;
                M2C_FIELD(var_r5_6, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 0x14);
                var_r5_6 += 0x1C;
            } while (var_sl_7 != 0x20);
        }
        temp_r3_8 = M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88);
        if (sp28 >= (s32) temp_r3_8) {
            temp_r3_9 = sp28 - temp_r3_8;
            var_sl_8 = 0;
            var_r5_7 = sp30 + 0x7080;
            do {
                temp_r3_10 = M2C_FIELD(var_r5_7, u32 *, 0x18);
                if (temp_r3_10 <= 0x11U) {
                    temp_r0_2 = 0x11 - temp_r3_10;
                    temp_r0_3 = (s32) (temp_r0_2 + (temp_r0_2 >> 0x1F)) >> 1;
                    temp_r4_6 = M2C_FIELD(temp_r0_3, u8 *, 0x080EDE96);
                    temp_r0_4 = temp_r4_6 >> 1;
                    renderers[1](sp2C, sp24 + M2C_FIELD((temp_r0_3 * 2), u16 *, 0x080EDE84), M2C_FIELD(var_r5_7, s16 *, 0xE) - temp_r0_4, (M2C_FIELD(var_r5_7, s16 *, 0x12) - temp_r0_4) - ((s32) (temp_r3_9 + (temp_r3_9 >> 0x1F)) >> 1), (s32) temp_r4_6, (s32) temp_r4_6);
                }
                temp_r3_11 = M2C_FIELD(var_r5_7, u32 *, 0x18) - 1;
                M2C_FIELD(var_r5_7, u32 *, 0x18) = temp_r3_11;
                if (((temp_r3_11 == -1U) || (temp_r3_11 == 0x11)) && (sp28 < (s32) (M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) + 0x23))) {
                    M2C_FIELD(var_r5_7, u32 *, 0x18) = 0x11U;
                    M2C_FIELD(var_r5_7, s32 *, 0xC) = (s32) ((0x7F & Func_08004458()) << 0x10);
                    M2C_FIELD(var_r5_7, s32 *, 0x10) = (s32) ((Func_08002304((s32) Func_08004458(), sp14 - sp18) + sp18) << 0x10);
                }
                var_sl_8 += 1;
                var_r5_7 += 0x1C;
            } while (var_sl_8 != 0x20);
        }
        M2C_FIELD(sp30, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        sp28 += 1;
        if (sp28 != (M2C_FIELD(((M2C_FIELD(*sp10, s32 *, 0x18) * 3) + 2), u8 *, 0x080EEA88) + 0x4B)) {
            goto loop_7;
        }
    }
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_08004278((void *)0x080CD261);
    Func_080cdbc0();
}
