#include "TYPES.H"

extern u8 gMapCellBuffer[];

extern s32 Resource_ResetEntry(u32 index);
extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(*(u16 *)(gMapCellBuffer + 0));
    Resource_ResetEntry(*(u16 *)(gMapCellBuffer + 2));
    UiWork_FinalizeFar(*(u32 *)(gMapCellBuffer + 0x1c), 2);
}
