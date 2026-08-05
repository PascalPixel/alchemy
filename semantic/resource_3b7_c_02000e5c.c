/*
 * resource_3b7 owner at 0x02000e5c, 2124 bytes (0x02000e5c-0x020016a7).
 *
 * The minigame's per-frame task - the callback Func_020016a8 installs through
 * Func_080000d0 (its pool word 0x02008e5d is this entry plus the Thumb bit,
 * which is one of the three witnesses for the overlay's 0x02008000 link base).
 * Each frame it advances the thrown object's ballistics, clamps it to the
 * arena, scores it once it stops, steps the four rotating obstacles, tests them
 * for collisions, and republishes every slot plus a four-frame position trail.
 *
 * Code runs 0x02000e5c-0x0200168b.  FOUR literal pools sit inside or at the end
 * of the span, and all four were derived from the owner's own control flow -
 * what the code branches over - not from `overlay_show.ts`'s "pool words
 * referenced" listing, which is not authoritative:
 *   0x02001072-0x02001093  eight words, jumped by `b.n 0x02001098` at
 *                          0x02001070 and by `bgt.n 0x02001094` at 0x0200106a
 *   0x0200136a-0x02001383  six words, jumped by `b.n 0x0200140e` at 0x02001368
 *                          and entered past by `bne.n 0x02001384` at 0x020012f8
 *   0x020013a0-0x020013a3  one word (0x000001ff), jumped by `b.n 0x020013a4`
 *                          at 0x0200139e and `bne.n 0x020013a4` at 0x02001398
 *   0x0200168c-0x020016a7  seven words, past the `bx r0` return
 * The disassembler renders several of these as NEON (`vhadd.s8`, `blx`,
 * `vshr.u64`, `vaddl.u`), which is exactly why they must not be decoded.
 * Register state is carried straight ACROSS the interior pools: r5 and r6 are
 * live across 0x0200136a and 0x020013a0 and are read immediately after them.
 *
 * LINK BASE - 0x02008000, and this owner is consistent with the layout the
 * other converted owners established.  Its in-image pool words are 0x0200a054
 * and 0x0200a057 (file offsets 0x2054/0x2057, byte tables spelled literally in
 * `assets/code/resource_3b7_overlay.s`).  The rest are the scratch EWRAM above
 * the 0x206a-byte image - 0x0200a070 (the state block), 0x0200a0c0 (the mode
 * word), 0x0200a0d0 (the four 24-byte obstacle records), 0x0200a134 (the
 * completion flag) and 0x0200a138 (the result) - all of which Func_0200173c
 * and Func_020016a8 use identically, plus the IWRAM helper 0x030001d8.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  Completeness, as a multiset:
 * Func_0808a080 x17, Func_08009080 x17, Func_02000dd0 x13, Func_03000380 x8,
 * Func_02000e44 x6, Func_030001d8 x2 (through the overlay's own `call_via`
 * veneer), Func_08000118 x2, Func_08000120 x2, and one each of Func_030003ac
 * and Func_080f9010 - 69 sites over 10 distinct targets, exactly the tool's
 * `sites=69 distinct_targets=10`.  The inventory row says calls=68, the
 * documented lower bound.
 *
 * Func_02000dd0 and Func_02000e44 are this overlay's own, both converted in
 * this reconstruction; Func_02000e44 is tracked byte-exact in `assets/code`.  The
 * Func_02000dd0 call sites confirm that owner's reconstruction from the other
 * side: r1 is always a pointer whose +0/+4/+8 are an s32 position triple, r2 is
 * a heading halfword, r3 a tag byte and [sp, #0] the fifth argument.
 *
 * The two `call_via` sites are the documented overlay veneer shape: a pool word
 * in the 0x030001xx IWRAM band (0x030001d8) loaded into r3 immediately before
 * the `bl`, with the callee's result returned unchanged.  0x030001d8 is the
 * relocated IWRAM square root, and both uses are `sqrt(dx*dx + dz*dz)` feeding
 * a normalisation divide - which is what identifies it here independently.
 * Func_03000380 is the IWRAM quotient helper (`src/080f2ebc.c` calls the same
 * address through a function pointer) and Func_030003ac its remainder
 * counterpart, used once as `(state + 1) % 3`.
 *
 * The epilogue is `add sp, #24 / pop {r3, r5, r6} / ... / pop {r5, r6, r7} /
 * pop {r0} / bx r0`, so r0 holds the popped return address and the owner
 * returns nothing: void.  r0 is written before any read, so it takes no
 * argument - as a task callback must.
 *
 * Uncertainties:
 *  - Func_08000118 and Func_08000120 are used as a sine/cosine pair (their
 *    results scale the X and Z of a circular path around a fixed centre).
 *    Which of the two is the sine is not established; the code is written as
 *    the assembly has it, by address.
 *  - The `negs / lsrs #31 / adds / asrs #1` idiom that follows every clamp is a
 *    negate-and-halve with truncation toward zero, i.e. a 50% restitution
 *    bounce.  It is spelled `-v / 2`, which C89 compilers on this target
 *    truncate identically; the sign is fixed by the branch above each use.
 *  - The two arena-boundary divides add pooled constants (0xef440000 and
 *    0xf5740000) after multiplying, rather than subtracting first.  Those are
 *    exactly -(0x660000 * 42) and -(0x960000 * 18), so the intent is
 *    `(z - 0x660000) * 42` and `(x - 0x960000) * 18`, but the assembly's own
 *    order is preserved because it differs on overflow.
 *  - r8's several lifetimes (the history-shift countdown, then the obstacle
 *    index) are split into separate variables here.
 *  - The state block's field names are descriptive.  The +4/+8/+12 triple and
 *    the +64/+68/+72 velocity triple are fixed by the integration; +16, +28,
 *    +40 and +52 are fixed by being handed to Func_02000dd0 as position
 *    triples; +2, +76 and the record fields are named from use alone.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

extern u8 Data_0200a054[];      /* in-image 0x2054: obstacle tag bytes, slots 0/1 */
extern u8 Data_0200a057[];      /* in-image 0x2057: obstacle tag bytes, slots 2/3 */
extern u8 Data_0200a070[];      /* scratch EWRAM: the state block */
extern s32 Data_0200a0c0;       /* scratch EWRAM: the mode word */
extern u8 Data_0200a0d0[];      /* scratch EWRAM: four 24-byte obstacle records */
extern s32 Data_0200a134;       /* scratch EWRAM: the completion flag */
extern s32 Data_0200a138;       /* scratch EWRAM: the result */

