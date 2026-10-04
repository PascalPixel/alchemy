#include "TYPES.H"
#include "RESOURCE.H"

/* The world map's work, which ⚓️ keeps in its own block rather than ☀️'s map
   cell buffer: the two resource entries first and the window at 0x1c. */
extern u8 gWorldMapWork[];

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(*(u16 *)(gWorldMapWork + 0));
    Resource_ResetEntry(*(u16 *)(gWorldMapWork + 2));
    UiWork_FinalizeFar(*(u32 *)(gWorldMapWork + 0x1c), 2);
}
