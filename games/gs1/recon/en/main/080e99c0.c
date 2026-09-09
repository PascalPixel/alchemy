#include "effect_step.h"

/*
 * Battle-presentation scene at main:080e99c0 (1816 bytes), another member of
 * the 0x03001eec "battle work" family.  The single argument is the effect
 * state pointer, stored at work + 0x7828 exactly like every other member of
 * that family -- see games/gs1/recon/en/main/080ea0d8.c (the immediate
 * neighbour and the closest sibling), 080e2538.c, 080e823c.c and 080eb754.c
 * for the shared prologue, the Value_XXXXXXXX effect-id idiom, the
 * heap[7]/heap[8] blitter publication through Func_080ed408 and the
 * Data_080ede48 sprite-cell table this owner reuses.
 *
 * The owner is one 102-frame animation pass, one frame per iteration, with
 * Func_080030f8(1) as the frame barrier.  Before the pass it seeds three
 * record tables of struct EffectStep:
 *
 *   - 32 records at work + 0x7080 (loop bound 32) get a radial position
 *     scaled by i * 2 and a variant of i / 2 + 25; only the first 16 are
 *     ever drawn (the per-frame loop bound is 16);
 *   - 684 records (171 << 2) at 0x02010000 are parked with variant == -1;
 *   - the 340 records (170 << 1) at 0x02014ad0 -- immediately after the
 *     parked pool, since 0x02010000 + 684 * 28 == 0x02014ad0 -- get a fixed
 *     origin, a random outward velocity and a variant of 32..39.
 *
 * The two EWRAM windows are adjacent, not overlapping.  What the reference
 * does do is walk different prefixes of the parked pool at different points:
 * the frame-28 seed scan stops after 256 records (128 << 1), the frames
 * 32..63 seed scan walks all 684, and the dust step at frames <= 71 walks
 * only the first 342 (171 << 1).  Records 342..683 can therefore be seeded
 * by the 32..63 scan and are then never stepped, drawn or recycled by this
 * owner.  That asymmetry is in the shipped code, not a transcription slip;
 * whether it was intentional is not established here.
 *
 * Per frame it calls Func_080e46f0 with one id over frames 25..47 and with a
 * second id over frames > 56, cues Func_080f9010 at frames 4, 32 and 60 plus
 * Func_080b50e8(134) at frame 60, draws the 16 work + 0x7080 billboards once
 * frame > 55, seeds dust from the parked table (frame 28, then at most
 * sixteen records per frame over frames 32..63), scrolls a 34x104 strip
 * through a wrapping pair of blits, steps and draws the dust with per-lane
 * gravity from Data_080eef18, slides a 36-wide panel down the screen with
 * bottom clipping, steps and draws the 340 sparks with Func_080e3908, and
 * finally shakes the camera and ticks the object group.
 *
 * `Func_080072f0`, `Func_080072f4`, `Func_080072fc` and `Func_08007308` are
 * NOT real callees: they are the r3/r4/r6/r9 entries of the `_call_via_rN`
 * trampoline bundle at games/gs1/asm/080072e4.s (0x080072e4 + 4*N), so every
 * one of those `bl` sites is an indirect call through whatever function
 * pointer the compiler placed in that register.  Here the r4/r6/r9 sites are
 * only ever the two rectangle blitters that Func_080ed408(46, ...) and
 * Func_080ed408(47, ...) publish into heap[7] (0x03001f08) and heap[8]
 * (0x03001f0c), so this draft spells them as ordinary indirect calls through
 * `DrawRectangleFn` locals -- the candidate emits its own `_call_via_rN`
 * veneers for them, through different registers than the reference chose.
 * The r3 site keeps the established `Func_080072f0(dest, a, b, routine)`
 * spelling already used in 08003fa4.c, 08004c1c.c and 08006cdc.c, where the
 * fourth argument is the routine the veneer jumps to.
 *
 * Known-deliberate oddities preserved from the reference rather than
 * "corrected":
 *   - frame 8 stores the loop counter itself into work + 0x77A8, while
 *     frames 48 and 60 store literal 8 and 16 into the same word;
 *   - the frame-9 and frame-60 IWRAM fills are byte-for-byte the same call,
 *     emitted twice under two separate guards;
 *   - the dust step at frames <= 71 open-codes exactly the body of
 *     EffectStep_AdvanceWithGravity2D with damping 62 instead of calling it,
 *     while the spark step at frames > 59 calls Func_080e3908 with damping
 *     64;
 *   - frames 32..63 run the seed scan and the strip blit under two separate
 *     evaluations of the same `(u32)(frame - 32) <= 31` guard.
 *
 * Uncertain: the role of the halfword read at object + 0x24 (an actor id fed
 * to Func_080e3980, Func_080d6888 and Func_080b5088) and of the count at
 * object + 0x14; whether the two high-half reads spelled
 * `*(s16 *)((u8 *)&rec + 2)` here were a union member in the original rather
 * than the `>> 16` the spark loop uses for the same datum; and whether the
 * pool-loaded small constants spelled `&Value_XXXXXXXX` below are link-time
 * ids (the family precedent) or plain literals the compiler chose to pool.
 * The macro names RAIN, DUST and SPARK below are unevidenced reading aids
 * for three record windows whose subject matter is not established; only
 * their addresses, extents and access widths are evidence.
 *
 * Measured residual (alchemy score --owner 080e99c0): candidate 1816 bytes
 * against reference 1816, topology equal, differing_halfwords 602,
 * wrong_instructions 672.  Every reference branch, loop, call, store and
 * side effect is represented and the `bl` sequence matches the reference
 * one-for-one, so the residual is register and stack-slot assignment, not
 * shape:
 *   - the reference keeps the frame counter in fp and the inner counters in
 *     r8/r9/sl; this draft lands the frame counter in r9, which renames a
 *     register in most of the loop body;
 *   - the reference assigns its spill slots in a different order (half at
 *     sp+16 and sheet at sp+20, against sp+12 and sp+28 here), which shifts
 *     most sp-relative operands;
 *   - the reference reserves a 52-byte frame although its emitted code
 *     touches only 48 bytes of it (sp+0..sp+39 plus the 8-byte
 *     EffectPosition at sp+40); struct EffectPosition is proved to be 8
 *     bytes by the exact games/gs1/src/effects/position source, so the
 *     spare word is an unexplained spill slot, not a wider record, and no
 *     padding has been invented here to reproduce it;
 *   - the 0x1010 store to 0x04000052 loads its constant from a word pool in
 *     the reference and from a halfword pool here.
 * This is a draft, not a match; it must not be adopted.
 */

