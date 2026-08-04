#include "types.h"

/*
 * One step of a leashed wander script: same family as
 * semantic/main/0800dd70.c ("pick a random step that stays clear of the
 * world and keeps the actor inside its leash"), same `program_00`/
 * `step_04`/`position_08`/`anchor_x_64`/`anchor_z_66` fields (all five
 * field offsets match that file's `struct Actor_0800dd70` exactly, down
 * to the `&program[step+1]` script-entry address and the `step += 4`
 * advance), and the same five callees with the same signatures. This
 * owner adds a leash PRE-check before trying to wander, widens the
 * wander fan from one probe to seven, and adds a second "flee away from
 * the anchor" loop for when wandering isn't an option.
 *
 * 796 bytes total. Full body cross-checked against
 * `arm-none-eabi-objdump -d out/full/asm/0800df04.elf` for every pool
 * address (four words: 0x0000ffff, 0x03000118, 0xffffe000, 0xffffc000),
 * resolving what an earlier read had flagged as two mystery wraparound constants --
 * they are NOT wraparound constants at all. All FIVE `if (x < 0) x +=
 * K` sites in this function use the SAME K, 0x0000ffff, exactly
 * 0800dd70.c's truncation idiom. 0xffffe000 (-8192) and 0xffffc000
 * (-16384) are two more heading-fan offsets, the negative counterparts
 * of the `128<<6` (8192) and `128<<7` (16384) offsets used two probes
 * earlier -- i.e. the fan tries straight, then a near probe, then a far
 * probe straight/+8192/-8192/+16384/-16384. That earlier park note's
 * "two different wraparound constants" claim was wrong; corrected here
 * against the actual resolved pool addresses rather than guessed
 * offsets.
 *
 * THE PRE-CHECK. Before either loop, the actor's own `position_08[0]`/
 * `[2]` (X/Z, truncated the usual way) are compared against
 * `anchor_x_64`/`anchor_z_66` -- the same anchor halfwords 0800dd70.c
 * already names, though that file only ever compares a CANDIDATE
 * position against them at the very end. Here they gate which loop
 * runs at all: only when the actor is already within
 * `radius^2` (the script entry's third word) of its anchor does loop 1
 * (wander) run; otherwise control skips straight to loop 2 (flee).
 *
 * LOOP 1 (up to seven attempts, `Func_0800d924` gates the first probe,
 * `Func_080120dc` gates the rest): each attempt draws a distance and a
 * jittered heading exactly like 0800dd70.c's single probe, but tests
 * SEVEN positions before accepting the attempt -- straight ahead at a
 * fixed 128<<12, then the random distance straight ahead, then a
 * "far" distance (random distance + 128<<12, reused across the
 * remaining four) at heading, heading+8192, heading-8192,
 * heading+16384, heading-16384. All seven must clear their gate for the
 * attempt to survive to the leash re-check below; any single failure
 * restarts the SAME attempt count from the top (matching the `bne`/`b`
 * targets landing on the loop head, not a fresh iteration).
 *
 * STALE-DELTA CARRYOVER (transcribed as found, not "fixed"): the
 * post-probe leash re-check overwrites the SAME `homeDeltaX`/
 * `homeDeltaZ` the pre-check computed, using the first (fixed-distance)
 * probe's landing point. If that re-check itself fails, control returns
 * to the top of loop 1 for another attempt WITHOUT recomputing those
 * deltas from the actor's real position -- so if the loop then runs out
 * of attempts, loop 2's flee heading is computed from the LAST failed
 * attempt's probe position, not the actor's actual position. This reads
 * as a register-reuse artifact of the original compiled code rather
 * than intended behaviour, and is kept exactly as compiled.
 *
 * LOOP 2 (up to seven attempts, same two-gate shape as 0800dd70.c's
 * single probe): heading is `Func_080044d0(homeDeltaZ, homeDeltaX) +
 * 128<<8` -- the established heading-from-delta helper (same argument
 * order as semantic/main/0800ebec.c's own call) turned by half a turn,
 * i.e. pointed away from the anchor rather than toward it -- jittered
 * the same way as loop 1's heading, then probed at a fixed 128<<12 and
 * then the random distance, no far/fan extension.
 *
 * NO DISTINCT FAILURE PATH. Unlike 0800dd70.c (which sets a `blocked`
 * flag, turns the actor around, and returns 0 when all seven candidates
 * fail), this owner has ONE shared tail for every path -- both loops'
 * success arms AND loop 2 running out of attempts all land on the same
 * `step_04 += 4; return 1`. If loop 2 also fails to find a spot, the
 * actor's position is simply left wherever it was, the script advances
 * anyway, and the function still reports success. Confirmed by reading
 * the `bgt` exit at the loop 2 head: it branches straight to the shared
 * tail, no flag write, no distinct return value.
 *
 * `flags_59` (a byte at +0x59, inside 0800dd70.c's `unknown_14[0x4a]`
 * span) is set on a loop-1 commit (`|= 2`) and cleared on a loop-2
 * commit (`&= ~2`) -- not touched on the no-op give-up path. Its
 * meaning (wandering vs. fleeing, presumably read by an animation or AI
 * state elsewhere) is not confirmed against any other reader.
 *
 * Uncertainty: none of the five shared callees are re-verified beyond
 * their established 0800dd70.c signatures; `flags_59`'s consumer is
 * not found; the stale-delta carryover above is transcribed, not
 * explained.
 */

