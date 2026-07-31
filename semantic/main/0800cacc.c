#include "types.h"

/*
 * Per-frame tick over the 64-object array (`Data_03001e64`, the same
 * global `semantic/main/0800c62c.c` and `semantic/main/0800ebec.c`
 * already dereference once for the identical struct). For each live
 * object: run its script-index handler until one says "advance"
 * (returns 0) or the object is temporarily blocked; accelerate or
 * decelerate its velocity toward a target position (`pushback_38`,
 * full 3-axis or horizontal-only depending on a flag); probe the
 * ground under the candidate new position and apply a landing-impact
 * damping impulse if the probe missed badly; settle/bounce or bob the
 * Y velocity; integrate position; optionally reject the whole frame's
 * movement if the new position fails a validity check; and finally
 * turn the heading toward whichever single axis is "locked" this
 * frame, or toward the velocity direction if none is.
 *
 * 1636 bytes. Full body verified against
 * `arm-none-eabi-objdump -d out/full/asm/0800cacc.elf` across three
 * passes (this session), each catching something the previous pass
 * got wrong or missed:
 *   - pass 1: overall shape, Object_0800ebec field match, found the
 *     three physics-phase split.
 *   - pass 2: corrected a "redundant codegen" misreading of phase B's
 *     far/close split -- it is a genuine dual-precision path (see
 *     below), fully re-verified the collision/bump block, and mapped
 *     the settle+bob sub-phase.
 *   - pass 3 (this one): traced the heading-turn tail, resolved the
 *     `bounceOrPhase_44` field's apparent double duty (confirmed real,
 *     not a misread -- see below), and corrected an inverted reading
 *     of `blocked_5e`'s role (it gates the WHOLE script-dispatch loop,
 *     not something inside it).
 *
 * STRUCT. Same 0x70-byte layout as `Object_0800ebec`
 * (`semantic/main/0800ebec.c`) and `Object_0800c62c`
 * (`semantic/main/0800c62c.c`); this file declares its own local copy
 * per this project's convention, splitting a few of those files'
 * opaque spans into the individual bytes/fields this owner reads:
 *   - `class_58` (0800ebec.c's `class_58`, a u32 there) is FOUR
 *     independently-read packed bytes here: byte[0] (+0x58) gates
 *     whether a "reached target" event resets the pushback fields;
 *     byte[1] (+0x59) is 0800ebec.c's own "live/solid bits" byte, not
 *     read by this owner; byte[2] (+0x5a) gates the post-integration
 *     heading recompute; byte[3] (+0x5b) is an early-out gate before
 *     any of this owner's logic runs at all.
 *   - `blocked_5e` (already named in 0800ebec.c) is read here via
 *     `ldrsh` as a per-object script-suspend countdown: nonzero skips
 *     the ENTIRE script-dispatch loop for the frame (decrementing by
 *     one), zero lets the script run.
 *   - `bounceOrPhase_44` is a genuine union, confirmed by reading both
 *     consumers precisely rather than assuming a mismatch: in the
 *     settle sub-phase (byte 0x55 bit 1) it's read once as a signed
 *     Q16 bounce coefficient, multiplied against `velocity_24[1]`; in
 *     the bob sub-phase (byte 0x55 bit 2, mutually exclusive with the
 *     settle bit in practice though not enforced by this code) it's
 *     read, masked to its low 6 bits as a phase index into the lookup
 *     table at `0x080131c0`, and then incremented as a plain frame
 *     counter. Which real-world purpose an object actually uses
 *     depends on which of those two flag bits is set on it; not
 *     independently confirmed which objects use which.
 *   - `unknown_60`'s first byte is a retry counter, incremented when
 *     the fully-integrated candidate position fails the post-move
 *     `Func_0800d924` validity check (which also rejects the whole
 *     frame's movement for that object) and reset to 0 when it passes.
 *
 * THE FLAG BYTE AT `+0x55` (`class58Gate` below is a different field;
 * this is the byte the `+85`-offset stack cursor itself points at,
 * i.e. object+0x55 directly) drives most of the per-object branching:
 * bit 0 gates the ground-collision probe and its damping impulse; bit
 * 1 gates the Y-settle/bounce step; bit 2 gates the vertical-bob step
 * (bit 3 of the SAME byte then picks which of two divide granularities
 * the bob lookup uses); a whole-byte nonzero test (not a single bit)
 * selects phase B (horizontal-only) over phase A (full 3-axis); and a
 * separate whole-byte nonzero test at +0x61 (inside `unknown_60`)
 * skips both physics phases entirely when set. Bit 7 of `class_58[3]`
 * gates the post-integration position-validity re-check.
 *
 * PHASE A vs PHASE B, the real distinction confirmed this session:
 * phase A (byte@0x55 == 0) works all three axes and always uses the
 * IWRAM square root (0x030001d8) on wrapped/truncated deltas. Phase B
 * (byte@0x55 != 0) works X/Z only, and has TWO precision regimes for
 * its own distance check: when the coarse (wrapped, IWRAM-sqrt)
 * horizontal distance is large, it's used as-is (left-shifted back to
 * Q16); when small, the code recomputes the delta from scratch with
 * NO wraparound truncation and calls the separate main-image square
 * root (`Func_080045d4`) instead -- full precision only when it's
 * cheap (the object is close). This looked like redundant codegen on
 * first read; it is not.
 *
 * `Func_08011f54`'s return type is corrected here to `s32`: its own
 * file (`semantic/main/08011f54.c`) declares it `void`, but its
 * epilogue is the real-return-value shape (`pop {r1}` not `pop {r0}`)
 * and this owner captures and uses the result twice (against
 * `ground_14` and a cached Y target). Flagged for review rather than
 * edited in that file, since this owner's read of the CALLER side
 * doesn't independently re-verify `Func_08011f54`'s own body.
 *
 * `Data_08013624` (the 64-entry script-handler table) and
 * `Data_080131c0` (the bob lookup table) are both new to this session
 * and not cross-checked against any other reader.
 *
 * Uncertainty: `onScriptExpire_6c`'s real trigger condition (a
 * nonzero pointer at +0x6c) and purpose are inferred only from the
 * call shape; `bounceOrPhase_44`'s two real-world uses are read
 * correctly from the bytes but not independently confirmed against
 * design intent beyond that; the handler table and bob table are
 * unnamed data, not yet cross-checked.
 */

