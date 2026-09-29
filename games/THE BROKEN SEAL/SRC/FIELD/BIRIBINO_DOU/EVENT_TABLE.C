#include "REGION.H"

/* Two of the scene hooks the entry veneers export: no follow-up, and the
 * cave's exits. */

/* The exits, in the overlay's read-only data. */
extern u8 gBiribinoDouExits[];

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *BiribinoDou_GetExits(void)
{
    return gBiribinoDouExits;
}