typedef s32 (*Multiply_0800df04)(s32 a, s32 b);

struct Actor_0800df04 {
    const s32 *program_00; /* +0x00 */
    s16 step_04;            /* +0x04 */
    u16 heading_06;          /* +0x06 */
    s32 position_08[3];       /* +0x08 */
    u8 unknown_14[0x45];       /* +0x14 .. +0x58 */
    u8 flags_59;                 /* +0x59 */
    u8 unknown_5a[0x04];           /* +0x5a .. +0x5d */
    u16 blocked_5e;                  /* +0x5e, named in 0800dd70.c */
    u8 unknown_60[0x04];               /* +0x60 */
    s16 anchor_x_64;                     /* +0x64, named in 0800dd70.c */
    s16 anchor_z_66;                       /* +0x66, named in 0800dd70.c */
    u8 unknown_68[0x08];                     /* +0x68 */
};

u32 Func_08004458(void);
void Func_0800447c(s32 distance, s32 angle, s32 position[3]);
s32 Func_0800d924(const struct Actor_0800df04 *actor, const s32 position[3]);
s32 Func_080120dc(const struct Actor_0800df04 *actor, const s32 position[3]);
void Func_0800d14c(struct Actor_0800df04 *actor, s32 x, s32 y, s32 z);
s32 Func_080044d0(s32 dz, s32 dx);

