/* NONMATCHING: reference 2124 bytes, candidate 2104, 772 differing
 * halfwords / 165 aligned edits (2026-09-27). Whole owner 02000e5c..020016a8;
 * its 22 pool words lie at 1074..1094, 136c..1384, 13a0..13a4 and
 * 168c..16a8 within resource_3b7. Initializer, placement helper, import
 * bindings and interworking sqrt calls audited against this repository/ROM.
 * Three structural hypotheses, closed:
 * 1. Retain updated dx/dz through both divide calls: 2104/1018/524;
 *    removed the reload but regressed whole-owner allocation. Rejected.
 * 2. Typed frame/actor records with indexed history: 2112/949/489;
 *    restored the stable state base plus a separate history cursor.
 * 3. Ground-first and move-first branch bodies: retained 2112/964/445,
 *    versus the original 2112/992/504. Airborne and hold tails now follow
 *    the reference topology. The countdown zero was already reused by
 *    the baseline compiler; no redundant zero-spelling trial was run.
 * 2026-09-27 arithmetic-interface transfer, one bounded model:
 * Exact COMMON/EFFECT/SPAWN.C uses ordinary C division. Own import veneers
 * 02009854/0200985c enter signed divide/modulo at 03000380/030003ac, so bind
 * __divsi3/__modsi3 there and replace all eight opaque divides plus modulo
 * with C operators. Result 2112/953/443 versus prior 2112/964/445, retained.
 * Chase velocity now survives the second divide without the former reload;
 * the collision divisor still uses r4/sp+4, not reference r2/sp+8, and the
 * reaction store still precedes modulo. Full normalized diff and all pools
 * reviewed. Exact SPRING_RIDE and TOPIC initializer confirm these state
 * blocks; PLACE_ACTOR consumes their leading x/y/z record. No missing call
 * was found. Do not follow this result with an allocation-spelling sweep.
 * Sol fountain H1: chase, final-distance and collision geometry have
 * separate block-local dx/dz/len lifetimes, as their distinct consumers in
 * the complete reference require. Prediction admitted: chase divisor sl,
 * collision speed r4/sp+4 and divisor r2/sp+8. State/index/actor now match
 * r9/r8/r6, and the chase plus final-distance bodies reproduce the reference
 * instruction sequence. Candidate 2100/768/172, retained; all 22 pool words
 * remain in order. Full normalized diff and -da/-fsched-verbose=5 dumps read.
 * Exact HAIDIA_IE/EXTENDED_SEQUENCE.C at 3c4fe6cee provides the independent
 * separate-lifetime witness; its nullable FieldActor lookup is absent here,
 * so no pointer-type transfer was applied. No new function/alignment credit.
 * H1 remaining: trapezoid x/z samples and x/z bound lifetimes; frame 12
 * versus 24 bytes; reaction store before modulo; circular counter tail.
 * Preserve these admitted geometry lifetimes in subsequent structural work.
 * H2, rejected: a pristine x sample for the far-bound and lower-edge checks,
 * plus an independent z clipping sample, predicted x's saved copy and the
 * reference high bound registers. CSE merges xpos into x; no saved copy or
 * bound allocation changes. z clipping alone changes four instructions from
 * r5 to r3, not the required r2. Still 2100/768/172; full normalized diff is
 * unchanged outside those four clipping instructions. Trial body preserved
 * at 6afa2a78f; canonical H1 bounds restored before the next model.
 * H3, prepare actor updates before publishing their fields: next_mode owns
 * the modulo result separately from the reaction constant. This reproduces
 * the complete modulo/reaction/mode/cooldown sequence without disturbing H1
 * geometry. Prepared circular heading/hold values use u16 destination width:
 * s32 produced signed loads (2108/733/180), whereas u16 restores the paired
 * unsigned loads and shared hold-store tail (2104/772/165). Retained partial
 * model; heading loads before the z store, not after it as in the reference.
 * Remaining: bounds/sample lifetimes, frame 12 versus 24, and circular load
 * scheduling/operand choice. All pools and the full normalized diff reviewed.
 * H4, rejected: inline clamp call boundaries predicted independent bound
 * rematerialization after divisions while preserving H1/H3. The first near-z
 * threshold now reloads, but CSE still shares its 0x300000 base with xlo;
 * clamp return temporaries add limit reloads/copies and move zlo to r8.
 * Result 2128/768/197, frame still 12, all 22 pool words still in order.
 * Full normalized diff plus CSE/loop/scheduler dumps read. Trial preserved at
 * bd8019bfe; canonical H3 restored. No exact credit. Closed axes: coordinate
 * copies alone and inline clamps cannot explain complete bound ownership.
 * Probe stop gate: the bounded lifetime, update-publication and clamp models
 * are exhausted; resume only with a new bound-ownership or scheduling fact.
 * H5 publication/consumer audit, stopped without a source trial:
 * Hypothesis: an independently witnessed actor-record consumer constrains
 * heading to load after z publication. Admission requires that boundary,
 * the record-2 tail below, preserved H1/H3 and all pools; adoption additionally
 * requires a whole-owner exact score, compare, coverage and staged verify.
 * Budget: one complete reference/diff and exact shared-source audit; stop
 * without a new boundary. TOPIC.C initializes disjoint x/z words at +0/+8
 * and heading/hold halfwords at +12/+14; PLACE_ACTOR.C consumes only x/y/z.
 * No call intervenes between the z store and heading load. Record 3 already
 * emits store-z then load-heading with this typed record; no alias or missing
 * consumer is established by these witnesses. No new trial was admitted.
 * Unresolved record-2 invariant at 0200135c..02001368: str r3,[r6,#8];
 * ldrh r3,[r6,#12]; adds r2,r3,r5; ldrh r3,[r6,#14]; adds r3,#1;
 * strh r2,[r6,#12]; branch to shared hold store. Canonical instead loads
 * heading into r2 before the z store and adds it after the hold load.
 * Complete normalized diff reviewed: 2104/772/165 and frame 12 versus 24
 * remain unchanged; bounds/sample lifetimes also remain unresolved. Preserve
 * canonical H1/H3 until evidence explains the ordinary source dependency.
 */
