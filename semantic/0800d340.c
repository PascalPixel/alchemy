#include "types.h"

/*
 * Core-drive row 0x0800d340, 788 bytes. This has the same shape as the
 * already reconstructed 0x0808bec0, and that comparison held: this row walks the first
 * 14 records of the SAME Data_03001e64 object array 0x0800c62c and
 * 0x0808bec0 both already used, at the same true offsets
 * semantic/main/0800ebec.c's struct Object_0800ebec establishes
 * (program_00@0, heading_06@6, position_08@8/12/16, ground_14@20,
 * velocity_24@36/40/44, speed_30@48, acceleration_34@52,
 * pushback_38@56/60/64, vertical_motion_strength@72). Locally
 * redeclared per this reconstruction's not-yet-exact include/ policy.
 *
 * Per-object physics tick, in two halves:
 *
 *   Half 1 (the previously traced piece): resolve any active pushback recoil --
 *   compute a whole-unit distance to the pushback target, and if
 *   under ~255 units, redo it at full fixed-point precision (two
 *   0x03000118 IWRAM multiplies + sqrt via the runtime's own
 *   _call_via_r2/r3/r4 thunks) and nudge velocity_24 toward the
 *   target scaled by a 0x0300013c ratio against acceleration_34,
 *   then clamp the result to speed_30 if it overshot. If there is no
 *   active pushback (pushback_38[0] == 0x80000000) but velocity is
 *   nonzero, run the same magnitude/ratio/clamp shape again as a
 *   plain deceleration toward zero, bounded by acceleration_34
 *   instead of a pushback target.
 *
 *   Half 2 (the piece this draft finishes): a Y-axis "ground stepping"
 *   adjustment gated by a status bit, a 4-way switch (byte at object
 *   +86, values 0/16/17/18) that detects whether the object's
 *   position just crossed or landed exactly on its own pushback
 *   target along one axis, a "reached target" reset that snaps
 *   pushback_38 back to the 0x80000000 sentinel on all three axes
 *   when it fires, committing the accumulated position back to
 *   position_08, and finally a heading-turn-toward-velocity step
 *   (Func_080044d0, exact, already used by semantic/main/0800ebec.c)
 *   clamped to +-0x1000 per frame -- the same turn-clamp shape
 *   0800ebec.c's own Func_0800ebec uses on the SAME heading_06 field.
 *
 * Several small flag/mode bytes near offset 85-97 are read without an
 * established name; two of them (offsets 88 and 90) land inside
 * Object_0800ebec's own named class_58 (u32 @88) as individual bytes,
 * consistent with that field's own documented "packed flags" role
 * (0800ebec.c: "byte 0x59 carries the live/solid bits"). Left as raw
 * offsets with comments, same convention as every other row this reconstruction
 * has drafted. Not yet checked byte-exact.
 */

struct Object_0800d340 {
    const void *program_00;
    u8 unknown_04[2];
    u16 heading_06;
    s32 position_08[3]; /* x, y, z */
    s32 ground_14;
    u8 unknown_18[12];
    s32 velocity_24[3];
    s32 speed_30;
    s32 acceleration_34;
    s32 pushback_38[3];
    s32 field_44; /* Uncertainty: role not established; second operand
                    * to the IWRAM multiply in the ground-step block */
    s32 vertical_motion_strength;
    u8 unknown_4c[9];
    u8 groundStepGate; /* 85: bit 1 gates the Y ground-step block */
    u8 axisMode;         /* 86: 0/16/17/18 selects which axis's
                           * pushback-target crossing to detect; also
                           * self-clears to 0 once "reached" fires */
    u8 unknown_57;
    u8 fullResetGate; /* 88: first byte of class_58 (u32 @88, already
                        * named in Object_0800ebec); gates whether
                        * "reached" also zeroes velocity and snaps
                        * position to the pushback target exactly */
    u8 unknown_59;
    u8 headingTurnGate; /* 90: third byte of class_58; bit 0 gates the
                          * final turn-toward-velocity step */
    u8 unknown_5b[6];
    u8 skipPushback; /* 97: nonzero skips the whole pushback-recoil
                       * half entirely for this object this frame */
    u8 unknown_62[14];
};

