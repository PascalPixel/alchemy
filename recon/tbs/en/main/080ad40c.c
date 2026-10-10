/* Pin-free FourObjectMotion_UpdateBottomRow attempt: 119 native instructions,
   120 built, 112 common, differ 15. The extra volatile store grows the stack
   frame from 36 to 40 bytes and changes pool/branch/stack distances; it is
   not adopted. Plain unbound py differs 81; a volatile signed-halfword read
   differs 144; an ordinary limit temporary with an empty output barrier
   differs 81. Reading a signed-halfword temporary earlier differs 26.
   The linked source retains its pin until this residual is closed. */
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
    /* FAKEMATCH: the volatile limit temporary changes register allocation
       from the plain candidate's 81 differences to 15, but adds a store. */
    volatile s32 limit_work;

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
            request[3] = 0;
            py = work->positions_y[i];
            limit_work = 0x8000;
            limit = limit_work;
            if (py >= 0)
                limit = 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
