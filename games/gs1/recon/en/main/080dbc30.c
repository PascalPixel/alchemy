#include "types.h"

#define BattleEffect_RunBurstShower Func_080dbc30

/*
 * Battle-presentation effect sequence at 0x080dbc30 (1468 bytes), in the
 * same kind-39 "battle work block" family as the exact
 * games/gs1/src/battle/effects/puff_arc/run.c (owner 080d9fc8) and
 * games/gs1/src/battle/effects/work/fetch_rectangle_blitters.c (owner 080cef64).
 * The heap-allocation cache Data_03001e50[kind], the republished effect
 * argument at work + 0x7828, the sixty-four twenty-eight byte particle
 * records at work + 0x7080, the display pair at 0x7780/0x7784 and the
 * frame_ready flag at 0x7824 are all taken from those two owners.
 *
 * Unlike its siblings this owner is shared by eight presentation variants:
 * the caller passes a `mode` selector in r1 that steers the graphics it
 * loads, the palette it installs, the sprite it draws and the member
 * reactions it triggers.  Behaviour:
 *
 *   - mode 7 allocates the kind-46 and kind-47 rectangle blitters itself,
 *     with a shape pair BattleFx_FetchRectangleBlitters never produces
 *     (kind 47 as 7,7,7,2 rather than 7,7,3,3 or 7,7,7,3), and keeps both
 *     entries so the draw below can pick between them by side; every other
 *     mode takes the pair through BattleFx_FetchRectangleBlitters.
 *   - the shared sheet is decompressed into work + 0xC56.  Modes 5 and 7
 *     use their own sheets and stop there; the rest also decompress the
 *     kind-41 block and then install a palette -- mode 6 writes a sixty-four
 *     step grey ramp straight into palette RAM and clears BLDCNT, the others
 *     copy a canned palette through the word-copy entry at 0x03001388.
 *   - all sixty-four particle records are disarmed (tick -1), the source
 *     actor's screen position is fetched once and nudged per mode, and each
 *     listed member's screen position is fetched into `seat`.
 *   - sixty-four frames then run.  Mode 5 draws one large 72x62 cell of a
 *     three-cell, three-frame sequence at the source position instead of
 *     spawning; every other mode arms particle `frame` with a random
 *     scattered target taken from the member seat it is cycling through
 *     (frames 48..63 spawn nothing, so the shower tails off).  All sixty-four
 *     particle records are then walked, drawn and advanced -- in mode 5 none
 *     is ever armed, so that walk is a no-op there -- and one member per
 *     frame is sent a reaction.
 *
 * Uncertain, and left as read from the reference:
 *   - Spark.unk08 and Spark.unk14 are never touched by this owner.
 *   - the particle x seed is `pos[0] << 15` while y is `pos[1] << 16`, so x
 *     runs at half the fixed-point scale of y.  It looks like an original
 *     slip, and it is reproduced as written.
 *   - the store at work + 0x7780 is identical in both mode arms; only the
 *     value at 0x7784 differs.  The duplicate store is what the reference
 *     emits, so the if/else is kept rather than hoisted.
 *   - the role of work + 0x77A8 (set to 8 whenever a member reacts) and of
 *     the 0x04000020 write in mode 5 are only known from their use here.
 *   - the palette switch tests `cmp #4` before its five-entry table, and the
 *     table's fifth slot is the default block, so the reference's case range
 *     really does reach 4 while case 4 and the default share one body.  The
 *     empty `case 4:` label above `default:` is how that is spelled here; a
 *     plain `default:` alone would narrow the range test to 3.
 *
 * Residual: the instruction multiset is identical to the reference
 * (wrong_instructions=0) and the extent matches exactly at 1468 bytes; seven
 * halfwords differ, from two remaining interleaves where the reference issues
 * an independent instruction between the two halves of a constant
 * materialisation instead of after it (the 0x05000000 at the palette copy,
 * and the work + 0x7080 ahead of the particle loop).
 *
 * Both are post-reload scheduling decisions, and the earlier note here that
 * claimed otherwise was wrong: the canonical route runs only sched2 (the dump
 * directory has an .23.sched2 and no sched1 file), and recompiling the very
 * same preprocessed input with -fno-schedule-insns2 moves both regions, so
 * sched2 does own their order.  The grey-ramp loop above was closed by
 * writing its initialisation as `for (i = 0, pal = (u16 *)0x05000000; ...)`,
 * and that worked by feeding sched2 a different instruction order, not by
 * bypassing it: without sched2 the counter assignment sits before the pointer
 * constant, and sched2 is what lifts `mov r0, #160` over it.
 *
 * What remains is a tie in sched2's ready list that source order cannot
 * reach.  At the palette copy the ready set is {ldr r3, lsl r0, mov r2}, all
 * of equal priority, and the tie is broken by original instruction order; the
 * reference picks the pool load, which would need the callee address to be
 * emitted before the destination constant.  calls.c forbids that: the arm
 * port has SMALL_REGISTER_CLASSES, so a register argument whose rtx_cost
 * exceeds two -- which 0x05000000 does -- is copied into a pseudo in the
 * argument loop, and prepare_call_address only forces the callee address
 * afterwards.  Hoisting the callee into a local (constant-propagated back)
 * and hoisting the source pointer out of the argument list so no argument
 * contains a call were both measured and left the residual at seven.  The
 * particle-loop interleave is the same tie one block later, and there the
 * reference also issues `add r5, r9` after the hoisted 32 and 64, which asks
 * for the work-relative base to be emitted after the loop-invariant hoists
 * rather than with the assignment.  Swapping `i = 0` and `spark = SPARKS`
 * regressed to eight; the comma-and-for spelling measured seven.
 *
 * Callee spellings follow the exact sibling: plain Func_<address> for every
 * target the owner register has no distinct name for.  `alchemy inspect`
 * reports source-file basenames for several of these, which are file names
 * rather than symbol spellings, so they are not adopted here.
 */

