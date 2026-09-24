/* NONMATCHING: 112 of 112 bytes, 2 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 2 halfwords: in the second fill call the reference copies x into r1 before setting r0 = 0 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void StagedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);

void Local_0200252c(void)
{
    Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
    StagedActor_FillGridAttributeRectangle(0, Engine_ActorGet(17)->x.fixed >> 20, Engine_ActorGet(17)->z.fixed >> 20, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(0, Engine_ActorGet(18)->x.fixed >> 20, Engine_ActorGet(18)->z.fixed >> 20, 1, 1, 0xff);
}
