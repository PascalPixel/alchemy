#include "HASHIRA.H"

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Actor_Get(10);
    s32 x;
    s32 z;

    Event_Begin();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
    Event_End();
}