/* Six drawn arguments: destination, source cell, x, y, width, height.
   Called through the r4 bx bank, so it is an indirect call through a
   cached blitter entry rather than a fixed callee. */
typedef void (*DrawRectangle)(void *, const void *, s32, s32, s32, s32);

/* The word-copy entry the runtime publishes at 0x03001388; reached through
   the r3 bx bank. */
typedef s32 (*CopyWords)(void *, const void *, s32);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40, 41, 46 and 47. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_00000054;
extern u8 Value_0000005a;
extern u8 Value_0000006e;
extern u8 Value_00000073;
extern u8 Value_0000007d;
extern u8 Value_0000008d;
extern u8 Value_000000a1;
extern u8 Value_000000b9;
extern u8 Value_000000ce;

void Func_080cd594(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void BattleFx_FetchRectangleBlitters(s32, u32 *);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
void Func_080041d8(s32, s32);
void Func_08004278(s32);
void Func_080e3958(s32, s32 *);
void Func_080e396c(s32, s32 *);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08004458(void);
void Func_080f9010(s32);
void Func_080b5088(s32, s32);
void Func_080b50e8(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);

/* The caller's effect state, republished at work + 0x7828. */
typedef struct Efx {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk0C;
    s32 unk10;
    s32 cnt;
    s32 variant;
    s32 unk1C;
    s32 unk20;
    s16 actors[8];
} Efx;

/* One 28-byte record; the array starts at work + 0x7080.  x and y are
   16.16 fixed point and their integer halves are read directly. */
typedef struct Spark {
    s32 x;
    s32 y;
    s32 unk08;
    s32 dx;
    s32 dy;
    s32 unk14;
    s32 tick;
} Spark;

#define WORK_EFX (*(Efx **)(work + 0x7828))
#define SHEET (work + 0xC56)
#define SPARKS ((Spark *)(work + 0x7080))

void BattleEffect_RunBurstShower(Efx *efx, s32 mode)
{
    u32 *cache;
    u8 *work;
    void *dst;
    u8 *aux;
    void *blit[2];
    s32 pos[3];
    s32 seat[8][3];
    s32 aim[3];
    Spark *spark;
    u16 *pal;
    s32 frame;
    s32 i;
    s32 pick;
    s32 lum;
    s32 id;

    cache = (u32 *)(Data_03001e50 + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (u8 *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    WORK_EFX = efx;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;

    if (mode == 7) {
        Func_080ed408(46, 7, 7, 3, 2);
        blit[0] = (void *)cache[46 - 40];
        Func_080ed408(47, 7, 7, 7, 2);
        blit[1] = (void *)cache[47 - 40];
    } else {
        BattleFx_FetchRectangleBlitters(WORK_EFX->side, (u32 *)blit);
    }

    Func_080e0524((s32)&Value_000000ce, work, 1, 0);
    if (mode == 5) {
        Func_080e0524((s32)&Value_0000005a, SHEET, 1, 1);
    } else if (mode == 7) {
        Func_080e0524((s32)&Value_00000054, SHEET, 1, 1);
    } else {
        Func_080e0524((s32)&Value_0000007d, SHEET, 1, 1);
        Func_080e0524((s32)&Value_00000073, aux, 0, 0);
        if (mode == 6) {
            for (i = 0, pal = (u16 *)0x05000000; i != 64; i++) {
                lum = i / 4;
                *pal = (u16)(((lum << 10) | (lum << 5)) | lum);
                pal++;
            }
            *(s16 *)0x04000050 = 0;
        } else {
            switch (mode) {
            case 0:
                id = (s32)&Value_0000007d;
                break;
            case 1:
                id = (s32)&Value_000000b9;
                break;
            case 2:
                id = (s32)&Value_0000006e;
                break;
            case 3:
                id = (s32)&Value_000000a1;
                break;
            case 4:
            default:
                id = (s32)&Value_0000008d;
                break;
            }
            ((CopyWords)0x03001388)((void *)0x05000000, Func_08002f40(id), 128);
        }
    }

    if (mode == 7) {
        *(s32 *)(work + 0x7780) = 2;
        *(s32 *)(work + 0x7784) = 50;
    } else {
        *(s32 *)(work + 0x7780) = 2;
        *(s32 *)(work + 0x7784) = 75;
    }
    Func_080041d8(0x080CD261, 0x480);

    for (i = 0; i != 64; i++) {
        SPARKS[i].tick = -1;
    }

    Func_080e3958(WORK_EFX->actor, pos);
    if (mode == 3) {
        pos[1] -= 16;
    }
    if (mode == 4) {
        if (WORK_EFX->side == 1) {
            pos[0] += 28;
        } else {
            pos[0] -= 28;
        }
    }
    if (mode == 7) {
        if (WORK_EFX->side == 1) {
            pos[0] += 16;
        } else {
            pos[0] -= 16;
        }
    }
    if (mode == 5) {
        pos[0] = Func_080022ec(pos[0], 3);
        *(s16 *)0x04000020 = 85;
    }

    i = 0;
    while (i != WORK_EFX->cnt) {
        Func_080e396c(WORK_EFX->actors[i], seat[i]);
        i += 1;
    }

    frame = 0;
    do {
        pick = Func_080022fc(frame, WORK_EFX->cnt);
        if (frame == 4) {
            Func_080f9010(0x88);
        }
        if (mode != 6) {
            if (frame == 24) {
                Func_080b50e8(0x86);
            }
        } else {
            if (frame == 60) {
                Func_080b50e8(0x86);
            }
        }

        if (mode == 5) {
            /* One large cell of a nine-cell sheet; the sequence advances
               every third frame and wraps every ninth. */
            if (WORK_EFX->side == 1) {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((Func_080022fc(Func_080022ec(frame, 3), 3) * 9)
                            << 9),
                    pos[0] - 2, pos[1] - 32, 72, 62);
            } else {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((Func_080022fc(Func_080022ec(frame, 3), 3) * 9)
                            << 9),
                    pos[0] - 70, pos[1] - 32, 72, 62);
            }
        } else {
            aim[0] = (seat[pick][0] + (Func_08004458() & 31)) - 16;
            aim[1] = (seat[pick][1] + (Func_08004458() & 63)) - 16;
            if (frame <= 47) {
                SPARKS[frame].x = pos[0] << 15;
                SPARKS[frame].y = pos[1] << 16;
                SPARKS[frame].dx = (aim[0] - pos[0]) << 11;
                SPARKS[frame].dy = (aim[1] - pos[1]) << 11;
                SPARKS[frame].tick = 0;
            }
        }

        i = 0;
        spark = SPARKS;
        do {
            if (spark->tick >= 0) {
                if (mode == 7) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[WORK_EFX->side])(dst, SHEET,
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, 32, 64);
                    }
                } else if (mode == 4) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, 32, 64);
                    }
                } else if (mode != 5) {
                    if (spark->tick > 1) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, 32, 64);
                    }
                }
                spark->x += spark->dx;
                spark->y += spark->dy;
                spark->tick += 1;
                if (spark->tick == 24) {
                    spark->tick = -1;
                }
            }
            i += 1;
            spark++;
        } while (i != 64);

        if (mode == 5) {
            i = 0;
            while (i != WORK_EFX->cnt) {
                if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
                    *(s32 *)(work + 0x77A8) = 8;
                    Func_080d6888(WORK_EFX->actors[i], 7, 5, i, 4);
                }
                i += 1;
            }
        } else {
            i = 0;
            while (i != WORK_EFX->cnt) {
                if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
                    *(s32 *)(work + 0x77A8) = 8;
                    if (mode == 6) {
                        Func_080d6888(WORK_EFX->actors[i], 14, 5, i, 4);
                    } else {
                        Func_080d6888(WORK_EFX->actors[i], 7, 5, i, 4);
                    }
                    Func_080b5088(WORK_EFX->actors[i], 4);
                }
                i += 1;
            }
        }

        Func_080e155c(4, 4);
        if (mode != 6) {
            Func_080cd52c();
        }
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    } while (frame != 64);

    Func_08004278(0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