extern struct Object_0800d340 *Data_03001e64;

/* IWRAM-relocated fixed-point multiply and square root, opaque -- same
 * helpers this reconstruction already treated as opaque for 0x0800c62c,
 * 0x0808b3ec and 0x0808bec0. Called here through the runtime's own
 * _call_via_rN thunks (0x080072e4-0x08007320), already documented as
 * ordinary indirect calls by semantic/main/0800ebec.c's own header
 * comment, not retained assembly. */
extern s32 Func_03000118(s32 value, s32 multiplier);
extern s32 Func_030001d8(s32 value);
extern s32 Func_0300013c(s32 numerator, s32 denominator);

/* Already exact, semantic/main/0800ebec.c. */
extern s32 Func_080044d0(s32 dz, s32 dx);

void Func_0800d340(void)
{
    struct Object_0800d340 *object = Data_03001e64;
    s32 count = 13;

    while (count >= 0) {
        s32 x;
        s32 y;
        s32 z;
        s32 arrived = 0;

        if (object->program_00 == 0) {
            goto next_object;
        }

        x = object->position_08[0];
        y = object->position_08[1];
        z = object->position_08[2];

        if (object->skipPushback == 0) {
            if (object->pushback_38[0] != (s32)0x80000000) {
                s32 dx = object->pushback_38[0] - x;
                s32 dz = object->pushback_38[2] - z;
                s32 wholeDx = ((dx >= 0) ? dx : dx + 0xffff) >> 16;
                s32 wholeDz = ((dz >= 0) ? dz : dz + 0xffff) >> 16;
                s32 wholeDist = Func_030001d8(wholeDx * wholeDx + wholeDz * wholeDz);

                if (((u32)wholeDist << 16) <= 0xffffff) {
                    s32 fullDx = object->pushback_38[0] - x;
                    s32 fullDz = object->pushback_38[2] - z;
                    s32 fullDist = Func_030001d8(
                        Func_03000118(fullDx, fullDx) +
                        Func_03000118(fullDz, fullDz));

                    if (((u32)fullDist << 8) != 0) {
                        s32 ratio = Func_0300013c(
                            (s32)((u32)fullDist << 8), object->acceleration_34);
                        s32 newVelX = object->velocity_24[0] +
                                      Func_03000118(fullDx, ratio);
                        s32 newVelZ = object->velocity_24[2] +
                                      Func_03000118(fullDz, ratio);
                        s32 magnitude;

                        object->velocity_24[0] = newVelX;
                        object->velocity_24[2] = newVelZ;
                        magnitude = Func_030001d8(
                            Func_03000118(newVelX, newVelX) +
                            Func_03000118(newVelZ, newVelZ));

                        if (((u32)magnitude << 8) > (u32)object->speed_30) {
                            s32 clampRatio = Func_0300013c(
                                (s32)((u32)magnitude << 8), object->speed_30);

                            object->velocity_24[0] =
                                Func_03000118(newVelX, clampRatio);
                            object->velocity_24[2] =
                                Func_03000118(newVelZ, clampRatio);
                        }
                    } else {
                        /* Already at (or effectively at) the target:
                           snap x/z to it outright and leave velocity
                           untouched -- the asm skips the velocity
                           update entirely on this path rather than
                           zeroing it. */
                        x = object->pushback_38[0];
                        z = object->pushback_38[2];
                    }
                } else {
                    x = object->pushback_38[0];
                    z = object->pushback_38[2];
                }
            } else if (object->velocity_24[0] != 0 || object->velocity_24[2] != 0) {
                s32 velX = object->velocity_24[0];
                s32 velZ = object->velocity_24[2];
                s32 magnitude = Func_030001d8(
                    Func_03000118(velX, velX) + Func_03000118(velZ, velZ));

                magnitude = (s32)((u32)magnitude << 8);
                if (magnitude != 0) {
                    if ((u32)magnitude >= (u32)object->acceleration_34) {
                        s32 decelRatio =
                            Func_0300013c(magnitude, object->acceleration_34);

                        object->velocity_24[0] = Func_03000118(velX, decelRatio);
                        object->velocity_24[2] = Func_03000118(velZ, decelRatio);
                    } else {
                        object->velocity_24[0] = 0;
                        object->velocity_24[2] = 0;
                    }
                } else {
                    object->velocity_24[0] = 0;
                    object->velocity_24[2] = 0;
                }
            }
        } else {
            /* skipPushback != 0: fall straight through with x/y/z
               already loaded above, no recoil computed this frame. */
        }

        {
            s32 accumY = y;
            s32 stepDelta;

            if ((object->groundStepGate & 2) != 0) {
                if (accumY > object->ground_14) {
                    stepDelta = object->pushback_38[1] -
                                object->vertical_motion_strength;
                    object->pushback_38[1] = stepDelta;
                } else if (object->pushback_38[1] < 0) {
                    s32 scaled = Func_03000118(object->pushback_38[1], object->field_44);
                    s32 negated = -scaled;
                    s32 magnitude = (negated >= 0) ? negated : scaled;

                    object->pushback_38[1] = negated;
                    if (magnitude <= object->vertical_motion_strength) {
                        object->pushback_38[1] = 0;
                        stepDelta = 0;
                    } else {
                        stepDelta = negated;
                    }
                } else {
                    stepDelta = object->pushback_38[1];
                }
            } else {
                stepDelta = object->pushback_38[1];
            }

            accumY += stepDelta;
            x += object->velocity_24[0];
            z += object->velocity_24[2];

            switch (object->axisMode) {
            case 16: {
                s32 target = object->pushback_38[0];

                if (x == target) {
                    arrived = 1;
                } else {
                    s32 before = object->position_08[0] - target;
                    s32 after = x - target;

                    if ((before ^ after) < 0) {
                        arrived = 1;
                    }
                }
                break;
            }
            case 17: {
                s32 target = object->pushback_38[1];

                if (accumY == target) {
                    arrived = 1;
                } else {
                    s32 before = object->position_08[1] - target;
                    s32 after = accumY - target;

                    if ((before ^ after) < 0) {
                        arrived = 1;
                    }
                }
                break;
            }
            case 18: {
                s32 target = object->pushback_38[2];

                if (z == target) {
                    arrived = 1;
                } else {
                    s32 before = object->position_08[2] - target;
                    s32 after = z - target;

                    if ((before ^ after) < 0) {
                        arrived = 1;
                    }
                }
                break;
            }
            default:
                break;
            }

            if (arrived) {
                if (object->fullResetGate == 0) {
                    object->velocity_24[0] = 0;
                    object->velocity_24[2] = 0;
                    x = object->pushback_38[0];
                    z = object->pushback_38[2];

                    /* Re-reads groundStepGate (offset 85) whole, not
                       masked to bit 1 this time -- matches the asm's
                       own second, differently-tested read of the same
                       byte rather than inventing a second field. */
                    if (object->groundStepGate == 0) {
                        accumY = object->pushback_38[1];
                        object->pushback_38[1] = 0;
                    }
                }
                object->pushback_38[0] = (s32)0x80000000;
                object->pushback_38[1] = (s32)0x80000000;
                object->pushback_38[2] = (s32)0x80000000;
                object->axisMode = 0;
            }

            object->position_08[0] = x;
            object->position_08[2] = z;
            object->position_08[1] = accumY;
        }

        if ((object->headingTurnGate & 1) != 0) {
            s32 velX = object->velocity_24[0];
            s32 velZ = object->velocity_24[2];

            if (velX != 0 || velZ != 0) {
                s32 angle = Func_080044d0(velZ, velX);
                s32 turn = (s16)(angle - object->heading_06);

                if (turn > 0x1000) {
                    turn = 0x1000;
                }
                if (turn < -0x1000) {
                    turn = -0x1000;
                }
                object->heading_06 = (u16)(object->heading_06 + turn);
            }
        }

    next_object:
        count--;
        object = (struct Object_0800d340 *)((u8 *)object + 0x70);
    }
}