#include "TYPES.H"

/*
 * resource_3b7 owner at 0x02000e5c, 2124 bytes (0x02000e5c-0x020016a7).
 *
 * Per-frame task of the resource_3b7 scene.  The neighbouring owner at
 * 0x020016a8 (State_InitFourActorRecordsAndInstallTask) lays out the two
 * blocks this task drives and then installs 0x02008e5d - this entry - as the
 * frame callback, so the two owners share their data layout:
 *
 *   Data_0200a070  scene work block
 *       +0x02  s16  frame counter, saturating at -1
 *       +0x04  s32  chased position x  (16.16)
 *       +0x08  s32  chased position y  (16.16, 0 means grounded)
 *       +0x0c  s32  chased position z  (16.16)
 *       +0x10  s32[3] position one frame old
 *       +0x1c  s32[3] position two frames old
 *       +0x28  s32[3] position three frames old
 *       +0x34  s32[3] flattened copy of the current position (y forced to 0)
 *       +0x40  s32  velocity x
 *       +0x44  s32  velocity y
 *       +0x48  s32  velocity z
 *       +0x4c  s32  remaining chase budget
 *
 *   Data_0200a0d0  four 24-byte actor records, the same stride and field
 *                  offsets the initialiser writes
 *       +0x00  s32  position x (16.16)
 *       +0x08  s32  position z (16.16)
 *       +0x0c  s16  heading / travel direction
 *       +0x0e  s16  hold counter
 *       +0x10  s16  mode index, 0..2
 *       +0x12  s16  reaction counter
 *       +0x14  s16  cooldown counter
 *
 * The frame does four things in order: age the position history and integrate
 * the chased body, keep that body inside a trapezoid whose edges lean with the
 * depth, advance the four actors (two sliding, one on a circle, one on a
 * circle with a dwell window), and hand every record plus the position history
 * to the placement helper.
 *
 * Call bindings.  Every call word the image holds is a pre-relocation overlay
 * displacement; the loader rewrites it.  One legacy spelling per runtime target
 * is used throughout, so no alias is ambiguous:
 *   Func_0200280e -> main 0x0808a080  scene record lookup
 *   Func_02002764 -> main 0x08009080  object mode setter
 *   Func_02002778 -> iwram 0x03000380 signed divide helper
 *   Func_02002d1e -> iwram 0x030003ac signed modulo helper
 *   Func_02002bb4 -> main 0x08000118  angle -> x component
 *   Func_02002bd2 -> main 0x08000120  angle -> z component
 *   Func_02002e7a -> main 0x080f9010  sound cue
 *   Func_02001e20 -> overlay 0x02000e44 OvObj_SetField54
 *   Func_020022e2 -> overlay 0x02000dd0 placement helper, five arguments
 * The two sites at 0x02000f12 and 0x0200147e are not direct calls: they reach
 * 0x020019d8, which is the `bx r3` slot of the retained GCC 2.96 interworking
 * bank at 0x020019cc recorded in recon/tbs/semantic/overlay-assembly.json.  They
 * are modelled here as what they are, an indirect call through a literal
 * function pointer, which is the only reading that makes the loaded image's r3
 * value (0x030001d8) meaningful.
 *
 * Uncertainties.
 *  - The divide and modulo operators bind their compiler-generated libcalls
 *    to the existing overlay veneers, without changing the compiler or ABI.
 *    Power-of-two divisions emit inline, as in the reference.
 *  - Which of 0x08000118 / 0x08000120 is cosine and which is sine is inferred
 *    from the axis each result is stored to, not from the callee.
 *  - Field roles at record +0x0e, +0x12 and +0x14 are read off their uses as
 *    down-counters; only +0x12 additionally gates the "reacting" mode call.
 *  - Data_0200a054 and Data_0200a057 are two three-byte tables indexed by the
 *    record mode; they are adjacent (0x0200a054..56 and 0x0200a057..59) and
 *    are loaded through two separate pool words.
 *  - `val` holds the constants written to the halfword record fields.  On this
 *    route a halfword-typed constant store becomes `ldrh` out of the literal
 *    pool, while the reference materialises the value with `mov`; that only
 *    happens when the value reaches the store as an integer, so the original
 *    kept these in an integer temporary.  With `val` in place the candidate's
 *    literal pool holds exactly the reference's twenty-two words, in the same
 *    order, which is the strongest available check on the constants below.
 *  - `speed` likewise keeps 0x30000 in one place: the reference holds it in a
 *    register across both the comparison and the multiply, so it is one value
 *    in the source rather than four repeated literals.
 *
 * Current residuals and completed trials are recorded in the header above.
 */

