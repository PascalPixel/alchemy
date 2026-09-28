#include "SUHARA.H"

/* The room's scene tables, which the main image asks for through the
 * overlay's entry veneers. */
u8 *SuharaHeya_GetEntrances(void)
{
    return gSuharaHeyaEntrances;
}

u8 *SuharaHeya_GetRegions(void)
{
    return gSuharaHeyaRegions;
}

u8 *SuharaHeya_GetExits(void)
{
    return gSuharaHeyaExits;
}

/* Flag 0x96f selects the later placements. */
s32 SuharaHeya_SelectPlacements(void)
{
    if (Engine_GameFlagIsSet(0x96f) != 0) {
        return (s32)gSuharaHeyaPlacements96f;
    }
    return (s32)gSuharaHeyaPlacements;
}

