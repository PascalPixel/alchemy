#include "types.h"

extern u8 Data_02010000[];

extern s32 Resource_ResetEntry(u32 index);
extern void UiWindow_Close(u32 arg0, u32 arg1);

void BattleEffect_CleanupResourcesAndWindow(void) {
    Resource_ResetEntry(*(u16 *)(Data_02010000 + 0));
    Resource_ResetEntry(*(u16 *)(Data_02010000 + 2));
    UiWindow_Close(*(u32 *)(Data_02010000 + 0x1c), 2);
}
