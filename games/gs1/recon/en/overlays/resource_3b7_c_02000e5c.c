#include "types.h"

/*
 * resource_3b7 owner at 0x02000e5c, 2124 bytes (0x02000e5c-0x020016a7).
 *
 * Per-frame task of the resource_3b7 scene.  The neighbouring owner at
 * 0x020016a8 (SceneState_InitFourActorRecordsAndInstallTask) lays out the two
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
 *   Func_02001e20 -> overlay 0x02000e44 OverlayObject_SetField54
 *   Func_020022e2 -> overlay 0x02000dd0 placement helper, five arguments
 * The two sites at 0x02000f12 and 0x0200147e are not direct calls: they reach
 * 0x020019d8, which is the `bx r3` slot of the retained GCC 2.96 interworking
 * bank at 0x020019cc recorded in games/gs1/semantic/overlay-assembly.json.  They
 * are modelled here as what they are, an indirect call through a literal
 * function pointer, which is the only reading that makes the loaded image's r3
 * value (0x030001d8) meaningful.
 *
 * Uncertainties.
 *  - The divide and modulo helpers are spelled as explicit calls rather than
 *    `/` and `%`.  The reference reaches them through overlay veneers, and the
 *    approved route has no binding for the libgcc names, so the veneer symbol
 *    is the only spelling that links.  Power-of-two divisions are left as `/`
 *    because the compiler emits them inline, exactly as the reference does.
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
 * Residual.  What still differs is register assignment, not shape: the
 * reference keeps the work block in r9 and this candidate keeps it in a
 * different callee-saved register, and it additionally hoists `work + 28` -
 * shared between the history cursor and one placement argument - into r11,
 * which costs one extra saved register.  Scoping the cursor, retyping it, and
 * merging short-lived locals were each tried and changed nothing, so this is
 * left as an allocation residual rather than chased with further respellings.
 */

#define FieldScene_RunSecondaryScript Func_02000e5c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
#define SceneActor_GetRecord Func_0200280e     /* main 0x0808a080 */
#define SceneActor_SetMode Func_02002764       /* main 0x08009080 */
#define SceneActor_SetPlacement Func_020022e2  /* overlay 0x02000dd0 */
#define SceneActor_SetField54 Func_02001e20    /* overlay 0x02000e44 */
#define FixedMath_Divide Func_02002778         /* iwram 0x03000380 */
#define FixedMath_Modulo Func_02002d1e         /* iwram 0x030003ac */
#define FixedMath_Cos Func_02002bb4            /* main 0x08000118 */
#define FixedMath_Sin Func_02002bd2            /* main 0x08000120 */
#define SceneAudio_PlayCue Func_02002e7a       /* main 0x080f9010 */

u8 *SceneActor_GetRecord(s32 id);
void SceneActor_SetMode(u8 *rec, s32 mode);
void SceneActor_SetPlacement(s32 id, void *pos, s32 angle, s32 frame, s32 unk);
void SceneActor_SetField54(s32 id, s32 value);
s32 FixedMath_Divide(s32 num, s32 den);
s32 FixedMath_Modulo(s32 num, s32 den);
s32 FixedMath_Cos(s32 angle);
s32 FixedMath_Sin(s32 angle);
void SceneAudio_PlayCue(s32 cue);

/* Indirect call through the retained `bx r3` interworking slot. */
typedef s32 (*FixedMath_SqrtProc)(s32);
#define FixedMath_Sqrt ((FixedMath_SqrtProc)0x030001d8)

extern u8 Data_0200a054[]; /* three frame indices, first object bank */
extern u8 Data_0200a057[]; /* three frame indices, second object bank */
extern u8 Data_0200a070[]; /* scene work block */
extern s32 Data_0200a0c0;  /* 1 selects the first object bank */
extern u8 Data_0200a0d0[]; /* four 24-byte actor records */
extern s32 Data_0200a134;  /* set whenever the proximity band is published */
extern s32 Data_0200a138;  /* proximity band, 0 (closest) .. 4 */