s32 Func_0800df04(struct Actor_0800df04 *actor)
{
    Multiply_0800df04 multiply = (Multiply_0800df04)0x03000118;
    const s32 *entry = &actor->program_00[actor->step_04 + 1];
    s32 base = entry[0];
    s32 spread = entry[1];
    s32 radius = entry[2];
    s32 leash;
    s32 homeDeltaX, homeDeltaZ;
    s32 x, z;
    s32 attempt;

    if (radius < 0) {
        radius += 0xffff;
    }
    radius >>= 16;
    leash = radius * radius;

    x = actor->position_08[0];
    if (x < 0) {
        x += 0xffff;
    }
    homeDeltaX = (x >> 16) - actor->anchor_x_64;

    z = actor->position_08[2];
    if (z < 0) {
        z += 0xffff;
    }
    homeDeltaZ = (z >> 16) - actor->anchor_z_66;

    if (homeDeltaX * homeDeltaX + homeDeltaZ * homeDeltaZ <= leash) {
        for (attempt = 1; attempt <= 7; attempt++) {
            s32 distance = base + multiply((s32)Func_08004458(), spread);
            s32 farDistance = distance + (128 << 12);
            s32 left = (s32)Func_08004458() >> 2;
            s32 right = (s32)Func_08004458() >> 2;
            u16 heading = (u16)(actor->heading_06 + left - right);
            s32 probe[3];
            s32 far[3];

            probe[0] = actor->position_08[0];
            probe[1] = actor->position_08[1];
            probe[2] = actor->position_08[2];
            Func_0800447c(128 << 12, heading, probe);
            if (Func_0800d924(actor, probe) != 0) {
                continue;
            }

            probe[0] = actor->position_08[0];
            probe[1] = actor->position_08[1];
            probe[2] = actor->position_08[2];
            Func_0800447c(distance, heading, probe);
            if (Func_080120dc(actor, probe) != 0) {
                continue;
            }

            far[0] = actor->position_08[0];
            far[1] = actor->position_08[1];
            far[2] = actor->position_08[2];
            Func_0800447c(farDistance, heading, far);
            if (Func_080120dc(actor, far) != 0) {
                continue;
            }

            far[0] = actor->position_08[0];
            far[1] = actor->position_08[1];
            far[2] = actor->position_08[2];
            Func_0800447c(farDistance, heading + (128 << 6), far);
            if (Func_080120dc(actor, far) != 0) {
                continue;
            }

            far[0] = actor->position_08[0];
            far[1] = actor->position_08[1];
            far[2] = actor->position_08[2];
            Func_0800447c(farDistance, heading + (s32)0xffffe000, far);
            if (Func_080120dc(actor, far) != 0) {
                continue;
            }

            far[0] = actor->position_08[0];
            far[1] = actor->position_08[1];
            far[2] = actor->position_08[2];
            Func_0800447c(farDistance, heading + (128 << 7), far);
            if (Func_080120dc(actor, far) != 0) {
                continue;
            }

            far[0] = actor->position_08[0];
            far[1] = actor->position_08[1];
            far[2] = actor->position_08[2];
            Func_0800447c(farDistance, heading + (s32)0xffffc000, far);
            if (Func_080120dc(actor, far) != 0) {
                continue;
            }

            /* Re-check the FIRST probe's (fixed 128<<12, straight
             * ahead) landing point against the anchor/leash -- not the
             * "far" probes, which only exist to clear the fan. This
             * overwrites homeDeltaX/homeDeltaZ; see the stale-delta
             * note above. */
            x = probe[0];
            if (x < 0) {
                x += 0xffff;
            }
            homeDeltaX = (x >> 16) - actor->anchor_x_64;

            z = probe[2];
            if (z < 0) {
                z += 0xffff;
            }
            homeDeltaZ = (z >> 16) - actor->anchor_z_66;

            if (homeDeltaX * homeDeltaX + homeDeltaZ * homeDeltaZ <= leash) {
                actor->flags_59 |= 2;
                Func_0800d14c(actor, probe[0], probe[1], probe[2]);
                actor->step_04 += 4;
                return 1;
            }
        }
    }

    {
        s32 awayHeading = (s32)(s16)(Func_080044d0(homeDeltaZ, homeDeltaX) + (128 << 8));

        for (attempt = 1; attempt <= 7; attempt++) {
            s32 distance = base + multiply((s32)Func_08004458(), spread);
            s32 left = (s32)Func_08004458() >> 2;
            s32 right = (s32)Func_08004458() >> 2;
            u16 heading = (u16)(awayHeading + left - right);
            s32 probe[3];

            probe[0] = actor->position_08[0];
            probe[1] = actor->position_08[1];
            probe[2] = actor->position_08[2];
            Func_0800447c(128 << 12, heading, probe);
            if (Func_0800d924(actor, probe) != 0) {
                continue;
            }

            probe[0] = actor->position_08[0];
            probe[1] = actor->position_08[1];
            probe[2] = actor->position_08[2];
            Func_0800447c(distance, heading, probe);
            if (Func_080120dc(actor, probe) != 0) {
                continue;
            }

            actor->flags_59 &= ~2;
            Func_0800d14c(actor, probe[0], probe[1], probe[2]);
            break;
        }
    }

    actor->step_04 += 4;
    return 1;
}
