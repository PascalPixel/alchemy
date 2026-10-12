#ifndef ALCHEMY_UNIT_MENU_ITEM_MENU2_H
#define ALCHEMY_UNIT_MENU_ITEM_MENU2_H

#include "types.h"

struct BtlUnit;
struct InventoryMenuState;
struct MenuResult;

s32 RunAssetSelectionScreen(void);
s32 ItemMenu_RunCommands(s32 *owner_out, s32 *target_out, s32 *item_out);
s32 ItemMenu_Use(void);
void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1);
void ItemMenu_HideAllIcons(void);
void ItemMenu_HidePageIcons(void);
void ItemMenu_Close(void);
s32 ItemMenu_PrepOwner(s32 pane);
s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items);
void ItemMenu_OpenCategory(s32 owner_id);
s32 ItemMenu_SelectTarget(s32 mode);
void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StartCompatibilityIndicators(void);
s32 ItemMenu_IsSpecial(s32 item_id);
void ItemMenu_DrawMsg(s32 unused, s32 message);
void Menu_HideEmptyEntryIcons(const u16 *items);
s32 ItemMenu_Count(s32 owner_id);
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);
s32 ItemMenu_Collect(struct BtlUnit *owner, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
void ItemMenu_RefreshOwner(s32 owner_id, s32 mode);
void InventoryMenu_NoOp(void);
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 ItemMenu_CmdCursorX(s32 column, s32 row);
s32 ItemMenu_CmdCursorY(s32 column, s32 row);
s32 ItemMenu_ChooseCmd(void);
void ItemMenu_BuildCmd(s8 *command_states);
void ItemMenu_DrawCmd(const s8 *command_states, s32 window);
s32 Item_ClassifyUseMode(s32 owner, s32 itemId);
void ItemMenu_TryBreak(void);
s32 ItemMenu_OpenDetail(s32 item_index);
s32 Menu_SelectQuantity(s32 value);
void ItemMenu_DrawItemDetails(s32 window, s32 item);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);
void ItemMenu_SetItemWin5(void);
void ItemMenu_SetItemWin3(void);
void ItemMenu_SetMsgWin5(void);
void ItemMenu_SetMsgWin6(void);
void ItemMenu_SetMsgWin7(void);
void ItemMenu_SetMsgWin3(void);
s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single);
void ItemMenu_DrawItemHead(void);
s32 ItemMenu_ConfirmDrop(s32 a0);
s32 ItemMenu_ConfirmEquip(s32 unused);
void Resource_LoadPairedBlocks(void);
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index);
s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state);
s32 ItemMenu_DrawNamePage(s32 window, s32 unused, const struct MenuResult *state);
s32 InventoryMenu_ItemNamePageReturnTrue(void);
void InventoryMenu_ItemNamePageNoOp(void);
s32 ItemMenu_RunList(s32 pane);

#endif