#define REC_X(rec) (*(s32 *)((rec) + 0))
#define REC_Z(rec) (*(s32 *)((rec) + 8))
#define REC_HEADING(rec) (*(s16 *)((rec) + 12))
#define REC_HOLD(rec) (*(s16 *)((rec) + 14))
#define REC_MODE(rec) (*(s16 *)((rec) + 16))
#define REC_REACT(rec) (*(s16 *)((rec) + 18))
#define REC_COOL(rec) (*(s16 *)((rec) + 20))

void FieldScene_RunSecondaryScript(void)
{
    u8 *work;
    u8 *rec;
    s32 *hist;
    s32 cnt;
    s32 i;
    s32 x;
    s32 z;
    s32 y;
    s32 dx;
    s32 dz;
    s32 len;
    s32 tmp;
    s32 step;
    s32 mode;
    s32 dist;
    s32 phase;
    s32 speed;
    s32 val;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;

    work = Data_0200a070;

    /* Age the position history: +0x28 <- +0x1c <- +0x10 <- +0x04. */
    hist = (s32 *)(work + 28);
    cnt = 3;
    do {
        hist[3] = hist[0];
        hist[4] = hist[1];
        hist[5] = hist[2];
        cnt--;
        hist -= 3;
    } while (cnt != 0);

    if (*(s16 *)(work + 2) > 31) {
        *(s32 *)(work + 4) += *(s32 *)(work + 64);
        y = *(s32 *)(work + 8) + *(s32 *)(work + 68);
        *(s32 *)(work + 8) = y;
        *(s32 *)(work + 12) += *(s32 *)(work + 72);

        if (y > 0) {
            /* Still airborne: keep falling. */
            *(s32 *)(work + 68) -= 0x4000;
        } else {
            *(s32 *)(work + 8) = 0;
            if (*(s32 *)(work + 68) != 0) {
                /* Landing frame. */
                *(s32 *)(work + 68) = 0;
                if (Data_0200a0c0 == 1) {
                    SceneActor_SetMode(SceneActor_GetRecord(17), 1);
                } else {
                    SceneActor_SetMode(SceneActor_GetRecord(12), 1);
                }
            }

            if (*(s32 *)(work + 76) > 0) {
                /* Chase the fixed target at (0x780000, 0x470000). */
                dx = (0x780000 - *(s32 *)(work + 4)) >> 8;
                dz = (0x470000 - *(s32 *)(work + 12)) >> 8;
                len = FixedMath_Sqrt(dx * dx + dz * dz);
                *(s32 *)(work + 64) += FixedMath_Divide(6553 * dx, len);
                *(s32 *)(work + 72) += FixedMath_Divide(6553 * dz, len);
                *(s32 *)(work + 64) = *(s32 *)(work + 64) * 253 / 256;
                *(s32 *)(work + 72) = *(s32 *)(work + 72) * 253 / 256;
                *(s32 *)(work + 76) = *(s32 *)(work + 76) - 1;
            } else {
                /* Budget spent: coast to a stop. */
                *(s32 *)(work + 64) = *(s32 *)(work + 64) * 220 / 256;
                *(s32 *)(work + 72) = *(s32 *)(work + 72) * 220 / 256;
                if (*(s32 *)(work + 64) > -1024 && *(s32 *)(work + 64) < 1024) {
                    *(s32 *)(work + 64) = 0;
                }
                if (*(s32 *)(work + 72) > -1024 && *(s32 *)(work + 72) < 1024) {
                    *(s32 *)(work + 72) = 0;
                }
                if (*(s32 *)(work + 64) == 0 && *(s32 *)(work + 72) == 0) {
                    if (Data_0200a0c0 == 1) {
                        SceneActor_SetMode(SceneActor_GetRecord(17), 2);
                        SceneActor_SetField54(15, 0);
                        SceneActor_SetField54(14, 0);
                        SceneActor_SetField54(13, 0);
                    } else {
                        SceneActor_SetMode(SceneActor_GetRecord(12), 2);
                        SceneActor_SetField54(10, 0);
                        SceneActor_SetField54(9, 0);
                        SceneActor_SetField54(8, 0);
                    }
                    dx = (0x780000 - *(s32 *)(work + 4)) >> 16;
                    dz = (0x470000 - *(s32 *)(work + 12)) >> 16;
                    dist = dx * dx + dz * dz;
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

            /* Trapezoid bounds: the near and far edges pull the x limits in,
             * the left and right edges pull the z limits in. */
            zhi = 0x780000;
            xhi = 0xc00000;
            xlo = 0x300000;
            zlo = 0x180000;

            z = *(s32 *)(work + 12);
            if (z < 0x2a0000) {
                tmp = FixedMath_Divide((0x2a0000 - z) * 42, 18);
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
                tmp = FixedMath_Divide(z * 42 - 0x10bc0000, 18);
                xlo = 0x300000 + tmp;
                if (xlo > 0x5a0000) {
                    xlo = 0x5a0000;
                }
                xhi = 0xc00000 - tmp;
                if (xhi < 0x960000) {
                    xhi = 0x960000;
                }
            }

            x = *(s32 *)(work + 4);
            if (x < 0x5a0000) {
                tmp = FixedMath_Divide((0x5a0000 - x) * 18, 42);
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
                tmp = FixedMath_Divide(x * 18 - 0xa8c0000, 42);
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
                *(s32 *)(work + 4) = xlo;
                if (*(s32 *)(work + 64) < 0) {
                    *(s32 *)(work + 64) = -*(s32 *)(work + 64) / 2;
                }
                x = xlo;
            }
            if (x > xhi) {
                *(s32 *)(work + 4) = xhi;
                if (*(s32 *)(work + 64) > 0) {
                    *(s32 *)(work + 64) = -*(s32 *)(work + 64) / 2;
                }
            }
            z = *(s32 *)(work + 12);
            if (z < zlo) {
                *(s32 *)(work + 12) = zlo;
                if (*(s32 *)(work + 72) < 0) {
                    *(s32 *)(work + 72) = -*(s32 *)(work + 72) / 2;
                }
                z = zlo;
            }
            if (z > zhi) {
                *(s32 *)(work + 12) = zhi;
                if (*(s32 *)(work + 72) > 0) {
                    *(s32 *)(work + 72) = -*(s32 *)(work + 72) / 2;
                }
            }
        }
    }

    i = 0;
    do {
        rec = Data_0200a0d0 + i * 24;

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
                    SceneActor_SetMode(SceneActor_GetRecord(18), 3);
                } else {
                    SceneActor_SetMode(SceneActor_GetRecord(19), 3);
                }
            } else {
                if (i == 0) {
                    SceneActor_SetMode(SceneActor_GetRecord(18), 1);
                } else {
                    SceneActor_SetMode(SceneActor_GetRecord(19), 1);
                }
                if (REC_HOLD(rec) != 0) {
                    REC_HOLD(rec) = REC_HOLD(rec) - 1;
                } else {
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
                SceneActor_SetMode(SceneActor_GetRecord(20), 3);
            } else {
                SceneActor_SetMode(SceneActor_GetRecord(20), 2);
                REC_X(rec) = FixedMath_Cos(REC_HEADING(rec)) * 48 + 0x700000;
                REC_Z(rec) = FixedMath_Sin(REC_HEADING(rec)) * 40 + 0x480000;
                REC_HEADING(rec) = REC_HEADING(rec) + step;
                REC_HOLD(rec) = REC_HOLD(rec) + 1;
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
                SceneActor_SetMode(SceneActor_GetRecord(21), 3);
            } else if (phase <= 383) {
                REC_X(rec) = FixedMath_Cos(REC_HEADING(rec)) * 52 + 0x700000;
                REC_Z(rec) = FixedMath_Sin(REC_HEADING(rec)) * 24 + 0x480000;
                REC_HEADING(rec) = REC_HEADING(rec) + step;
                SceneActor_SetMode(SceneActor_GetRecord(21), 2);
            } else {
                SceneActor_SetMode(SceneActor_GetRecord(21), 3);
            }
            REC_HOLD(rec) = REC_HOLD(rec) + 1;
        }

        /* Contact test against the chased body. */
        if (REC_COOL(rec) == 0 && *(s32 *)(work + 8) == 0) {
            dx = (REC_X(rec) - *(s32 *)(work + 4)) >> 16;
            dz = (REC_Z(rec) - *(s32 *)(work + 12)) >> 16;
            dist = dx * dx + dz * dz;
            if (dist <= 119 && *(s32 *)(work + 76) > 30) {
                speed = 0x30000;
                if (i <= 1) {
                    if (REC_HEADING(rec) == 0) {
                        if (*(s32 *)(work + 64) < speed) {
                            *(s32 *)(work + 64) = speed;
                            *(s32 *)(work + 76) = *(s32 *)(work + 76) - 100;
                        }
                    } else {
                        if (*(s32 *)(work + 64) > -speed) {
                            *(s32 *)(work + 64) = -speed;
                            *(s32 *)(work + 76) = *(s32 *)(work + 76) - 100;
                        }
                    }
                } else {
                    len = FixedMath_Sqrt(dist);
                    *(s32 *)(work + 64) = FixedMath_Divide(-dx * speed, len);
                    *(s32 *)(work + 72) = FixedMath_Divide(-dz * speed, len);
                    *(s32 *)(work + 76) = *(s32 *)(work + 76) - 100;
                }
                SceneAudio_PlayCue(301);
                val = 36;
                REC_REACT(rec) = val;
                REC_MODE(rec) = FixedMath_Modulo(REC_MODE(rec) + 1, 3);
                val = 30;
                REC_COOL(rec) = val;
            }
        }

        switch (i) {
        case 0:
            SceneActor_SetPlacement(18, rec, 0, Data_0200a054[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 1:
            SceneActor_SetPlacement(19, rec, 0, Data_0200a054[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 2:
            SceneActor_SetPlacement(20, rec, 0x8000 - REC_HEADING(rec),
                                    Data_0200a057[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        case 3:
            SceneActor_SetPlacement(21, rec, 0xffff - REC_HEADING(rec),
                                    Data_0200a057[REC_MODE(rec)],
                                    (REC_MODE(rec) << 4) + 16);
            break;
        }

        i++;
    } while (i != 4);

    /* Flattened copy of the current position, then publish the body and its
     * three trailing samples. */
    *(s32 *)(work + 52) = *(s32 *)(work + 4);
    *(s32 *)(work + 56) = 0;
    *(s32 *)(work + 60) = *(s32 *)(work + 12);

    if (Data_0200a0c0 == 1) {
        SceneActor_SetPlacement(17, work + 4, 0, 0, 16);
        SceneActor_SetPlacement(16, work + 52, 0, 0, 16);
        SceneActor_SetPlacement(15, work + 16, 0, 0, 16);
        SceneActor_SetPlacement(14, work + 28, 0, 0, 16);
        SceneActor_SetPlacement(13, work + 40, 0, 0, 16);
        SceneActor_SetMode(SceneActor_GetRecord(15), 4);
        SceneActor_SetMode(SceneActor_GetRecord(14), 4);
        SceneActor_SetMode(SceneActor_GetRecord(13), 4);
    } else {
        SceneActor_SetPlacement(12, work + 4, 0, 0, 16);
        SceneActor_SetPlacement(11, work + 52, 0, 0, 16);
        SceneActor_SetPlacement(10, work + 16, 0, 0, 16);
        SceneActor_SetPlacement(9, work + 28, 0, 0, 16);
        SceneActor_SetPlacement(8, work + 40, 0, 0, 16);
        SceneActor_SetMode(SceneActor_GetRecord(10), 4);
        SceneActor_SetMode(SceneActor_GetRecord(9), 4);
        SceneActor_SetMode(SceneActor_GetRecord(8), 4);
    }

    if (*(s16 *)(work + 2) != -1) {
        *(s16 *)(work + 2) = *(s16 *)(work + 2) + 1;
    }
}