typedef struct Object_0800cacc Object_0800cacc;

struct Object_0800cacc {
    const s32 *program_00;                              /* +0x00 */
    s16 step_04;                                          /* +0x04 */
    u16 heading_06;                                         /* +0x06 */
    s32 position_08[3];                                       /* +0x08 (x,y,z) */
    s32 ground_14;                                              /* +0x14 */
    u8 unknown_18[8];                                             /* +0x18 */
    u16 radius_20;                                                  /* +0x20 */
    u16 unknown_22;                                                   /* +0x22 */
    s32 velocity_24[3];                                                 /* +0x24 (x,y,z) */
    s32 speed_30;                                                         /* +0x30 */
    s32 acceleration_34;                                                    /* +0x34 */
    s32 pushback_38[3];                                                       /* +0x38, a
                                                                                   steady
                                                                                   target here */
    s32 bounceOrPhase_44;      /* +0x44, union: see header */
    s32 settleRateOrAmplitude_48;  /* +0x48 */
    u8 unknown_4c[12];              /* +0x4c */
    u8 class_58[4];                   /* +0x58, four packed bytes, see header */
    u8 unknown_5c[2];                   /* +0x5c */
    u16 blocked_5e;                       /* +0x5e, script-suspend countdown here */
    u8 retryCounter_60;                     /* +0x60 */
    u8 unknown_61;                            /* +0x61, whole-byte "skip physics" gate */
    u8 unknown_62[2];                           /* +0x62 */
    s16 animation_timer_64;                       /* +0x64 */
    s16 animation_phase_66;                         /* +0x66 */
    u8 unknown_68[4];                                 /* +0x68 */
    void (*onScriptExpire_6c)(Object_0800cacc *);       /* +0x6c */
};

typedef s32 (*Multiply_0800cacc)(s32 a, s32 b);
typedef s32 (*IsqrtIwram_0800cacc)(s32 value);
typedef s32 (*Ratio_0800cacc)(s32 a, s32 b);

extern Object_0800cacc *Data_03001e64;
extern s32 (*Data_08013624[64])(Object_0800cacc *object);
extern const s32 Data_080131c0[];

s32 Func_080045d4(s32 sum_of_squares);
s32 Func_080044d0(s32 dz, s32 dx);
s32 Func_0800d924(const Object_0800cacc *object, const s32 position[3]);
s32 Func_08011f54(s32 index, s32 x, s32 y); /* see header: corrected from void */

