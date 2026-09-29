#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"

void StagedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);

/* Copies the cell attributes back and marks the grid cells under actors 17 and 18. */
void BabiChika_MarkActorCells(void)
{
    Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
    StagedActor_FillGridAttributeRectangle(0, Object_GetById(17)->x.fixed >> 20, Object_GetById(17)->z.fixed >> 20, 1, 1, 0xff);
    /* FAKEMATCH: a value-returning cast sets r0 last, after the column copy into r1 */
    ((s32 (*)())StagedActor_FillGridAttributeRectangle)(0, Object_GetById(18)->x.fixed >> 20, Object_GetById(18)->z.fixed >> 20, 1, 1, 0xff);
}