/* Runtime names are bound by the single-overlay translation unit. */
u8 *Object_GetById(s32 id);
void Object_SetMode(u8 *rec, s32 mode);
void TorebiIzumi_PlaceActor(s32 id, void *pos, s32 angle, s32 frame, s32 unk);
void OverlayObject_SetField54(s32 id, s32 value);
s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);
void Engine_AudioPlayCue(s32 cue);

/* Indirect call through the retained `bx r3` interworking slot. */
typedef s32 (*FixedMath_SqrtProc)(s32);
#define FixedMath_Sqrt ((FixedMath_SqrtProc)0x030001d8)

struct SpringFrameState {
    s16 mode;
    s16 frame;
    s32 pos[4][3];
    s32 flat[3];
    s32 velocity[3];
    s32 budget;
};

struct SpringActorState {
    s32 pos[3];
    s16 heading;
    s16 hold;
    s16 mode;
    s16 react;
    s16 cool;
    s16 pad;
};

extern u8 Data_0200a054[]; /* three frame indices, first object bank */
extern u8 Data_0200a057[]; /* three frame indices, second object bank */
extern struct SpringFrameState Data_0200a070; /* scene work block */
extern s32 Data_0200a0c0;  /* 1 selects the first object bank */
extern struct SpringActorState Data_0200a0d0[]; /* four 24-byte actor records */
extern s32 Data_0200a134;  /* set whenever the proximity band is published */
extern s32 Data_0200a138;  /* proximity band, 0 (closest) .. 4 */

#define REC_X(rec) ((rec)->pos[0])
#define REC_Z(rec) ((rec)->pos[2])
#define REC_HEADING(rec) ((rec)->heading)
#define REC_HOLD(rec) ((rec)->hold)
#define REC_MODE(rec) ((rec)->mode)
#define REC_REACT(rec) ((rec)->react)
#define REC_COOL(rec) ((rec)->cool)

