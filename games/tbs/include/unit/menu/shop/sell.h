#ifndef ALCHEMY_UNIT_MENU_SHOP_SELL_H
#define ALCHEMY_UNIT_MENU_SHOP_SELL_H

#include "types.h"

struct EffectSlot;
struct NodeChainNode;
struct NodeChainState;
struct RenderOutput;
struct ShopCursor;

s32 AbilityMenu_BuildAvailableList(void);
void Shop_StepCursor(void);
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
s32 Shop_Run(s32 row, s32 keeper_id);
s32 Battle_ApplyPresetItemsAndFlags(s32 unused);
void AudioCommand_WaitForStateByteClear(void);
void UiMessage_ShowAndWait(s32 arg0);
void UiMessage_ShowAndRestoreState(s32 message_id);
s32 UiMessage_ShowChoice(s32 arg0);
s32 UiMessage_ShowChoiceVariant(s32 arg0);
struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state);
void Shop_FillSelector(s32 count, s32 selector, u8 *base);
void Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2);
struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y);
void Shop_RestoreSceneTiles(s32 layer);
void Shop_InitEffect(void);
void ShopCursor_Advance(struct ShopCursor *cursor);
void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor);
void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Shop_SelBuy(void);
void Shop_DrawStock(s32 window, s32 selected);
void Shop_DrawMoney(void);
void Shop_DrawItemPrice(s32 window, s32 item, s32 price, s32 price_mode);
void Shop_DrawMsg(s32 win, s32 mes);
void Shop_DrawParty(s32 window, s32 selected, s32 requirement);
void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id);
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id);
s32 Shop_SelBuyNum(s32 unit_id, s32 item_id);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity);
s32 Shop_ConfirmEquip(s32 unit_id, s32 slot);
s32 Shop_SellOld(s32 unit_id, s32 slot);
s32 Shop_SalePrice(s32 item_id);
s32 Shop_PickUnit(void);
s32 Shop_SelSell(s32 unit_id);
void Shop_DrawUnitGrid(s32 window, s32 unit_id);
s32 Shop_SelSellNum(s32 unit_id, s32 slot);
void Shop_SellItem(s32 unit_id, s32 slot, s32 count);
s32 Shop_RepairPrice(s32 item_id);
s32 Shop_SelUnit(void);
s32 Shop_SelRepair(s32 unit_id);
void Shop_RepairItem(s32 unit_id, s32 slot);
void Shop_BuySpecialItem(void *window, s32 item_window);
s32 EventTable_GetRowLimit(void);
void EventTable_ApplyRowAbilities(s32 row_no);
s32 EventTable_CopyRowHeader(s32 row_no, s16 *output);
s32 EventTable_GetRowType(s32 index);
s32 Shop_ServicePrice(s32 entry_no, s32 kind);
s32 Shop_CanServe(s32 entry_no, s32 kind);
s32 Shop_CountUnits(void);
s32 Shop_MsgByMode(s32 value);
void UiMessage_ShowResolvedAndWait(s32 value);
void UiMessage_ShowResolvedAndRestoreState(s32 arg0);
s32 Shop_ConfirmAct(s32 unit_id);
s32 Sanctum_RunPartyService(void);
void BattleUnit_ResetStateByMode(s32 id, s32 mode);
void Shop_HiliteUnit(s32 enabled, s32 selected);
void Shop_DrawSelMsg(s32 target, s32 selection);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
void Shop_ResetEffects(void);
void Shop_RunPartyMemberIconBurst(s32 member);
s32 Inn_RoomPrice(s32 mode);
s32 Inn_CheckIn(s32 mode, s32 object_id);
void Inn_PlaySleep(s32 room_price);
s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item);
s32 Shop_SelUse(s32 actor);
void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id);

#endif
