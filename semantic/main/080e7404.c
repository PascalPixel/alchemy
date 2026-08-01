#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void *Func_08002f40(s32);
u32 Func_08004458(void);
u8 *Func_08009030(s32);
u8 **Func_080b5098(s32);
s32 Func_080cdbc0(void);

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Nine `bl`
 * sites land in the 0x080072e4 bank: three __call_via_r3 and six
 * __call_via_r4.
 *
 * 0x080e758a, 0x080e75ae (pool 0x080e75f8) and 0x080e76c8 (pool 0x080e7814)
 * are all 0x03001388, the IWRAM word copy. r3 is an ARGUMENT register, so the
 * draft's fourth argument WAS the callee; each call takes three.
 *
 * THE OTHER SIX ARE SLOT 46 IN TWO ERAS, AND THE RESOLVER REFUSED THE FIRST.
 * There are exactly TWO Func_080ed408 calls in 0x080e7404..0x080e823c, both
 * id 46 -- 0x080e77fa and 0x080e7d18 -- and exactly two releases, 0x080e7ce8
 * and 0x080e81f8. Counted over the whole owner, not sampled. So each era is
 * internally single-publish and caches; they are different pointers and need
 * separate locals.
 *
 *   ERA 1 -- publish 0x080e77fa, read 0x080e77fe, parked in [sp, #68] at
 *   0x080e7850, release 0x080e7ce8. Sites 0x080e7b6c, 0x080e7bd6, 0x080e7c56.
 *
 *   ERA 2 -- publish 0x080e7d18, read 0x080e7d20, parked in [sp, #68] AGAIN
 *   at 0x080e7d24, release 0x080e81f8. Sites 0x080e7f4e, 0x080e7ff2,
 *   0x080e8074.
 *
 * WHY THE RESOLVER REFUSED ERA 1, AND IT WAS RIGHT TO. It reports "no
 * fall-through past 0x080e7802", and 0x080e7802 is `b.n 0x080e7850` -- an
 * unconditional jump over an INLINE LITERAL POOL occupying
 * 0x080e7804..0x080e784f. A backward walk that stops at the first
 * non-fall-through cannot cross it. Following the branch settles it: the
 * value stored at 0x080e7850 is the r5 loaded at 0x080e77fe, four
 * instructions after the publish. That is a limit of the walk, not an open
 * question about the ROM -- and a different mechanism from the branch-target
 * refusal in 080d3854, which is why each refusal wants its own reading rather
 * than a rule of thumb.
 *
 * A SIXTH BASE SPELLING. Era 1 reads `[pool 0x080e784c + 8]`, and the pool
 * word is 0x03001f00, so the address is 0x03001f08. Era 2 reads
 * `[pool 0x080e7d64 + 184]` with the word 0x03001e50 -- 0x03001f08 again.
 * Two spellings of one slot inside one function, alongside 0x03001eec + 28,
 * 0x03001e80 + 0x88, 0x03001ef0 + 0x18 and 0x03001e8c + 140 elsewhere.
 * Resolve the base; never recognise an offset.
 *
 * Both eras are CLOSED against loops: no branch inside
 * 0x080e7850..0x080e7ce8 targets an address at or before 0x080e7850, and none
 * inside 0x080e7d24..0x080e823c targets one at or before 0x080e7d24. Checked
 * over every branch in both spans.
 *
 * PINNING: within each era all three sites take the same callee, so no
 * assignment among them can change the answer, and none was invented. The
 * era boundary itself is not an assignment problem -- the draft's own
 * Func_080ed408(0x2E, ...) / Func_08002dd8(0x2E) pairs bracket the two groups
 * exactly as the ROM does.
 *
 * ARITY: six at all six renderer sites; r4 is above the argument registers.
 *
 * UNCERTAINTY, left standing: what slot 46 CONTAINS in either era is not
 * settled here, and nothing here says the two eras hold the same routine.
 */
typedef void *(*WordCopy_080e7404)(void *destination, const void *source,
                                   s32 size);
typedef void (*Renderer_080e7404)(void *target, const void *source, s32 x,
                                  s32 y, u32 width, s32 height);

void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(s32, s32);
void Func_08009008(s32, s32 *, s32 *, s32);
void Func_08009020(void *, s32);
void Func_08009038(void *);
void Func_080b50e8(s32);
void Func_080b5118(void);
void Func_080c9048(void);
void Func_080cd104(s32, s32);
void Func_080cd594(s32);
void Func_080d6750(s32);
void Func_080d67dc(void);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080dbb24(s32, s32, u32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(void *, s32, s32);
void Func_080e3944(const void *, s32 *);
void Func_080e727c(s32, s32, s32);
void Func_080e7338(s32, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

/*
 * Drive the selector-driven two-stage visual sequence from setup through final
 * cleanup.
 *
 * The first stage builds the palette and tiled noise mask, projects a field of
 * sprite records, and advances the wipe for 192 frames.  The second stage
 * reuses those records for two particle layers and the foreground objects for
 * another 54 frames before restoring display state.
 *
 * Owner span: 0x080e7404-0x080e823c.  Both frame loops and their cleanup paths
 * are continuations of this owner rather than independently callable entries.
 */

s32 Func_080e7404(s32 arg0, s32 arg1) {
    Renderer_080e7404 renderer_46_first;
    Renderer_080e7404 renderer_46_second;
    s32 sp10;
    s32 sp14;
    s32 sp24;
    u8 *sp30;
    s32 sp34;
    s32 sp38;
    s32 sp40;
    u8 *sp48;
    s32 sp4C;
    s32 sp118;
    s32 stackWorkspace[17];
    s32 *phaseOneProjection;
    s32 *phaseTwoProjection;
    s32 *projected;
    s32 *screen;
    s32 *world;
    s32 *transform;
    u8 noise[128];
    u8 *state;
    s16 *var_r5_4;
    s16 *var_sl;
    s16 temp_r2_5;
    s16 temp_r3_6;
    u8 *var_r3_3;
    s32 temp_r0_5;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r1_7;
    s32 temp_r2_11;
    s32 temp_r2_4;
    s32 temp_r3_11;
    s32 temp_r3_12;
    s32 temp_r3_2;
    s32 temp_r3_7;
    s32 temp_r3_9;
    s32 temp_r4;
    s32 temp_r5_2;
    s32 temp_r5_6;
    s32 temp_r5_7;
    s32 temp_r5_8;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_4;
    s32 textureOffset;
    s32 var_fp;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r2_4;
    s32 var_r2_5;
    s32 var_r2_6;
    s32 var_r3;
    s32 var_r3_10;
    s32 var_r3_11;
    s32 var_r3_2;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r3_9;
    s32 var_r5;
    s32 var_r5_3;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r7;
    s32 var_r7_3;
    s32 var_r8;
    s32 var_r8_10;
    s32 var_r8_11;
    s32 var_r8_12;
    s32 var_r8_13;
    s32 var_r8_14;
    s32 var_r8_15;
    s32 var_r8_16;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r8_5;
    s32 var_r8_6;
    s32 var_r8_7;
    s32 var_r8_8;
    s32 var_r8_9;
    s32 var_sl_2;
    s8 temp_r1_2;
    u16 temp_r3_4;
    u16 temp_r3_5;
    u16 temp_r4_3;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r2_10;
    u32 temp_r3_13;
    u32 temp_r4_2;
    u32 temp_r5;
    u32 temp_r6;
    u32 var_fp_2;
    u32 var_r8_4;
    u8 *var_r5_2;
    u8 *temp_r0;
    u8 *temp_r0_2;
    u8 *temp_r1;
    u8 *temp_r1_8;
    u8 *temp_r2;
    u8 *temp_r2_2;
    u8 *temp_r2_3;
    u8 *temp_r2_6;
    u8 *temp_r2_7;
    u8 *temp_r2_9;
    u8 *temp_r3_10;
    u8 *temp_r3_3;
    u8 *temp_r3_8;
    u8 *temp_r5_3;
    u8 *temp_r5_4;
    u8 *temp_r5_5;
    u8 *temp_r7;
    u8 *var_r5_5;
    u8 *var_r5_6;
    u8 *var_r5_7;
    u8 *var_r7_2;
    u8 *var_r7_4;
    u8 *var_r7_5;

    /*
     * The original ABI deliberately overlays these temporary vectors.  The
     * two projection states start eight bytes apart; their later fields share
     * storage with the projected, screen, world, and transform vectors.
     */
    phaseTwoProjection = stackWorkspace;
    phaseOneProjection = stackWorkspace + 2;
    projected = stackWorkspace + 4;
    screen = stackWorkspace + 7;
    world = stackWorkspace + 10;
    transform = stackWorkspace + 13;

    sp4C = arg1;
    sp48 = *(u8 **)0x03001EF0;
    state = *(u8 **)0x03001EEC;
    M2C_FIELD(state, s32 *, 0x7828) = arg0;
    Func_080cd594(0x2000);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    if (sp4C == 1) {
        temp_r2 = *Func_080b5098(M2C_FIELD(M2C_FIELD(state, void **, 0x7828), s32 *, 8));
        M2C_FIELD(temp_r2, s32 *, 0x28) = 0xA0000;
        M2C_FIELD(temp_r2, s32 *, 0x48) = 0x91EB;
        Func_080d6888(M2C_FIELD(M2C_FIELD(state, void **, 0x7828), s32 *, 8), -1, 2, -1, 0);
        Func_080f9010(0x91);
        sp40 = sp4C;
        if (M2C_FIELD(M2C_FIELD(state, void **, 0x7828), s32 *, 4) != 1) {
            sp40 = -1;
        }
    } else {
        sp40 = -1;
    }
    Func_080c9048();
    M2C_FIELD((void *)0x05000000, s16 *, 0) = 0;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = 0;
    M2C_FIELD(state, s32 *, 0x7780) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080cd104(0, 0);
    Func_08004278((void *)0x080CD261);
    if (sp4C == 1) {
        var_r5 = 0x77D8;
        var_r6 = 0x1E3;
        var_r8 = 0;
        do {
            temp_r0 = Func_08009030(var_r6);
            M2C_FIELD(state, void **, var_r5) = temp_r0;
            if (temp_r0 != NULL) {
                M2C_FIELD(temp_r0, s8 *, 0x26) = 0;
                Func_08009020(temp_r0, 2);
                temp_r1 = M2C_FIELD(state, u8 **, var_r5);
                M2C_FIELD(temp_r1, u8 *, 9) = (u8) (M2C_FIELD(temp_r1, u8 *, 9) | 0xC);
            }
            var_r8 += 1;
            var_r5 += 4;
            var_r6 += 0x2001;
        } while (var_r8 != 2);
    } else {
        Func_080dbb24(1, 0x17D, 3U);
    }
    Func_080e0524((void *)0xC1, state, 1, 1);
    if (sp4C == 1) {
        ((WordCopy_080e7404)0x03001388)((void *)0x05000000, Func_08002f40(0xC4), 0x80);
    }
    sp118 = 0x01010101;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = &sp118;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x02010000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85002000;
    ((WordCopy_080e7404)0x03001388)((void *)0x06008000, (void *)0x02010000, 0x7800);
    Func_080030f8(1U);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x0400000A, s16 *, 0) = 0x1F80;
    M2C_FIELD((void *)0x0400000A, s16 *, 2) = 0x2787;
    var_r8_2 = 0;
    var_sl = (s16 *)0x05000100;
    do {
        temp_r6 = Func_08004458();
        temp_r5 = Func_08004458();
        var_r8_2 += 1;
        *var_sl = (((Func_08004458() & 0xF) + 0x10) << 0xA) | (((temp_r5 & 0xF) + 0x10) << 5) | ((temp_r6 & 0xF) + 0x10);
        var_sl += 1;
    } while (var_r8_2 != 0x3F);
    sp118 = 0;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = &sp118;
    M2C_FIELD((void *)0x040000D4, u8 **, 4) = sp48;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    var_r8_3 = 0;
    do {
        temp_r6_2 = Func_08004458() & 0x7F;
        temp_r5_2 = Func_08004458() & 0x7F;
        temp_r1_2 = (0x3F & Func_08004458()) + 0x40;
        var_r3 = temp_r5_2;
        if (temp_r5_2 < 0) {
            var_r3 = temp_r5_2 + 7;
        }
        var_r2 = temp_r6_2;
        if (temp_r6_2 < 0) {
            var_r2 = temp_r6_2 + 7;
        }
        textureOffset =
            (((((var_r3 >> 3) * 0x10) + (var_r2 >> 3)) * 8) +
             (temp_r5_2 & 7)) * 8 + (temp_r6_2 & 7);
        sp48[textureOffset] = (u8) temp_r1_2;
        var_r8_3 += 1;
    } while (var_r8_3 != 0x100);
    ((WordCopy_080e7404)0x03001388)((void *)0x06004000, sp48, 0x4000);
    M2C_FIELD((void *)0x03001CE0, s32 *, 0x10) = 0xF0;
    Func_080d6750(M2C_FIELD(state, s32 *, 0x7828));
    temp_r2_2 = state;
    M2C_FIELD(temp_r2_2, s32 *, 0x77D0) = 0;
    M2C_FIELD(temp_r2_2, s32 *, 0x77D4) = 0;
    M2C_FIELD(temp_r2_2, s32 *, 0x7790) = 0;
    M2C_FIELD(temp_r2_2, s32 *, 0x7794) = 2;
    M2C_FIELD(temp_r2_2, s32 *, 0x7798) = (s32) (sp40 << 7);
    M2C_FIELD(temp_r2_2, s32 *, 0x779C) = var_r8_3;
    Func_080041d8((void *)0x080C9139, 0x4FF);
    Func_080041d8((void *)0x080CD359, 0x480);
    var_r5_2 = noise;
    do {
        *var_r5_2 = Func_08004458() & 0x3F;
        var_r5_2 += 1;
    } while (var_r5_2 != noise + 128);
    var_r6_2 = 0;
    var_r8_4 = 1;
    var_r5_3 = 0;
    do {
        var_r6_2 += (s32) ((var_r8_4 >> 0x1F) + var_r8_4) >> 1;
        var_r8_4 += 4;
        if (var_r5_3 != var_r6_2) {
            do {
                var_r0 = 0;
loop_29:
                temp_r1_3 = var_r5_3 - noise[var_r0 & 0x7F];
                if ((temp_r1_3 >= 0) && (temp_r1_3 <= 0x7F)) {
                    var_r2_2 = temp_r1_3;
                    if (temp_r1_3 < 0) {
                        var_r2_2 = temp_r1_3 + 7;
                    }
                    var_r3_2 = var_r0;
                    if (var_r0 < 0) {
                        var_r3_2 = var_r0 + 7;
                    }
                    textureOffset =
                        (((((var_r2_2 >> 3) << 5) + (var_r3_2 >> 3)) * 8 +
                          (temp_r1_3 & 7)) * 8) + (var_r0 & 7);
                    M2C_FIELD((void *)0x02010000, s8 *, textureOffset) = 0;
                }
                var_r0 += 1;
                if (var_r0 != 0x100) {
                    goto loop_29;
                }
                var_r5_3 += 1;
            } while (var_r5_3 != var_r6_2);
        }
        M2C_FIELD(state, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
    } while (var_r6_2 <= 0xBF);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F42;
    M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x1010;
    sp38 = (s32) M2C_FIELD((void *)0x03001AD0, u16 *, 4);
    sp34 = (s32) M2C_FIELD((void *)0x03001AD0, u16 *, 6);
    sp30 = M2C_FIELD((void *)0x03001F00, void **, 0);
    M2C_FIELD((void *)0x03001AD0, u16 *, 4) = 0U;
    M2C_FIELD((void *)0x03001AD0, u16 *, 6) = 0x20U;
    Func_080ed408(0x2E, 8, 7, 3, 2);
    renderer_46_first = *(Renderer_080e7404 *)0x03001F08;
    temp_r2_3 = state;
    M2C_FIELD(temp_r2_3, s32 *, 0x7780) = 3;
    M2C_FIELD(temp_r2_3, s32 *, 0x7784) = 0x02020202;
    Func_080041d8((void *)0x080E72E1, 0x4FE);
    var_r8_5 = 0;
    var_r3_3 = state + 0x7098;
    do {
        var_r8_5 += 1;
        M2C_FIELD(var_r3_3, s32 *, 0) = -1;
        var_r3_3 += 0x1C;
    } while (var_r8_5 != 0x40);
    M2C_FIELD(sp30, s32 *, 0x10) = 1;
    M2C_FIELD(state, s32 *, 0x778C) = 0;
    var_fp = 0;
    sp10 = 0;
loop_43:
    temp_r0_2 = state;
    var_r3_4 = M2C_FIELD(temp_r0_2, s32 *, 0x778C);
    if (var_r3_4 < 0) {
        var_r3_4 += 3;
    }
    temp_r4 = var_r3_4 >> 2;
    var_r5_4 = (s16 *)(temp_r0_2 + 0x1F80);
    if (sp4C == 1) {
        if ((*(s32 *)0x03001B04 & 3) && (var_fp > 0x10)) {

        } else {
            goto block_52;
        }
    } else if ((*(s32 *)0x03001B04 & 3) && (var_fp > 4)) {

    } else {
block_52:
        if (var_fp == 0) {
            Func_080f9010(0x8D);
        }
        var_r8_6 = 0;
        do {
            var_r8_6 += 1;
            *var_r5_4 = 0;
            var_r5_4 += 1;
        } while (var_r8_6 != 0xF);
        do {
            temp_r1_4 = var_r8_6 - 0x10;
            var_r3_5 = temp_r1_4;
            if (temp_r1_4 < 0) {
                var_r3_5 = var_r8_6 - 0xD;
            }
            temp_r2_4 = (var_r3_5 >> 2) + temp_r4;
            var_r3_6 = temp_r2_4 - 0x20;
            var_r1 = temp_r2_4 - 0x50;
            if (var_r3_6 < 0) {
                var_r3_6 = 0;
            }
            if (var_r3_6 > 0x1F) {
                var_r3_6 = 0x1F;
            }
            if (var_r1 < 0) {
                var_r1 = 0;
            }
            if (var_r1 > 0x1F) {
                var_r1 = 0x1F;
            }
            var_r8_6 += 1;
            *var_r5_4 = (var_r3_6 << 0xA) | (var_r1 << 5) | (var_r1 >> 1);
            var_r5_4 += 1;
        } while (var_r8_6 != 0x87);
        do {
            var_r8_6 += 1;
            *var_r5_4 = 0;
            var_r5_4 += 1;
        } while (var_r8_6 != 0xA0);
        if (sp40 == 1) {
            var_r3_7 = var_fp;
            if (var_r3_7 < 0) {
                var_r3_7 += 3;
            }
            var_r7 = var_r3_7 >> 2;
        } else {
            var_r2_3 = var_fp;
            if (var_r2_3 < 0) {
                var_r2_3 += 3;
            }
            var_r7 = 0x40 - (var_r2_3 >> 2);
        }
        temp_r3_2 = 0x60 - var_fp;
        transform[3] = 0;
        transform[1] = 0xFF0000;
        if (sp4C == 1) {
            var_r6_3 = sp10 + 0xA000;
            phaseOneProjection[1] = var_r6_3;
            transform[0] = (s32) ((var_r7 << 0x10) + 0x500000);
            transform[2] = (s32) ((0x40 - temp_r3_2) << 0x10);
            Func_08009008(M2C_FIELD(state, s32 *, 0x77D8), transform, phaseOneProjection, 0);
            Func_08009008(M2C_FIELD(state, s32 *, 0x77DC), transform, phaseOneProjection, 0);
        } else {
            var_r6_3 = sp10 + 0x10000;
            phaseOneProjection[1] = var_r6_3;
            transform[0] = (s32) ((var_r7 << 0x10) + 0x600000);
            transform[2] = (s32) ((0x60 - temp_r3_2) << 0x10);
            Func_08009008(M2C_FIELD(state, s32 *, 0x77D8), transform, phaseOneProjection, 0);
        }
        var_r8_7 = 0;
        var_r2_4 = 0;
loop_80:
        temp_r3_3 = state + var_r2_4;
        temp_r5_3 = temp_r3_3 + 0x7080;
        if (M2C_FIELD(temp_r5_3, s32 *, 0x18) == -1) {
            temp_r1_5 = (0x7FFF & Func_08004458()) + 0x4000;
            M2C_FIELD(temp_r5_3, s32 *, 0x18) = 0;
            var_r3_8 = Func_08002322(temp_r1_5) * 0x1E;
            if (var_r3_8 < 0) {
                var_r3_8 += 0xFFFF;
            }
            M2C_FIELD(temp_r3_3, s32 *, 0x7080) = (s32) (((var_r7 + 0x60) << 0x10) + ((var_r3_8 >> 0x10) * var_r6_3));
            var_r3_9 = Func_0800231c(temp_r1_5) * 0x1E;
            if (var_r3_9 < 0) {
                var_r3_9 += 0xFFFF;
            }
            M2C_FIELD(temp_r5_3, s32 *, 4) = (s32) (((0x20 - temp_r3_2) << 0x10) - ((var_r3_9 >> 0x10) * var_r6_3));
        } else {
            var_r8_7 += 1;
            var_r2_4 += 0x1C;
            if (var_r8_7 != 0x20) {
                goto loop_80;
            }
        }
        projected[0] = 0;
        projected[1] = 0;
        projected[2] = 0x02000000;
        Func_080049ac();
        Func_08004cb4(projected);
        Func_08004c6c(0x800);
        Func_08004c1c(sp10);
        var_r7_2 = (void *)0x080EEE76;
        var_r8_8 = 0;
        do {
            temp_r3_4 = M2C_FIELD(var_r7_2, u16 *, 0);
            world[1] = (s32) ((M2C_FIELD(var_r7_2, s16 *, 2) + var_fp) << 0x10);
            temp_r3_5 = M2C_FIELD(var_r7_2, u16 *, 4);
            world[0] = (s32) (((s32) ((s16) temp_r3_4 + ((u32) (temp_r3_4 << 0x10) >> 0x1F)) >> 1) << 0x10);
            world[2] = (s32) (((s32) ((s16) temp_r3_5 + ((u32) (temp_r3_5 << 0x10) >> 0x1F)) >> 1) << 0x10);
            Func_080e3944(world, screen);
            temp_r2_5 = M2C_FIELD(screen, s16 *, 2);
            screen[0] = (s32) (temp_r2_5 + 0x80);
            temp_r3_6 = M2C_FIELD(screen, s16 *, 6);
            screen[1] = (s32) (temp_r3_6 + 0x3C);
            renderer_46_first((void *)0x02010000, state + 0x1F40, temp_r2_5 + 0x7C, temp_r3_6 + 0x38, 8U, 8);
            var_r8_8 += 1;
            var_r7_2 += 6;
        } while (var_r8_8 != 7);
        if (sp40 == 1) {
            var_r3_10 = var_fp;
            if (var_r3_10 < 0) {
                var_r3_10 += 3;
            }
            var_r7_3 = (var_r3_10 >> 2) - 0x10;
        } else {
            var_r2_5 = var_fp;
            if (var_r2_5 < 0) {
                var_r2_5 += 3;
            }
            var_r7_3 = 0x10 - (var_r2_5 >> 2);
        }
        var_r5_5 = (void *)0x080EEEA0;
        var_r8_9 = 0;
        do {
            temp_r3_7 = M2C_FIELD(var_r5_5, s16 *, 2) + (var_fp - 0x60);
            if (temp_r3_7 <= 0x5D) {
                renderer_46_first((void *)0x02010000, state + 0x1C80, (M2C_FIELD(var_r5_5, s16 *, 0) + var_r7_3) - 0xC, temp_r3_7 - 0xC, 0x18U, 0x18);
            } else if (temp_r3_7 <= 0x5F) {
                Func_080e7338((M2C_FIELD(var_r5_5, s16 *, 0) + var_r7_3) << 0x10, temp_r3_7 << 0x10, 1);
            }
            var_r8_9 += 1;
            var_r5_5 += 4;
        } while (var_r8_9 != 7);
        var_r8_10 = 0;
        var_r6_4 = 0;
        do {
            temp_r2_6 = state;
            temp_r3_8 = temp_r2_6 + var_r6_4;
            temp_r5_4 = temp_r3_8 + 0x7080;
            temp_r1_6 = M2C_FIELD(temp_r5_4, s32 *, 0x18);
            if (temp_r1_6 >= 0) {
                renderer_46_first((void *)0x02010000, temp_r2_6 + (temp_r1_6 << 0xA), M2C_FIELD(temp_r5_4, s16 *, 2) - 0x10, M2C_FIELD(temp_r5_4, s16 *, 6) - 0x10, 0x20U, 0x20);
                M2C_FIELD(temp_r3_8, s32 *, 0x7080) = (s32) (M2C_FIELD(temp_r3_8, s32 *, 0x7080) - (sp40 * 0x14000));
                M2C_FIELD(temp_r5_4, s32 *, 4) = (s32) (M2C_FIELD(temp_r5_4, s32 *, 4) + 0xFFFB0000);
                temp_r3_9 = M2C_FIELD(temp_r5_4, s32 *, 0x18) + 1;
                M2C_FIELD(temp_r5_4, s32 *, 0x18) = temp_r3_9;
                if (temp_r3_9 == 6) {
                    M2C_FIELD(temp_r5_4, s32 *, 0x18) = -1;
                }
            }
            var_r8_10 += 1;
            var_r6_4 += 0x1C;
        } while (var_r8_10 != 0x20);
        M2C_FIELD(state, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        sp10 += 0x100;
        temp_r2_7 = state;
        var_fp += 1;
        M2C_FIELD(temp_r2_7, s32 *, 0x778C) = (s32) (M2C_FIELD(temp_r2_7, s32 *, 0x778C) + 1);
        if (var_fp != 0xC0) {
            goto loop_43;
        }
    }
    Func_080030f8(1U);
    M2C_FIELD(sp30, s32 *, 0x10) = 0;
    Func_08004278((void *)0x080C9139);
    Func_08004278((void *)0x080E72E1);
    Func_08004278((void *)0x080CD359);
    M2C_FIELD((void *)0x03001AD0, u16 *, 4) = (u16) sp38;
    M2C_FIELD((void *)0x03001AD0, u16 *, 6) = (u16) sp34;
    Func_08002dd8(0x2E);
    Func_080d67dc();
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
    *(s32 *)0x04000028 = 0;
    M2C_FIELD((void *)0x04000020, s32 *, 0xC) = 0xFFFFF000;
    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x0400000C = 0x2784;
    Func_080ed408(0x2E, 7, 7, 3, 2);
    renderer_46_second = *(Renderer_080e7404 *)0x03001F08;
    Func_080e0524((void *)0xC0, state, 1, 0);
    var_r8_11 = 0;
    var_r6_5 = 0;
    do {
        temp_r5_5 = state + var_r6_5 + 0x7080;
        M2C_FIELD(temp_r5_5, s32 *, 0) = (s32) (Func_08004458() & 0x7F);
        var_r8_11 += 1;
        M2C_FIELD(temp_r5_5, s32 *, 4) = (s32) ((Func_08004458() & 0x7F) + 0x7F);
        var_r6_5 += 0x1C;
    } while (var_r8_11 != 0x20);
    var_r5_6 = (void *)0x02010000;
    var_r8_12 = 0;
    do {
        M2C_FIELD(var_r5_6, s32 *, 0) = 0;
        M2C_FIELD(var_r5_6, s32 *, 4) = 0;
        M2C_FIELD(var_r5_6, s32 *, 8) = 0;
        M2C_FIELD(var_r5_6, s32 *, 0xC) = (s32) (((u8) Func_08004458() - 0x7F) << 0xC);
        M2C_FIELD(var_r5_6, s32 *, 0x10) = (s32) ((u8) Func_08004458() << 0xB);
        var_r8_12 += 1;
        M2C_FIELD(var_r5_6, s32 *, 0x14) = (s32) (((u8) Func_08004458() - 0x7F) << 0xC);
        M2C_FIELD(var_r5_6, s32 *, 0x18) = 0;
        var_r5_6 += 0x1C;
    } while (var_r8_12 != 0x80);
    var_r5_7 = (void *)0x02010E00;
    var_r8_13 = 0;
    do {
        M2C_FIELD(var_r5_7, s32 *, 0) = 0;
        M2C_FIELD(var_r5_7, s32 *, 4) = 0;
        M2C_FIELD(var_r5_7, s32 *, 8) = 0;
        M2C_FIELD(var_r5_7, s32 *, 0xC) = (s32) (((u8) Func_08004458() - 0x80) << 0xD);
        M2C_FIELD(var_r5_7, s32 *, 0x10) = (s32) ((u8) Func_08004458() << 0xB);
        var_r8_13 += 1;
        M2C_FIELD(var_r5_7, s32 *, 0x14) = (s32) (((u8) Func_08004458() - 0x80) << 0xD);
        M2C_FIELD(var_r5_7, s32 *, 0x18) = 0;
        var_r5_7 += 0x1C;
    } while (var_r8_13 != 0x200);
    temp_r2_9 = state;
    M2C_FIELD(temp_r2_9, s32 *, 0x7780) = 1;
    M2C_FIELD(temp_r2_9, s32 *, 0x7784) = 0x10101010;
    Func_080041d8((void *)0x080CD261, 0x480);
    sp14 = 0x1D000;
    var_fp_2 = 0;
loop_117:
    temp_r1_7 = var_fp_2 - 0x10;
    temp_r5_6 = *(s32 *)0x03001E80;
    sp24 = temp_r1_7;
    if (temp_r1_7 > 0x13) {
        Func_080e727c(2, 2, 2);
    }
    if (var_fp_2 == 0) {
        Func_080f9010(0x9C);
    }
    if (var_fp_2 == 0x28) {
        Func_080f9010(0x91);
    }
    if (var_fp_2 == 0x30) {
        if (sp4C == 1) {
            Func_08009038(M2C_FIELD(state, void **, 0x77D8));
            Func_08009038(M2C_FIELD(state, void **, 0x77DC));
            Func_080b5118();
        }
        Func_080b50e8(0x86);
    }
    Func_080049ac();
    Func_080051d8(temp_r5_6, temp_r5_6 + 0xC);
    var_r7_4 = (void *)0x02010E00;
    var_r8_14 = 0;
    do {
        if ((s32) M2C_FIELD(var_r7_4, s32 *, 4) >= 0) {
            Func_080e3944(var_r7_4, projected);
            var_r2_6 = projected[2];
            projected[0] = projected[0] >> 1;
            if (var_r2_6 <= 0x9F) {
                projected[2] = 0xA0;
                var_r2_6 = 0xA0;
            }
            if (var_r2_6 > 0x31F) {
                projected[2] = 0x31F;
                var_r2_6 = 0x31F;
            }
            var_r3_11 = var_r2_6 - 0xA0;
            if (var_r3_11 < 0) {
                var_r3_11 += 0x3F;
            }
            temp_r0_3 = 9 - (var_r3_11 >> 6);
            temp_r5_7 = temp_r0_3 * 2;
            renderer_46_second(sp48, state + M2C_FIELD((void *)0x080EDE48, u16 *, temp_r5_7 - 2) + ((var_r8_14 & 1) * 0x302) + 0x3200, projected[0] - ((s32) (temp_r0_3 + (temp_r0_3 >> 0x1F)) >> 1), projected[1] - temp_r0_3, temp_r0_3, temp_r5_7);
            Func_080e38b8(var_r7_4, 0x40, -0x2000);
            if ((s32) M2C_FIELD(var_r7_4, s32 *, 4) <= 0x140000) {
                M2C_FIELD(var_r7_4, s32 *, 0) = 0;
                M2C_FIELD(var_r7_4, s32 *, 8) = 0;
                M2C_FIELD(var_r7_4, s32 *, 4) = 0x140000;
                M2C_FIELD(var_r7_4, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xF);
                M2C_FIELD(var_r7_4, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xD);
                M2C_FIELD(var_r7_4, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xF);
            }
        }
        var_r8_14 += 1;
        var_r7_4 += 0x1C;
    } while (var_r8_14 != 0x40);
    var_r8_15 = 0;
    var_sl_2 = 0;
    do {
        temp_r1_8 = state;
        temp_r5_8 = 7 & var_r8_15;
        temp_r3_10 = temp_r1_8 + var_sl_2;
        temp_r4_2 = temp_r5_8 + 3;
        temp_r6_3 = temp_r4_2 * 2;
        temp_r7 = temp_r3_10 + 0x7080;
        renderer_46_second(sp48, temp_r1_8 + M2C_FIELD((void *)0x080EDE48, u16 *, temp_r6_3 - 2) + ((var_r8_15 & 1) * 0x302) + 0x3200, M2C_FIELD(temp_r3_10, s32 *, 0x7080) - (temp_r4_2 >> 1), M2C_FIELD(temp_r7, s32 *, 4) - temp_r4_2, temp_r4_2, temp_r6_3);
        temp_r3_11 = (M2C_FIELD(temp_r7, s32 *, 4) - temp_r5_8) - 8;
        M2C_FIELD(temp_r7, s32 *, 4) = temp_r3_11;
        if (temp_r3_11 < -0xA) {
            M2C_FIELD(temp_r7, s32 *, 4) = 0x80;
        }
        var_r8_15 += 1;
        var_sl_2 += 0x1C;
    } while (var_r8_15 != 0x40);
    var_r7_5 = (void *)0x02010000;
    var_r8_16 = 0;
    do {
        if ((Func_080022ec(var_r8_16, 3) < sp24) && ((s32) M2C_FIELD(var_r7_5, s32 *, 4) >= 0)) {
            Func_080e3944(var_r7_5, projected);
            temp_r6_4 = projected[0] >> 1;
            projected[0] = temp_r6_4;
            temp_r2_10 = M2C_FIELD(var_r7_5, u32 *, 0x18);
            if (temp_r2_10 <= 0xDU) {
                temp_r3_12 = ((s32) (temp_r2_10 + (temp_r2_10 >> 0x1F)) >> 1) * 2;
                temp_r4_3 = M2C_FIELD((void *)0x080EEECA, u16 *, temp_r3_12);
                temp_r0_4 = temp_r4_3 >> 1;
                renderer_46_second(sp48, state + M2C_FIELD((void *)0x080EEEBC, u16 *, temp_r3_12), temp_r6_4 - temp_r0_4, projected[1] - temp_r0_4, (u32) temp_r4_3, (s32) temp_r4_3);
            }
            temp_r3_13 = M2C_FIELD(var_r7_5, u32 *, 0x18) + 1;
            M2C_FIELD(var_r7_5, u32 *, 0x18) = temp_r3_13;
            if (temp_r3_13 == 0xE) {
                M2C_FIELD(var_r7_5, s32 *, 4) = 0x140000;
                M2C_FIELD(var_r7_5, s32 *, 0) = 0;
                M2C_FIELD(var_r7_5, s32 *, 8) = (s32) (((u8) Func_08004458() - 0x7F) << 0x10);
                M2C_FIELD(var_r7_5, s32 *, 0xC) = 0;
                M2C_FIELD(var_r7_5, s32 *, 0x10) = (s32) ((u8) Func_08004458() << 0xB);
                M2C_FIELD(var_r7_5, s32 *, 0x14) = 0;
                M2C_FIELD(var_r7_5, u32 *, 0x18) = 0U;
            } else {
                Func_080e38b8(var_r7_5, 0x40, 1);
            }
        }
        var_r8_16 += 1;
        var_r7_5 += 0x1C;
    } while (var_r8_16 != 0x40);
    if (sp40 == 1) {
        var_r1_2 = ((s32) ((var_fp_2 >> 0x1F) + var_fp_2) >> 1) + 0x18;
    } else {
        var_r1_2 = 0x38 - ((s32) ((var_fp_2 >> 0x1F) + var_fp_2) >> 1);
    }
    temp_r0_5 = 0x40 - (var_fp_2 * 2);
    temp_r2_11 = (var_fp_2 << 8) + 0x20000;
    transform[3] = 0;
    transform[1] = 0xFF0000;
    if (sp4C == 1) {
        phaseTwoProjection[1] = sp14;
        transform[0] = (s32) ((var_r1_2 << 0x10) + 0x600000);
        transform[2] = (s32) ((0x60 - temp_r0_5) << 0x10);
        Func_08009008(M2C_FIELD(state, s32 *, 0x77D8), transform, phaseTwoProjection, 0);
        Func_08009008(M2C_FIELD(state, s32 *, 0x77DC), transform, phaseTwoProjection, 0);
    } else {
        phaseTwoProjection[1] = temp_r2_11;
        transform[0] = (s32) ((var_r1_2 << 0x10) + 0x600000);
        transform[2] = (s32) ((0x60 - temp_r0_5) << 0x10);
        Func_08009008(M2C_FIELD(state, s32 *, 0x77D8), transform, phaseTwoProjection, 0);
    }
    M2C_FIELD(state, s32 *, 0x77A8) = 1;
    Func_080e155c(8, 8U);
    M2C_FIELD(state, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    var_fp_2 += 1;
    sp14 += 0x100;
    if (var_fp_2 != 0x36) {
        goto loop_117;
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2E);
    if (sp4C == 0) {
        Func_08009038(M2C_FIELD(state, void **, 0x77D8));
    }
    return Func_080cdbc0();
}