/* The record table at work + 0x7080: 32 records are seeded, the first 16
   are drawn.  The reference recomputes this address at each of its two uses
   rather than keeping it in a callee-saved register, so this draft spells it
   the same way. */
#define RAIN(work) ((struct EffectStep *)((u8 *)(work) + (225 << 7)))

/* The two fixed EWRAM record tables: 684 parked dust records followed by
   340 spark records.  The reference reloads both addresses from the literal
   pool at every use rather than holding them in a register. */
#define DUST ((struct EffectStep *)0x02010000)
#define SPARK ((struct EffectStep *)0x02014AD0)

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);

extern char Value_00000056;
extern char Value_00000073;
extern char Value_0000007d;
extern char Value_00000085;
extern char Value_000000c0;
extern char Value_000000c4;
extern char Value_03000168;

extern const u16 Data_080ede48[];
extern const u8 Data_080eef12[];
extern const s32 Data_080eef18[];

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 a, s32 b);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
/* Runtime_ReleaseHeapBlock */
void Func_08002dd8(s32 kind);
void Func_080030f8(s32 frames);
/* Scheduler_AddOrUpdateCallback */
void Func_080041d8(void *callback, s32 interval);
/* Scheduler_RemoveCallback */
void Func_08004278(void *callback);
/* Random16 */
s32 Func_08004458(void);
/* _call_via_r3 thunk, games/gs1/asm/080072e4.s */
void Func_080072f0(void *dest, s32 arg1, s32 arg2, void *routine);
void Func_080b5088(s32 member, s32 arg);
void Func_080b50e8(s32 id);
/* ObjectGroup_TickMemberTimers */
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
s32 Func_080cdbc0(void);
/* object/group/update_members.c */
void Func_080d6888(s32 member, s32 b, s32 c, s32 d, s32 e);
/* Resource_LoadAndDecompress */
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);
/* Camera_ApplyShake */
void Func_080e155c(s32 a, s32 b);
/* EffectStep_AdvanceWithGravity2D */
void Func_080e3908(struct EffectStep *step, s32 damping, s32 gravity);
/* EffectPosition_ApplyAlternateStepAndYOffset */
void Func_080e3980(s32 actor, struct EffectPosition *out);
void Func_080e46f0(s32 id);
s32 Func_080ed408(s32 kind, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 cue);

