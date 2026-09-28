#include "KORIMAKI.H"

/* The tree's scene tables, which the main image asks for through the
 * overlay's entry veneers. */
u8 *KorimaKi_GetEntrances(void)
{
    return gKorimaKiEntrances;
}

u8 *KorimaKi_GetRegions(void)
{
    return gKorimaKiRegions;
}

u8 *KorimaKi_GetExits(void)
{
    return gKorimaKiExits;
}

u8 *KorimaKi_GetPlacements(void)
{
    return gKorimaKiPlacements;
}

void PaletteScene_Initialize(void)
{
    void *scene;

    scene = *(void **)&gEventWork;
    Event_Begin();
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
    Event_RequestExit(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Event_End();
}

u8 *KorimaKi_GetEvents(void)
{
    return gKorimaKiEvents;
}
