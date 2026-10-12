#ifndef ALCHEMY_UNIT_MENU_ITEM_STATUS_H
#define ALCHEMY_UNIT_MENU_ITEM_STATUS_H

#include "types.h"

struct MenuResult;
struct RenderOutput;
struct UiWindow;

s32 Menu_OpenCharacterSelector(void);
s32 CharacterSelector_Run(void);
s32 ActionMenu_Open(void);
s32 Menu_RunActionFlow(void);
s32 CharacterMenu_SelectOwner(s32 slot);
s32 DjinnMenu_ShowCurrentList(void);
s32 CharacterSelector_RunRearrange(void);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction);
void Menu_InitSelectorCursorAndEntries(s32 unused0, s32 unused1, s32 unused2, s32 unused3);
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame);
s32 CharacterMenu_SelectCommand(void);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries, s32 invert);
void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected, const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination, s32 message_variant, s32 preserve_variant);
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 unit, s32 mode);
void Runtime_RunBusyWaitDelay(void);
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);
s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id);
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index);
void Menu_BuildPatternTiles(void);
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *res);
s32 PsynergyMenu_SelectAction(void);
s32 PsynergyMenu_ActionReturnTrue(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id);
void PsynergyMenu_PreparedIconsNoOp(void);
void PsynergyMenu_PreparedIconsSecondNoOp(void);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_SelectItem(void);
s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2);
s32 Menu_ReservedStatusOne(void);
void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase);
void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index, s32 origin_x, s32 origin_y, s32 phase);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_PosCategory(void);
s32 Menu_CreateEightEntryObjects(s32 resource);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ResetCategory(void);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);

#endif