void FieldScene_RunSecondaryScript(void)
{
    struct SpringFrameState *work;
    struct SpringActorState *rec;
    s32 i;
    s32 x;
    s32 z;
    s32 y;
    s32 tmp;
    s32 step;
    s32 mode;
    s32 phase;
    s32 val;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;

    work = &Data_0200a070;
    i = 3;
    do {
        work->pos[i][0] = work->pos[i - 1][0];
        work->pos[i][1] = work->pos[i - 1][1];
        work->pos[i][2] = work->pos[i - 1][2];
        i--;
    } while (i != 0);

    if (work->frame > 31) {
        work->pos[0][0] += work->velocity[0];
        y = work->pos[0][1] + work->velocity[1];
        work->pos[0][1] = y;
        work->pos[0][2] += work->velocity[2];

        if (y <= 0) {
            work->pos[0][1] = 0;
            if (work->velocity[1] != 0) {
                /* Landing frame. */
                work->velocity[1] = 0;
                if (Data_0200a0c0 == 1) {
                    Object_SetMode(Object_GetById(17), 1);
                } else {
                    Object_SetMode(Object_GetById(12), 1);
                }
            }

            if (work->budget > 0) {
                s32 dx;
                s32 dz;
                s32 len;

                /* Chase the fixed target at (0x780000, 0x470000). */
                dx = (0x780000 - work->pos[0][0]) >> 8;
                dz = (0x470000 - work->pos[0][2]) >> 8;
                len = FixedMath_Sqrt(dx * dx + dz * dz);
                work->velocity[0] += 6553 * dx / len;
                work->velocity[2] += 6553 * dz / len;
                work->velocity[0] = work->velocity[0] * 253 / 256;
                work->velocity[2] = work->velocity[2] * 253 / 256;
                work->budget = work->budget - 1;
            } else {
                /* Budget spent: coast to a stop. */
                work->velocity[0] = work->velocity[0] * 220 / 256;
                work->velocity[2] = work->velocity[2] * 220 / 256;
                if (work->velocity[0] > -1024 && work->velocity[0] < 1024) {
                    work->velocity[0] = 0;
                }
                if (work->velocity[2] > -1024 && work->velocity[2] < 1024) {
                    work->velocity[2] = 0;
                }
                if (work->velocity[0] == 0 && work->velocity[2] == 0) {
                    if (Data_0200a0c0 == 1) {
                        Object_SetMode(Object_GetById(17), 2);
                        OverlayObject_SetField54(15, 0);
                        OverlayObject_SetField54(14, 0);
                        OverlayObject_SetField54(13, 0);
                    } else {
                        Object_SetMode(Object_GetById(12), 2);
                        OverlayObject_SetField54(10, 0);
                        OverlayObject_SetField54(9, 0);
                        OverlayObject_SetField54(8, 0);
                    }
                    {
                        s32 dx = (0x780000 - work->pos[0][0]) >> 16;
                        s32 dz = (0x470000 - work->pos[0][2]) >> 16;
                        s32 dist = dx * dx + dz * dz;

                        Data_0200a134 = 1;
                        if (dist <= 224) {
                            Data_0200a138 = 0;
                        } else if (dist <= 624) {
                            Data_0200a138 = 1;
                        } else if (dist <= 1088) {
                            Data_0200a138 = 2;
                        } else if (dist <= 1680) {
                            Data_0200a138 = 3;
                        } else {
                            Data_0200a138 = 4;
                        }
                    }
                }
            }

            /* Trapezoid bounds: the near and far edges pull the x limits in,
             * the left and right edges pull the z limits in. */
            zhi = 0x780000;
            xhi = 0xc00000;
            xlo = 0x300000;
            zlo = 0x180000;

            z = work->pos[0][2];
            if (z < 0x2a0000) {
                tmp = (0x2a0000 - z) * 42 / 18;
                xlo = 0x300000 + tmp;
                if (xlo > 0x5a0000) {
                    xlo = 0x5a0000;
                }
                xhi = 0xc00000 - tmp;
                if (xhi < 0x960000) {
                    xhi = 0x960000;
                }
            }
            if (z > 0x660000) {
                tmp = (z * 42 - 0x10bc0000) / 18;
                xlo = 0x300000 + tmp;
                if (xlo > 0x5a0000) {
                    xlo = 0x5a0000;
                }
                xhi = 0xc00000 - tmp;
                if (xhi < 0x960000) {
                    xhi = 0x960000;
                }
            }

            x = work->pos[0][0];
            if (x < 0x5a0000) {
                tmp = (0x5a0000 - x) * 18 / 42;
                zlo = 0x180000 + tmp;
                if (zlo > 0x2a0000) {
                    zlo = 0x2a0000;
                }
                zhi = 0x780000 - tmp;
                if (zhi < 0x660000) {
                    zhi = 0x660000;
                }
            }
            if (x > 0x960000) {
                tmp = (x * 18 - 0xa8c0000) / 42;
                zlo = 0x180000 + tmp;
                if (zlo > 0x2a0000) {
                    zlo = 0x2a0000;
                }
                zhi = 0x780000 - tmp;
                if (zhi < 0x660000) {
                    zhi = 0x660000;
                }
            }

            /* Bounce off each edge with half the incoming speed. */
            if (x < xlo) {
                work->pos[0][0] = xlo;
                if (work->velocity[0] < 0) {
                    work->velocity[0] = -work->velocity[0] / 2;
                }
                x = xlo;
            }
            if (x > xhi) {
                work->pos[0][0] = xhi;
                if (work->velocity[0] > 0) {
                    work->velocity[0] = -work->velocity[0] / 2;
                }
            }
            z = work->pos[0][2];
            if (z < zlo) {
                work->pos[0][2] = zlo;
                if (work->velocity[2] < 0) {
                    work->velocity[2] = -work->velocity[2] / 2;
                }
                z = zlo;
            }
            if (z > zhi) {
                work->pos[0][2] = zhi;
                if (work->velocity[2] > 0) {
                    work->velocity[2] = -work->velocity[2] / 2;
                }
            }
        } else {
            /* Still airborne: keep falling. */
            work->velocity[1] -= 0x4000;
        }
    }

    i = 0;
    do {
        rec = &Data_0200a0d0[i];

        if (REC_REACT(rec) > 0) {
            REC_REACT(rec) = REC_REACT(rec) - 1;
        }
        if (REC_COOL(rec) > 0) {
            REC_COOL(rec) = REC_COOL(rec) - 1;
        }

        if (i <= 1) {
            /* Records 0 and 1 slide back and forth along x. */
            mode = REC_MODE(rec);
            step = 0x10000;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (REC_REACT(rec) > 0) {
                if (i == 0) {
                    Object_SetMode(Object_GetById(18), 3);
                } else {
                    Object_SetMode(Object_GetById(19), 3);
                }
            } else {
                if (i == 0) {
                    Object_SetMode(Object_GetById(18), 1);
                } else {
                    Object_SetMode(Object_GetById(19), 1);
                }
                if (REC_HOLD(rec) == 0) {
                    if (REC_HEADING(rec) == 0) {
                        REC_X(rec) = REC_X(rec) + step;
                    } else {
                        REC_X(rec) = REC_X(rec) - step;
                    }
                    if (REC_X(rec) <= 0x400000) {
                        val = 0;
                        REC_HEADING(rec) = val;
                        if (i == 1) {
                            val = 30;
                            REC_HOLD(rec) = val;
                        }
                    }
                    if (REC_X(rec) > 0xafffff) {
                        val = 1;
                        REC_HEADING(rec) = val;
                        if (i == 1) {
                            val = 30;
                            REC_HOLD(rec) = val;
                        }
                    }
                } else {
                    REC_HOLD(rec) = REC_HOLD(rec) - 1;
                }
            }
        } else if (i == 2) {
            /* Record 2 turns clockwise on a 48 x 40 ellipse. */
            mode = REC_MODE(rec);
            step = -64;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (REC_REACT(rec) > 0) {
                Object_SetMode(Object_GetById(20), 3);
            } else {
                u16 heading;
                u16 hold;

                Object_SetMode(Object_GetById(20), 2);
                REC_X(rec) = Engine_MathCos(REC_HEADING(rec)) * 48 + 0x700000;
                REC_Z(rec) = Engine_MathSin(REC_HEADING(rec)) * 40 + 0x480000;
                heading = REC_HEADING(rec) + step;
                hold = REC_HOLD(rec) + 1;
                REC_HEADING(rec) = heading;
                REC_HOLD(rec) = hold;
            }
        } else {
            /* Record 3 turns the other way and rests for the last 128 counts
             * of every 512-count lap. */
            phase = REC_HOLD(rec) & 511;
            mode = REC_MODE(rec);
            step = 64;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (REC_REACT(rec) > 0) {
                Object_SetMode(Object_GetById(21), 3);
            } else if (phase <= 383) {
                REC_X(rec) = Engine_MathCos(REC_HEADING(rec)) * 52 + 0x700000;
                REC_Z(rec) = Engine_MathSin(REC_HEADING(rec)) * 24 + 0x480000;
                REC_HEADING(rec) = REC_HEADING(rec) + step;
                Object_SetMode(Object_GetById(21), 2);
            } else {
                Object_SetMode(Object_GetById(21), 3);
            }
            REC_HOLD(rec) = REC_HOLD(rec) + 1;
        }

        /* Contact test against the chased body. */
        if (REC_COOL(rec) == 0 && work->pos[0][1] == 0) {
            s32 dx;
            s32 dz;
            s32 dist;

            dx = (REC_X(rec) - work->pos[0][0]) >> 16;
            dz = (REC_Z(rec) - work->pos[0][2]) >> 16;
            dist = dx * dx + dz * dz;
            if (dist <= 119 && work->budget > 30) {
                s32 speed;
                s32 next_mode;

                speed = 0x30000;
                if (i <= 1) {
                    if (REC_HEADING(rec) == 0) {
                        if (work->velocity[0] < speed) {
                            work->velocity[0] = speed;
                            work->budget = work->budget - 100;
                        }
                    } else {
                        if (work->velocity[0] > -speed) {
                            work->velocity[0] = -speed;
                            work->budget = work->budget - 100;
                        }
                    }
                } else {
                    s32 len;

                    len = FixedMath_Sqrt(dist);
                    work->velocity[0] = -dx * speed / len;
                    work->velocity[2] = -dz * speed / len;
                    work->budget = work->budget - 100;
                }
                Engine_AudioPlayCue(301);
                next_mode = (REC_MODE(rec) + 1) % 3;
                val = 36;
                REC_REACT(rec) = val;
                REC_MODE(rec) = next_mode;
                val = 30;
                REC_COOL(rec) = val;
            }
        }

        switch (i) {
        case 0:
            TorebiIzumi_PlaceActor(18, rec, 0, Data_0200a054[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 1:
            TorebiIzumi_PlaceActor(19, rec, 0, Data_0200a054[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 2:
            TorebiIzumi_PlaceActor(20, rec, 0x8000 - REC_HEADING(rec),
                                    Data_0200a057[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 3:
            TorebiIzumi_PlaceActor(21, rec, 0xffff - REC_HEADING(rec),
                                    Data_0200a057[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        }

        i++;
    } while (i != 4);

    /* Flattened copy of the current position, then publish the body and its
     * three trailing samples. */
    work->flat[0] = work->pos[0][0];
    work->flat[1] = 0;
    work->flat[2] = work->pos[0][2];

    if (Data_0200a0c0 == 1) {
        TorebiIzumi_PlaceActor(17, work->pos[0], 0, 0, 16);
        TorebiIzumi_PlaceActor(16, work->flat, 0, 0, 16);
        TorebiIzumi_PlaceActor(15, work->pos[1], 0, 0, 16);
        TorebiIzumi_PlaceActor(14, work->pos[2], 0, 0, 16);
        TorebiIzumi_PlaceActor(13, work->pos[3], 0, 0, 16);
        Object_SetMode(Object_GetById(15), 4);
        Object_SetMode(Object_GetById(14), 4);
        Object_SetMode(Object_GetById(13), 4);
    } else {
        TorebiIzumi_PlaceActor(12, work->pos[0], 0, 0, 16);
        TorebiIzumi_PlaceActor(11, work->flat, 0, 0, 16);
        TorebiIzumi_PlaceActor(10, work->pos[1], 0, 0, 16);
        TorebiIzumi_PlaceActor(9, work->pos[2], 0, 0, 16);
        TorebiIzumi_PlaceActor(8, work->pos[3], 0, 0, 16);
        Object_SetMode(Object_GetById(10), 4);
        Object_SetMode(Object_GetById(9), 4);
        Object_SetMode(Object_GetById(8), 4);
    }

    if (work->frame != -1) {
        work->frame = work->frame + 1;
    }
}