void Func_0800cacc(void)
{
    Multiply_0800cacc multiply = (Multiply_0800cacc)0x03000118;
    IsqrtIwram_0800cacc isqrtIwram = (IsqrtIwram_0800cacc)0x030001d8;
    Ratio_0800cacc ratioHelper = (Ratio_0800cacc)0x0300013c;
    Object_0800cacc *object = Data_03001e64;
    s32 count;

    for (count = 64; count > 0; count--, object++) {
        s32 newX, newY, newZ;
        s32 reached = 0;

        if (object->program_00 == 0) {
            continue;
        }
        if (object->onScriptExpire_6c != 0) {
            object->onScriptExpire_6c(object);
            if (object->program_00 == 0) {
                continue;
            }
        }
        if (object->class_58[3] != 0) { /* +0x5b, early-out gate */
            continue;
        }

        if (object->blocked_5e != 0) {
            object->blocked_5e -= 1;
        } else {
            for (;;) {
                s32 entry = object->program_00[object->step_04];
                if ((u32)entry > 63) {
                    object->step_04 += 1;
                    continue;
                }
                if (Data_08013624[entry](object) != 0) {
                    continue; /* handler says stay on this step */
                }
                break; /* handler says advance */
            }
            if (object->program_00 == 0) {
                continue;
            }
        }

        newX = object->position_08[0];
        newY = object->position_08[1];
        newZ = object->position_08[2];

        /* The +0x55 byte (inside unknown_4c, not individually named
         * elsewhere) selects phase A vs phase B; +0x61 (unknown_61)
         * skips both movement phases entirely when nonzero. */
        if (object->unknown_61 == 0) {
            u8 phaseSelector = ((u8 *)object)[0x55];

            if (phaseSelector == 0) {
                /* Phase A: full 3-axis accelerate/decelerate toward pushback_38. */
                if (object->pushback_38[0] == (s32)0x80000000) {
                    s32 vx = object->velocity_24[0];
                    s32 vy = object->velocity_24[1];
                    s32 vz = object->velocity_24[2];
                    s32 speed = Func_080045d4(multiply(vx, vx) + multiply(vy, vy) + multiply(vz, vz));

                    if (speed == 0) {
                        object->velocity_24[0] = 0;
                        object->velocity_24[1] = 0;
                        object->velocity_24[2] = 0;
                    } else {
                        s32 reduced = speed - object->acceleration_34;
                        s32 ratio;
                        if (reduced < 0) {
                            reduced = 0;
                        }
                        ratio = ratioHelper(speed, reduced);
                        object->velocity_24[0] = multiply(vx, ratio);
                        object->velocity_24[1] = multiply(vy, ratio);
                        object->velocity_24[2] = multiply(vz, ratio);
                    }
                } else {
                    s32 dx = object->pushback_38[0] - newX;
                    s32 dy = object->pushback_38[1] - newY;
                    s32 dz = object->pushback_38[2] - newZ;
                    s32 dist;
                    if (dx < 0) dx += 0xffff;
                    dx >>= 16;
                    if (dy < 0) dy += 0xffff;
                    dy >>= 16;
                    if (dz < 0) dz += 0xffff;
                    dz >>= 16;
                    dist = isqrtIwram(multiply(dx, dx) + multiply(dy, dy) + multiply(dz, dz));

                    if (dist == 0) {
                        newX = object->pushback_38[0];
                        newY = object->pushback_38[1];
                        newZ = object->pushback_38[2];
                    } else {
                        s32 ratio = ratioHelper(dist << 16, object->acceleration_34);
                        s32 nvx = object->velocity_24[0] + multiply(dx, ratio);
                        s32 nvy = object->velocity_24[1] + multiply(dy, ratio);
                        s32 nvz = object->velocity_24[2] + multiply(dz, ratio);
                        s32 newSpeed;
                        object->velocity_24[0] = nvx;
                        object->velocity_24[1] = nvy;
                        object->velocity_24[2] = nvz;
                        newSpeed = Func_080045d4(multiply(nvx, nvx) + multiply(nvy, nvy) + multiply(nvz, nvz));
                        if (newSpeed > object->speed_30) {
                            s32 scale = ratioHelper(newSpeed, object->speed_30);
                            object->velocity_24[0] = multiply(nvx, scale);
                            object->velocity_24[1] = multiply(nvy, scale);
                            object->velocity_24[2] = multiply(nvz, scale);
                        }
                    }
                }
            } else {
                /* Phase B: horizontal-only (X/Z), dual-precision distance check. */
                if (object->pushback_38[0] == (s32)0x80000000) {
                    s32 vx = object->velocity_24[0];
                    s32 vz = object->velocity_24[2];
                    s32 speed = Func_080045d4(multiply(vx, vx) + multiply(vz, vz));

                    if (speed == 0) {
                        object->velocity_24[0] = 0;
                        object->velocity_24[2] = 0;
                    } else {
                        s32 reduced = speed - object->acceleration_34;
                        s32 ratio;
                        if (reduced < 0) {
                            reduced = 0;
                        }
                        ratio = ratioHelper(speed, reduced);
                        object->velocity_24[0] = multiply(vx, ratio);
                        object->velocity_24[2] = multiply(vz, ratio);
                    }
                } else {
                    s32 dxWrapped = object->pushback_38[0] - newX;
                    s32 dzWrapped = object->pushback_38[2] - newZ;
                    s32 dist;
                    if (dxWrapped < 0) dxWrapped += 0xffff;
                    dxWrapped >>= 16;
                    if (dzWrapped < 0) dzWrapped += 0xffff;
                    dzWrapped >>= 16;
                    dist = isqrtIwram(multiply(dxWrapped, dxWrapped) + multiply(dzWrapped, dzWrapped)) << 16;

                    if (dist <= 0x00ffffff) {
                        /* Close: recompute at full precision, no
                         * wraparound truncation, main-image sqrt. */
                        s32 dxRaw = object->pushback_38[0] - newX;
                        s32 dzRaw = object->pushback_38[2] - newZ;
                        dist = Func_080045d4(multiply(dxRaw, dxRaw) + multiply(dzRaw, dzRaw));
                        dxWrapped = dxRaw;
                        dzWrapped = dzRaw;
                    }

                    if (dist == 0) {
                        newX = object->pushback_38[0];
                        newZ = object->pushback_38[2];
                    } else {
                        s32 ratio = ratioHelper(dist, object->acceleration_34);
                        s32 nvx = object->velocity_24[0] + multiply(dxWrapped, ratio);
                        s32 nvz = object->velocity_24[2] + multiply(dzWrapped, ratio);
                        s32 newSpeed;
                        object->velocity_24[0] = nvx;
                        object->velocity_24[2] = nvz;
                        newSpeed = Func_080045d4(multiply(nvx, nvx) + multiply(nvz, nvz));
                        if (newSpeed > object->speed_30) {
                            s32 scale = ratioHelper(newSpeed, object->speed_30);
                            object->velocity_24[0] = multiply(nvx, scale);
                            object->velocity_24[2] = multiply(nvz, scale);
                        }
                    }
                }
            }

            /* Ground-collision probe and landing-impact damping,
             * gated by bit 0 of the +0x55 byte. */
            if (phaseSelector & 1) {
                s32 candidateX = newX + object->velocity_24[0];
                s32 candidateZ = newZ + object->velocity_24[2];
                s32 groundResult = Func_08011f54(((u8 *)object)[0x22], candidateX, candidateZ);
                s32 groundDelta = groundResult - object->ground_14;
                s32 yDelta = groundResult - newY;

                if (yDelta <= (s32)0xfffc0000) {
                    newY += groundDelta;
                }

                if (groundDelta < 0) {
                    groundDelta = -groundDelta;
                }
                {
                    s32 halfAccel = object->acceleration_34;
                    halfAccel = (halfAccel + ((u32)halfAccel >> 31)) >> 1;
                    if (groundDelta > halfAccel) {
                        groundDelta = halfAccel;
                    }
                }
                groundDelta *= 3;

                if (groundDelta != 0 && !(phaseSelector & 16)) {
                    s32 vx = object->velocity_24[0];
                    s32 vy = object->velocity_24[1];
                    s32 vz = object->velocity_24[2];
                    s32 speed = Func_080045d4(multiply(vx, vx) + multiply(vy, vy) + multiply(vz, vz));
                    if (speed != 0) {
                        s32 reduced = speed - groundDelta;
                        s32 ratio;
                        if (reduced < 0) {
                            reduced = 0;
                        }
                        ratio = ratioHelper(speed, reduced);
                        object->velocity_24[0] = multiply(vx, ratio);
                        object->velocity_24[1] = multiply(vy, ratio);
                        object->velocity_24[2] = multiply(vz, ratio);
                    }
                }

                object->ground_14 = groundResult;
            }

            /* Y-settle/bounce, gated by bit 1. */
            if (phaseSelector & 2) {
                if (newY > object->ground_14) {
                    object->velocity_24[1] -= object->settleRateOrAmplitude_48;
                } else if (object->velocity_24[1] < 0) {
                    s32 bounce;
                    s32 magnitude;
                    newY = object->ground_14;
                    bounce = -multiply(object->velocity_24[1], object->bounceOrPhase_44);
                    object->velocity_24[1] = bounce;
                    magnitude = bounce < 0 ? -bounce : bounce;
                    if (magnitude <= object->settleRateOrAmplitude_48) {
                        object->velocity_24[1] = 0;
                    }
                }
            }

            /* Vertical bob, gated by bit 2; bit 3 picks the divide
             * granularity. */
            if (phaseSelector & 4) {
                s32 phase = object->bounceOrPhase_44 & 63;
                s32 sample = Data_080131c0[phase >> 1];
                s32 bob = object->settleRateOrAmplitude_48 * sample;

                if (phaseSelector & 8) {
                    if (bob < 0) bob += 15;
                    bob >>= 4;
                } else {
                    if (bob < 0) bob += 63;
                    bob >>= 6;
                }
                object->velocity_24[1] = bob;
                object->bounceOrPhase_44 += 1;
            }
        }

        newX += object->velocity_24[0];
        newY += object->velocity_24[1];
        newZ += object->velocity_24[2];

        /* Optional post-integration validity re-check, gated by bit 7
         * of class_58[3] (+0x5b). */
        if (object->class_58[3] & 0x80) {
            s32 candidate[3];
            candidate[0] = newX;
            candidate[1] = newY;
            candidate[2] = newZ;
            if (Func_0800d924(object, candidate) != 0) {
                object->retryCounter_60 += 1;
                continue; /* reject the whole frame's movement */
            }
            object->retryCounter_60 = 0;
        }

        /* Single-axis "locked target" turn tracking, selected by the
         * +0x56 byte (16 = X, 17 = Y, 18 = Z; anything else skips). */
        {
            u8 lockedAxis = ((u8 *)object)[0x56];
            s32 target = 0, oldCoord = 0, newCoord = 0;
            s32 haveAxis = 1;

            if (lockedAxis == 16) {
                target = object->pushback_38[0];
                newCoord = newX;
                if (newCoord == target) {
                    reached = 1;
                } else {
                    oldCoord = object->position_08[0];
                }
            } else if (lockedAxis == 17) {
                target = object->pushback_38[1];
                newCoord = newY;
                if (newCoord == target) {
                    reached = 1;
                } else {
                    oldCoord = object->position_08[1];
                }
            } else if (lockedAxis == 18) {
                target = object->pushback_38[2];
                newCoord = newZ;
                if (newCoord == target) {
                    reached = 1;
                } else {
                    oldCoord = object->position_08[2];
                }
            } else {
                haveAxis = 0;
            }

            if (haveAxis && !reached) {
                s32 signOld = oldCoord - target;
                s32 signNew = newCoord - target;
                if ((signOld ^ signNew) < 0) {
                    reached = 1; /* crossed the target this frame */
                }
            }

            if (reached) {
                if (object->class_58[0] == 0) { /* +0x58 */
                    newX = object->pushback_38[0];
                    object->velocity_24[0] = 0;
                    newZ = object->pushback_38[2];
                    object->velocity_24[2] = 0;
                    if (((u8 *)object)[0x55] == 0) {
                        newY = object->pushback_38[1];
                        object->velocity_24[1] = 0;
                    }
                }
                object->pushback_38[0] = (s32)0x80000000;
                object->pushback_38[1] = (s32)0x80000000;
                object->pushback_38[2] = (s32)0x80000000;
                ((u8 *)object)[0x56] = 0;
            }
        }

        object->position_08[0] = newX;
        object->position_08[1] = newY;
        object->position_08[2] = newZ;

        /* Heading turn, gated by bit 0 of class_58[2] (+0x5a). */
        if (object->class_58[2] & 1) {
            s32 vx = object->velocity_24[0];
            s32 vz = object->velocity_24[2];
            if (vx != 0 || vz != 0) {
                s32 target = Func_080044d0(vz, vx);
                s32 turn = (s16)(target - object->heading_06);
                if (turn > (128 << 5)) {
                    turn = 128 << 5;
                }
                if (turn < (s32)0xfffff000) {
                    turn = (s32)0xfffff000;
                }
                object->heading_06 += (u16)turn;
            }
        }
    }
}
