#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/cleanup_resources_and_window.h"

extern u8 gUnk[];

extern s32 Resource_ResetEntry(u32 index);
extern void UiWindow_Close(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(*(u16 *)(gUnk + 0));
    Resource_ResetEntry(*(u16 *)(gUnk + 2));
    UiWindow_Close(*(u32 *)(gUnk + 0x1c), 2);
}
