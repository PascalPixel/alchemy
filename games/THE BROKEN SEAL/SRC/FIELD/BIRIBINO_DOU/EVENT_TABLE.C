#include "REGION.H"

/* Two of the scene hooks the entry veneers export: no follow-up, and the
 * cave's event table. */

/* The event table, in the overlay's read-only data. */
extern u8 BiribinoDou_EventTable[];

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetEventTable(void)
{
    return BiribinoDou_EventTable;
}