u8 *Func_0808a080();
void Func_08009080();
void Func_02000dd0();
void Func_02000e44();
void Func_080f9010();
s32 Func_03000380();            /* IWRAM quotient helper */
s32 Func_030003ac();            /* IWRAM remainder helper */
s32 Func_030001d8();            /* IWRAM square root, reached via a call_via veneer */
s32 Func_08000118();
s32 Func_08000120();

void Func_02000e5c(void)
{
    s32 permuted_3;
    u8 *state = Data_0200a070;
    u8 *rec;
    s32 remaining;
    s32 index;
    s32 x;
    s32 y;
    s32 z;
    s32 vy;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 quotient;
    s32 v;
    s32 t;
    s32 x_lo;
    s32 x_hi;
    s32 z_lo;
    s32 z_hi;
    s32 step;
    s32 tag;
    s32 slot;

    /* Shift the three-deep position history: +28 -> +40, +16 -> +28, +4 -> +16.
     * The loop walks its cursor backwards from state+28 in 12-byte steps. */
    {
        u8 *cursor = state + 28;

        remaining = 3;
        do {
            *(s32 *)(cursor + 20) = *(s32 *)(cursor + 8);
            *(s32 *)(cursor + 12) = *(s32 *)(cursor + 0);
            *(s32 *)(cursor + 16) = *(s32 *)(cursor + 4);
            remaining = remaining - 1;
            cursor = cursor - 12;
        } while (remaining != 0);
    }

    if (*(s16 *)(state + 2) > 31) {
        /* Integrate. */
        vy = *(s32 *)(state + 68);
        permuted_3 = *(s32 *)(state + 8) + vy;
        *(s32 *)(state + 8) = y;
        *(s32 *)(state + 4) = *(s32 *)(state + 4) + *(s32 *)(state + 64);
        y  = permuted_3;
        *(s32 *)(state + 12) = *(s32 *)(state + 12) + *(s32 *)(state + 72);

        if (y > 0) {
            /* Still airborne: gravity only, and nothing else this frame. */
            *(s32 *)(state + 68) = vy + (s32)0xffffc000;
            goto obstacles;
        }

        /* Landed. */
        *(s32 *)(state + 8) = 0;

        if (vy != 0) {
            *(s32 *)(state + 68) = 0;
            if (Data_0200a0c0 == 1) {
                Func_08009080(Func_0808a080(17), 1);
            } else {
                Func_08009080(Func_0808a080(12), 1);
            }
        }

        if (*(s32 *)(state + 76) > 0) {
            /* Driven phase: steer toward the arena centre and decay. */
            s32 kick;

            dx = (0x780000 - *(s32 *)(state + 4)) >> 8;
            dz = (0x470000 - *(s32 *)(state + 12)) >> 8;   /* 142 << 15 */

            dist = Func_030001d8(dx * dx + dz * dz);

            kick = *(s32 *)(state + 64) + Func_03000380(0x1999 * dx, dist);
            *(s32 *)(state + 64) = kick;

            v = *(s32 *)(state + 72) + Func_03000380(0x1999 * dz, dist);
            *(s32 *)(state + 72) = v;

            /* (n * 64 - n) * 4 + n == n * 253, then /256 rounding toward 0. */
            t = ((kick * 63) << 2) + kick;
            if (t < 0) {
                t = t + 255;
            }
            *(s32 *)(state + 64) = t >> 8;

            t = ((v * 63) << 2) + v;
            if (t < 0) {
                t = t + 255;
            }
            *(s32 *)(state + 72) = t >> 8;

            *(s32 *)(state + 76) = *(s32 *)(state + 76) - 1;
        } else {
            /* Rolling: friction, deadband, then score once fully stopped. */
            t = *(s32 *)(state + 64) * 220;
            if (t < 0) {
                t = t + 255;
            }
            *(s32 *)(state + 64) = t >> 8;

            t = *(s32 *)(state + 72) * 220;
            if (t < 0) {
                t = t + 255;
            }
            *(s32 *)(state + 72) = t >> 8;

            if ((*(s32 *)(state + 64) + 0x3ff) <= 0x7fe) {
                *(s32 *)(state + 64) = 0;
            }
            if ((*(s32 *)(state + 72) + 0x3ff) <= 0x7fe) {
                *(s32 *)(state + 72) = 0;
            }

            if (*(s32 *)(state + 64) == 0 && *(s32 *)(state + 72) == 0) {
                s32 score;

                if (Data_0200a0c0 == 1) {
                    Func_08009080(Func_0808a080(17), 2);
                    Func_02000e44(15, 0);
                    Func_02000e44(14, 0);
                    Func_02000e44(13, 0);
                } else {
                    Func_08009080(Func_0808a080(12), 2);
                    Func_02000e44(10, 0);
                    Func_02000e44(9, 0);
                    Func_02000e44(8, 0);
                }

                dx = (0x780000 - *(s32 *)(state + 4)) >> 16;
                dz = (0x470000 - *(s32 *)(state + 12)) >> 16;
                score = dx * dx + dz * dz;

                Data_0200a134 = 1;

                if (score <= 224) {
                    Data_0200a138 = 0;
                } else if (score <= 624) {            /* 156 << 2 */
                    Data_0200a138 = 1;
                } else if (score <= 1088) {           /* 136 << 3 */
                    Data_0200a138 = 2;
                } else if (score <= 1680) {           /* 210 << 3 */
                    Data_0200a138 = 3;
                } else {
                    Data_0200a138 = 4;
                }
            }
        }

        /* Arena bounds.  The X limits are a function of Z and vice versa, so
         * the playable area is a lozenge rather than a rectangle. */
        x_lo = 0x300000;        /* 192 << 14 */
        x_hi = 0xc00000;        /* 192 << 16 */
        z_lo = 0x180000;        /* 192 << 13 */
        z_hi = 0x780000;        /* 240 << 15 */

        z = *(s32 *)(state + 12);

        if (z < 0x2a0000) {                          /* 168 << 14 */
            quotient = Func_03000380((0x2a0000 - z) * 42, 18);
            x_lo = quotient + 0x300000;
            if (x_lo > 0x5a0000) {                   /* 180 << 15 */
                x_lo = 0x5a0000;
            }
            x_hi = 0xc00000 - quotient;
            if (x_hi < 0x960000) {                   /* 150 << 16 */
                x_hi = 0x960000;
            }
        }
        if (z > 0x660000) {                          /* 204 << 15 */
            /* z * 42 - 0x660000 * 42, in the assembly's own order. */
            quotient = Func_03000380(z * 42 + (s32)0xef440000, 18);
            x_lo = quotient + 0x300000;
            if (x_lo > 0x5a0000) {
                x_lo = 0x5a0000;
            }
            x_hi = 0xc00000 - quotient;
            if (x_hi < 0x960000) {
                x_hi = 0x960000;
            }
        }

        x = *(s32 *)(state + 4);

        if (x < 0x5a0000) {
            t = 0x5a0000 - x;
            quotient = Func_03000380(((t << 3) + t) << 1, 42);   /* t * 18 */
            z_lo = quotient + 0x180000;
            if (z_lo > 0x2a0000) {
                z_lo = 0x2a0000;
            }
            z_hi = 0x780000 - quotient;
            if (z_hi < 0x660000) {
                z_hi = 0x660000;
            }
        }
        if (x > 0x960000) {
            /* x * 18 - 0x960000 * 18, in the assembly's own order. */
            quotient = Func_03000380((((x << 3) + x) << 1) + (s32)0xf5740000, 42);
            z_lo = quotient + 0x180000;
            if (z_lo > 0x2a0000) {
                z_lo = 0x2a0000;
            }
            z_hi = 0x780000 - quotient;
            if (z_hi < 0x660000) {
                z_hi = 0x660000;
            }
        }

        if (x < x_lo) {
            v = *(s32 *)(state + 64);
            *(s32 *)(state + 4) = x_lo;
            if (v < 0) {
                *(s32 *)(state + 64) = -v / 2;
            }
            x = x_lo;
        }
        if (x > x_hi) {
            v = *(s32 *)(state + 64);
            *(s32 *)(state + 4) = x_hi;
            if (v > 0) {
                *(s32 *)(state + 64) = -v / 2;
            }
        }

        z = *(s32 *)(state + 12);
        if (z < z_lo) {
            v = *(s32 *)(state + 72);
            *(s32 *)(state + 12) = z_lo;
            if (v < 0) {
                *(s32 *)(state + 72) = -v / 2;
            }
            z = z_lo;
        }
        if (z > z_hi) {
            *(s32 *)(state + 12) = z_hi;
            v = *(s32 *)(state + 72);
            if (v > 0) {
                *(s32 *)(state + 72) = -v / 2;
            }
        }
    }

obstacles:
    for (index = 0; index != 4; index++) {
        rec = Data_0200a0d0 + index * 24;

        if (*(s16 *)(rec + 18) > 0) {
            *(u16 *)(rec + 18) = (u16)(*(u16 *)(rec + 18) - 1);
        }
        if (*(s16 *)(rec + 20) > 0) {
            *(u16 *)(rec + 20) = (u16)(*(u16 *)(rec + 20) - 1);
        }

        if (index <= 1) {
            /* Two sliding pieces on the X axis. */
            step = 0x10000;
            if (*(s16 *)(rec + 16) == 1) {
                step = step << 1;
            }
            if (*(s16 *)(rec + 16) == 2) {
                step = (step << 1) + step;
            }

            slot = (index == 0) ? 18 : 19;

            if (*(s16 *)(rec + 18) > 0) {
                Func_08009080(Func_0808a080(slot), 3);
                goto collision;
            }

            Func_08009080(Func_0808a080(slot), 1);

            if (*(s16 *)(rec + 14) != 0) {
                *(u16 *)(rec + 14) = (u16)(*(u16 *)(rec + 14) - 1);
            } else {
                if (*(s16 *)(rec + 12) == 0) {
                    *(s32 *)(rec + 0) = *(s32 *)(rec + 0) + step;
                } else {
                    *(s32 *)(rec + 0) = *(s32 *)(rec + 0) - step;
                }

                t = *(s32 *)(rec + 0);
                if (t <= 0x400000) {                 /* 128 << 15 */
                    *(u16 *)(rec + 12) = 0;
                    if (index == 1) {
                        *(u16 *)(rec + 14) = 30;
                    }
                }
                if (t > 0x00afffff) {
                    *(u16 *)(rec + 12) = 1;
                    if (index == 1) {
                        *(u16 *)(rec + 14) = 30;
                    }
                }
            }
        } else if (index == 2) {
            /* A piece orbiting counter-clockwise. */
            step = -64;
            if (*(s16 *)(rec + 16) == 1) {
                step = step << 1;
            }
            if (*(s16 *)(rec + 16) == 2) {
                step = (step << 1) + step;
            }

            if (*(s16 *)(rec + 18) > 0) {
                Func_08009080(Func_0808a080(20), 3);
                goto collision;
            }

            Func_08009080(Func_0808a080(20), 2);

            *(s32 *)(rec + 0) = ((Func_08000118(*(s16 *)(rec + 12)) * 3) << 4)
                                + 0x700000;                  /* 224 << 15 */
            *(s32 *)(rec + 8) = ((Func_08000120(*(s16 *)(rec + 12)) * 5) << 3)
                                + 0x480000;                  /* 144 << 15 */

            *(u16 *)(rec + 12) = (u16)(*(u16 *)(rec + 12) + step);
            *(u16 *)(rec + 14) = (u16)(*(u16 *)(rec + 14) + 1);
        } else {
            /* The fourth piece orbits clockwise but only for part of its
             * 512-frame cycle; outside that window it is idle. */
            s32 phase = *(u16 *)(rec + 14) & 0x1ff;

            step = 64;
            if (*(s16 *)(rec + 16) == 1) {
                step = 128;
            }
            if (*(s16 *)(rec + 16) == 2) {
                step = (step << 1) + step;
            }

            /* The `||` is NOT one test in the assembly: each disjunct has its
             * own copy of the call (0x020013b4 and 0x02001406), so the two are
             * written out separately. */
            if (*(s16 *)(rec + 18) > 0) {
                Func_08009080(Func_0808a080(21), 3);
                goto fourth_piece_done;
            }
            if (phase <= 0x17f) {
                *(s32 *)(rec + 0) = Func_08000118(*(s16 *)(rec + 12)) * 52
                                    + 0x700000;
                *(s32 *)(rec + 8) = ((Func_08000120(*(s16 *)(rec + 12)) * 3) << 3)
                                    + 0x480000;
                *(u16 *)(rec + 12) = (u16)(*(u16 *)(rec + 12) + step);
                Func_08009080(Func_0808a080(21), 2);
                goto fourth_piece_done;
            }
            Func_08009080(Func_0808a080(21), 3);

fourth_piece_done:
            *(u16 *)(rec + 14) = (u16)(*(u16 *)(rec + 14) + 1);
        }

        /* Collision, only while the object is on the ground and this piece is
         * not already in its post-hit cooldown.  The two `goto collision` arms
         * above are the assembly's `b.n 0x02001410`: a piece in its hit
         * animation skips only its own movement, never this test. */
collision:
        if (*(s16 *)(rec + 20) == 0 && *(s32 *)(state + 8) == 0) {
            dx = (*(s32 *)(rec + 0) - *(s32 *)(state + 4)) >> 16;
            dz = (*(s32 *)(rec + 8) - *(s32 *)(state + 12)) >> 16;

            if ((dx * dx + dz * dz) <= 119 && *(s32 *)(state + 76) > 30) {
                remaining = *(s32 *)(state + 76);

                if (index <= 1) {
                    if (*(s16 *)(rec + 12) == 0) {
                        if (*(s32 *)(state + 64) < 0x30000) {
                            *(s32 *)(state + 64) = 0x30000;
                            *(s32 *)(state + 76) = remaining - 100;
                        }
                    } else {
                        if (*(s32 *)(state + 64) > -0x30000) {
                            *(s32 *)(state + 64) = -0x30000;
                            *(s32 *)(state + 76) = remaining - 100;
                        }
                    }
                } else {
                    dist = Func_030001d8(dx * dx + dz * dz);
                    *(s32 *)(state + 64) = Func_03000380(-dx * 0x30000, dist);
                    *(s32 *)(state + 72) = Func_03000380(-dz * 0x30000, dist);
                    *(s32 *)(state + 76) = *(s32 *)(state + 76) - 100;
                }

                Func_080f9010(301);
                *(u16 *)(rec + 18) = 36;
                *(u16 *)(rec + 16) =
                    (u16)Func_030003ac(*(s16 *)(rec + 16) + 1, 3);
                *(u16 *)(rec + 20) = 30;
            }
        }

        if (index == 0 || index == 1) {
            tag = *(s16 *)(rec + 16);
            Func_02000dd0(index == 0 ? 18 : 19, rec, 0,
                          Data_0200a054[tag], (tag << 4) + 16);
        } else if (index == 2) {
            tag = *(s16 *)(rec + 16);
            Func_02000dd0(20, rec, 0x8000 - *(s16 *)(rec + 12),
                          Data_0200a057[tag], (tag << 4) + 16);
        } else if (index == 3) {
            tag = *(s16 *)(rec + 16);
            Func_02000dd0(21, rec, 0xffff - *(s16 *)(rec + 12),
                          Data_0200a057[tag], (tag << 4) + 16);
        }
    }

    /* The ground shadow: current X and Z at height zero. */
    *(s32 *)(state + 52) = *(s32 *)(state + 4);
    *(s32 *)(state + 60) = *(s32 *)(state + 12);
    *(s32 *)(state + 56) = 0;

    if (Data_0200a0c0 == 1) {
        Func_02000dd0(17, state + 4, 0, 0, 16);
        Func_02000dd0(16, state + 52, 0, 0, 16);
        Func_02000dd0(15, state + 16, 0, 0, 16);
        Func_02000dd0(14, state + 28, 0, 0, 16);
        Func_02000dd0(13, state + 40, 0, 0, 16);
        Func_08009080(Func_0808a080(15), 4);
        Func_08009080(Func_0808a080(14), 4);
        Func_08009080(Func_0808a080(13), 4);
    } else {
        Func_02000dd0(12, state + 4, 0, 0, 16);
        Func_02000dd0(11, state + 52, 0, 0, 16);
        Func_02000dd0(10, state + 16, 0, 0, 16);
        Func_02000dd0(9, state + 28, 0, 0, 16);
        Func_02000dd0(8, state + 40, 0, 0, 16);
        Func_08009080(Func_0808a080(10), 4);
        Func_08009080(Func_0808a080(9), 4);
        Func_08009080(Func_0808a080(8), 4);
    }

    /* -1 is the disabled state Func_0200173c writes on entry. */
    if (*(s16 *)(state + 2) != -1) {
        *(u16 *)(state + 2) = (u16)(*(u16 *)(state + 2) + 1);
    }
}
