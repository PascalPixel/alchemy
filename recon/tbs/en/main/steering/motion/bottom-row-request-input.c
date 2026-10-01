/* NONMATCHING (2026-10-01): complete current EN compile/link is 252 bytes
 * against the complete 252-byte native owner, with 119 differing bytes.
 * Finite source scheduling/carrier experiment; no exact credit. The ordinary
 * game compiler and option set were used, including every emitted pool word.
 * The current preserved base remains preferable; no production source changed.
 */
/* 2026-09-30 (Venus): gMenuWork is named; a tagged memory barrier after
 * the flag store fixes that slot (6 lines left). Left: the reference loads
 * positions_y twice (for the request and again for the limit); a second
 * plain, volatile or asm-hidden read reshuffles the frame (77 to 138). */
/* Draft main:080ad40c, complete extent 252 bytes through 080ad508.
 * Bindings: gMenuWork=03001f2c; Link_DrawShiftedTilePairFar=08015418;
 * __divsi3=080022ec; Object_ApplyProjectedPlacementFar=08009008 (s32).
 * Hypothesis 3: the matched sibling's unsigned projection/scale words and
 * signed one-byte flag view, retaining the shared typed four-slot state.
 * Both scale words are set; only a nonnegative phase is written back.
 * Residual: 252/252 bytes, equal topology, 76 differing halfwords and 48
 * halfword edits; identical instructions to hypothesis 2. Vertical pseudo
 * 42 occupies r4 and spills across __divsi3; origin pseudo 131 occupies fp
 * and motion pseudo 112 occupies r9. Reference keeps vertical in sl, origin
 * at sp+4 and motion in fp/r4; flag/phase and request stores also reorder.
 * Stop: three structural hypotheses exhausted; not-yet-c, no adoption.
 * 2026-09-29 alchemy permute (8 minutes, 31,727 candidates): 1178 -> 335,
 * 11 register-only, 1 operand, 1 reordered, 2 deleted. Minimized to one
 * change: the row's y is read once into row_y for both the request and the
 * limit. That relieves the loop's registers, so vertical, origin and
 * motion land as in the reference, but the reference reads positions_y a
 * second time for the limit, after the request[3] store (the two missing
 * instructions); every re-reading spelling falls back to 1178.
 * A second 8-minute run from 335 found 290: a zero local, set before the
 * request and stored into request[3], fixes the loop tail's reload
 * registers. Remaining: only the re-read (2 register-only, 1 operand,
 * 1 reordered, 2 deleted). With the re-read the positions cursor carries
 * two more references, outranks the other induction variables and the two
 * stack-held cursors swap slots (1178 again, zero local or not).
 */
#include "FOUR_OBJECT_MOTION.H"
#include "FIXED_MATH.H"

struct MenuMotionFlags { s8 flags; };

extern struct FourObjectMotionState *gMenuWork;

void Link_DrawShiftedTilePairFar(void *);
s32 Object_ApplyProjectedPlacementFar(u32, u32 *, u32 *, u32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    u32 motion[2];
    u32 request[4];

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        u32 obj = (u32)work->objects[i];

        if (obj != 0) {
            u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;
            s32 row_y;
            /* FAKEMATCH: a zero local set before the request, as the
               reference's allocation needs. */
            u32 zero;

            ((struct MenuMotionFlags *)(obj + 9))->flags &= -13;
            asm volatile("" ::: "memory"); /* FAKEMATCH: stores the flags before the phase load */
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + __divsi3(0x10000 - phase, 3);
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            zero = 0;
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            row_y = work->positions_y[i];
            request[2] = ((u32)row_y << 16) + y;
            request[3] = zero;
            /* FAKEMATCH: expose the request publication to the second row read without emitting instructions. */
            asm volatile("" : : "m" (request) : "memory");
            limit = work->positions_y[i] < 0 ? 0x8000 : 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
