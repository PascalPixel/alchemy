/* Pin-free FourObjectMotion_UpdateBottomRow attempt: 119 native instructions,
   119 built, 117 common, differ 4 in all six TBS editions. Reading py before
   request[3] = 0 restores every native register choice and the 36-byte frame,
   but movs #8 / ldrsh r2 precede the zero-store instead of following it.
   The linked source retains
   its pin until this ordering residual is closed.
   Earlier volatile s32 limit_work differed 15 with an extra store and a
   40-byte frame; plain unbound py differed 81, a volatile signed-halfword
   read 144, an ordinary limit temporary with an empty output barrier 81,
   and an earlier signed-halfword read 26.
   2026-10-10: targeted y/phase reuse, required-store volatile carriers,
   typed/scoped zero stores, split conditional restores, known-zero early
   reads and call dependencies did not beat 4. Twenty limit-type/byte-scale
   variants and a final twelve CSE-boundary restores before the late py read
   did not beat 4. A one-minute tail-only search
   (seed 408; 4834 iterations, 3944 distinct, 78 compile failures) reached
   weighted 115 / differ 22 but did not improve this ordering residual.
   Thirty widened-read/order forms and fifteen early dead-definition or
   split-restoration forms also did not improve the four-row residual.
   Pass-directed review: py pseudo 54 spans five RTL insns with the early
   read and gets r2 in local allocation; the native-position read spans
   three and gets r3, forcing limit to r4 and changing outer allocation.
   82 new local-quantity/preference checks did not improve 4. A double zero
   store preserves py r2 and outer regs, but its live zero forces offset
   scratch r0; late dead-store elimination leaves differ 22. Scoped copies,
   statement expressions, one-pass/goto notes, unsigned/register/const
   variants and inline pointer helpers collapse to plain unpin 81. Split
   pointer/read conditions retain branches. An exact late read needs a
   different py preference while leaving r3 free for its offset scratch. */
#include "types.h"
#include "four_object_motion.h"
#include "field_sprite.h"

void Link_DrawShiftedTilePairFar(void *);
s32 Object_ApplyProjectedPlacementFar(u32 object, u32 *request, u32 *motion, u32 limit);

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
            s32 py;

            ((struct FieldSprite *)obj)->priority = 0;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + (s32)(0x10000 - phase) / 3;
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
            py = work->positions_y[i];
            request[3] = 0;
            limit = 0x8000;
            if (py >= 0)
                limit = 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
