/* The title overlay: its scene tables. */
#include "TYPES.H"

extern u8 gTitleEntrances[];
extern u8 gTitleExits[];
extern u8 gTitlePlacements[];
extern u8 gTitleEvents[];

u8 *Title_GetEntrances(void)
{
    return gTitleEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *Title_GetExits(void)
{
    return gTitleExits;
}

u8 *Title_GetPlacements(void)
{
    return gTitlePlacements;
}

u8 *Title_GetEvents(void)
{
    return gTitleEvents;
}