void Func_080e99c0(void *object)
{
    void **heap;
    void **p;
    void *work;
    void *canvas;
    void *sheet;
    DrawRectangleFn draw;
    DrawRectangleFn draw2;
    struct EffectStep *dust;
    struct EffectStep *spark;
    struct EffectPosition pos;
    s32 frame;
    s32 step;
    s32 half;
    s32 origin;
    s32 i;
    s32 cnt;
    s32 ang;
    s32 mag;
    s32 size;
    s32 life;
    s32 top;
    s32 high;
    s32 x;
    s32 y;
    const u8 *tbl;

    /* The reference walks the first two heap slots through a copy of the
       base pointer (adds/ldmia r3!) and keeps the base itself for slots 2,
       7 and 8, so the copy is spelled out here. */
    heap = (void **)0x03001EEC;
    p = heap;
    work = *p++;
    canvas = *p;
    sheet = heap[2];
    dust = DUST;
    spark = SPARK;

    *(void **)((u8 *)work + 0x7828) = object;
    Func_080cd594(1);
    *(u16 *)0x04000052 = 0x1010;
    Func_080e3980(
        *(s16 *)((u8 *)(*(void **)((u8 *)work + 0x7828)) + 0x24), &pos);
    half = pos.x / 2;

    Func_080ed408(46, 7, 7, 3, 2);
    draw = (DrawRectangleFn)heap[7];
    Func_080ed408(47, 7, 7, 3, 1);
    draw2 = (DrawRectangleFn)heap[8];

    Func_080e0524((s32)&Value_00000056, (u8 *)work + 20000, 1, 1);
    Func_080e0524((s32)&Value_00000085, work, 1, 0);
    Func_080e0524((s32)&Value_0000007d, (u8 *)work + (221 << 4), 1, 0);
    Func_080e0524((s32)&Value_00000073, sheet, 0, 0);

    *(s32 *)((u8 *)work + (239 << 7)) = 2;
    *(s32 *)((u8 *)work + 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 144 << 3);

    /* Sixteen drawn billboards (the table holds more) at work + 0x7080. */
    i = 0;
    do {
        ang = Func_08004458() & 0xFFFF;
        RAIN(work)[i].x = (i * 2) * Func_08002322(ang);
        RAIN(work)[i].y = -((i * 2) * Func_0800231c(ang));
        RAIN(work)[i].variant = i / 2 + 25;
        i++;
    } while (i != 32);

    /* Park the dust pool; the 340 records past it are seeded below. */
    i = 0;
    do {
        dust[i].variant = -1;
        i++;
    } while (i != 171 << 2);

    origin = half << 16;
    i = 0;
    do {
        mag = Func_08004458() & 0x1FF;
        ang = Func_08004458() & 0xFFFF;
        spark[i].x = origin;
        spark[i].y = 176 << 15;
        mag += 32;
        spark[i].velocity_x = (mag * Func_08002322(ang)) >> 5;
        spark[i].velocity_y = -(mag * Func_0800231c(ang)) >> 6;
        spark[i].variant = (Func_08004458() & 7) + 32;
        i++;
    } while (i != 170 << 1);

    frame = 0;
    do {
        if (frame >= 25 && frame <= 47) {
            Func_080e46f0((s32)&Value_000000c0);
        }
        if (frame > 56) {
            Func_080e46f0((s32)&Value_000000c4);
        }

        if (frame == 8) {
            *(s32 *)((u8 *)work + 0x77A8) = frame;
        }
        if (frame == 48) {
            *(s32 *)((u8 *)work + 0x77A8) = 8;
        }
        if (frame == 60) {
            *(s32 *)((u8 *)work + 0x77A8) = 16;
        }

        if (frame == 4) {
            Func_080f9010(212);
        }
        if (frame == 32) {
            Func_080f9010(164);
        }
        if (frame == 60) {
            Func_080f9010(145);
            Func_080b50e8(134);
        }

        if (frame > 55) {
            /* The reference holds the cell table base in a callee-saved
               register across the whole loop rather than reloading it. */
            tbl = Data_080eef12;
            i = 0;
            do {
                x = *(s16 *)((u8 *)&RAIN(work)[i] + 2) + half;
                life = RAIN(work)[i].variant;
                y = *(s16 *)((u8 *)&RAIN(work)[i] + 6);
                if ((u32)life <= 17) {
                    draw(canvas,
                        (u8 *)work
                            + (tbl[Func_080022ec(life, 3)] << 11)
                            + (221 << 4),
                        x - 16, y + 48, 32, 64);
                }
                life = RAIN(work)[i].variant;
                RAIN(work)[i].variant = life > 0 ? life - 1 : -1;
                i++;
            } while (i != 16);
        }

        /* Frame 28 seeds every still-parked record in the first 256. */
        if (frame == 28) {
            i = 0;
            do {
                if (dust[i].variant == -1) {
                    mag = Func_08004458() & 63;
                    ang = Func_08004458() & 0xFFFF;
                    dust[i].x = ((mag * Func_08002322(ang)) >> 3) + origin;
                    dust[i].y =
                        ((mag * Func_0800231c(ang)) >> 2) + (192 << 15);
                    dust[i].velocity_x = ((Func_08004458() & 63) - 32) << 14;
                    dust[i].velocity_y = (-(Func_08004458() & 63) - 8) << 13;
                    dust[i].variant = 0;
                }
                i++;
            } while (i != 128 << 1);
        }

        /* Frames 32..63 scan all 684 and seed at most sixteen more each. */
        step = frame - 32;
        if ((u32)step <= 31) {
            cnt = 0;
            i = 0;
            do {
                if (dust[i].variant == -1) {
                    mag = Func_08004458() & 63;
                    ang = Func_08004458() & 0xFFFF;
                    dust[i].x = ((mag * Func_08002322(ang)) >> 3) + origin;
                    dust[i].y =
                        ((mag * Func_0800231c(ang)) >> 2) + (192 << 15);
                    dust[i].velocity_x = ((Func_08004458() & 63) - 32) << 14;
                    cnt++;
                    dust[i].velocity_y = (-(Func_08004458() & 63) - 8) << 13;
                    dust[i].variant = 0;
                    if (cnt == 16) {
                        break;
                    }
                }
                i++;
            } while (i != 171 << 2);
        }

        /* The same guard again: a 34x104 strip wrapping through 104 rows. */
        if ((u32)step <= 31) {
            size = Func_080022fc(frame * 16 - 256, 104);
            draw(canvas, work, half - 17, 4 - size, 34, 104);
            draw(canvas, work, half - 17, 108 - size, 34, size);
        }

        if (frame <= 71) {
            i = 0;
            do {
                if (dust[i].variant >= 0) {
                    size = Func_080022fc(i, 3) + 2;
                    if (dust[i].velocity_y > 0) {
                        size += 2;
                    }
                    if (frame > 68 && size <= 5) {
                        size = 6;
                    }
                    if (frame > 70 && size <= 6) {
                        size = 7;
                    }
                    if (frame > 72 && size <= 7) {
                        size = 8;
                    }
                    if (frame > 74 && size <= 8) {
                        size = 9;
                    }
                    if (frame > 76) {
                        size = 10;
                    }
                    draw(canvas, (u8 *)sheet + Data_080ede48[size - 1],
                        *(s16 *)((u8 *)&dust[i] + 2) - size / 2,
                        *(s16 *)((u8 *)&dust[i] + 6) - size,
                        size, size * 2);

                    /* EffectStep_AdvanceWithGravity2D open-coded with a
                       per-lane gravity and damping 62. */
                    dust[i].x += dust[i].velocity_x;
                    dust[i].y += dust[i].velocity_y;
                    if (frame > 80) {
                        dust[i].velocity_y += -32768;
                    } else {
                        dust[i].velocity_y += Data_080eef18[i & 3];
                    }
                    dust[i].velocity_x = dust[i].velocity_x * 62 / 64;
                    dust[i].velocity_y = dust[i].velocity_y * 62 / 64;
                    dust[i].variant++;
                    if (dust[i].velocity_y > 0
                        && *(s16 *)((u8 *)&dust[i] + 6) > 108) {
                        dust[i].variant = -1;
                    }
                }
                i++;
            } while (i != 171 << 1);
        }

        if (frame <= 95) {
            x = half - 18;
            high = 120;
            if (frame > 60) {
                top = frame * 8 - 450;
            } else if (frame > 32) {
                top = step / 2 + 16;
            } else if (frame <= 9) {
                top = frame * 16 - 128;
            } else {
                top = 16;
            }
            if (top + 120 > 108) {
                high = high - top - 12;
            }
            if (high > 0) {
                draw2(canvas, (u8 *)work + 20000, x, top, 36, high);
            }
        }

        if (frame > 59) {
            i = 0;
            do {
                if (spark[i].variant > 0) {
                    Func_080e3908(&spark[i], 64, 128 << 6);
                    y = spark[i].y;
                    spark[i].variant--;
                    if (y > (216 << 15)) {
                        spark[i].velocity_y = -spark[i].velocity_y / 2;
                    } else {
                        x = spark[i].x;
                        if ((u32)x <= 0x007EFFFF && y >= 0) {
                            size = Func_080022ec(spark[i].variant, 5) + 1;
                            draw(canvas,
                                (u8 *)sheet + Data_080ede48[size - 1],
                                (x >> 16) - size / 2, (y >> 16) - size,
                                size, size * 2);
                        }
                    }
                }
                i++;
            } while (i != 170 << 1);
        }

        if (frame == 68) {
            if (*(s32 *)((u8 *)(*(void **)((u8 *)work + 0x7828)) + 20)
                != 0) {
                i = 0;
                cnt = 36;
                do {
                    Func_080d6888(
                        *(s16 *)((u8 *)(*(void **)((u8 *)work + 0x7828))
                            + cnt),
                        7, 5, i, 16);
                    Func_080b5088(
                        *(s16 *)((u8 *)(*(void **)((u8 *)work + 0x7828))
                            + cnt),
                        7);
                    i++;
                    cnt += 2;
                } while (i
                    != *(s32 *)((u8 *)(*(void **)((u8 *)work + 0x7828))
                        + 20));
            }
        }

        if (frame == 9) {
            Func_080072f0(canvas, 128 << 7, 0x3F3F3F3F, &Value_03000168);
        }
        if (frame == 60) {
            Func_080072f0(canvas, 128 << 7, 0x3F3F3F3F, &Value_03000168);
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)((u8 *)work + 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 102);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
