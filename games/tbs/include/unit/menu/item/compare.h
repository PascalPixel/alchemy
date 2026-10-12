#ifndef ALCHEMY_UNIT_MENU_ITEM_COMPARE_H
#define ALCHEMY_UNIT_MENU_ITEM_COMPARE_H

#include "types.h"

struct RenderInput;

s32 OwnerAction_RunCompareLoop(u32 mode);
s32 DjinnMenu_ShowOwnerStatus(void);
s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self);
s32 Djinn_CheckTurnBalance(s32 from, s32 to);
void Djinn_CountTurns(u8 *counts);
void Resource_LoadPairedBlocksIfAvailable(void);
void Menu_ResetTwoResourceEntries(void);
s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot);
s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id);
s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant);
s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant);

#endif
