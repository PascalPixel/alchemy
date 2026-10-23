#ifndef ALCHEMY_UNIT_SHOP_SHOP_H
#define ALCHEMY_UNIT_SHOP_SHOP_H

#include "types.h"
struct UiWindow;

struct FieldEffectSprite;
struct NodeChainNode;
struct NodeChainState;
struct RenderOutput;
struct ShopCursor;

s32 Shop_ListAvailableArtifacts(void);
void Shop_StepCursor(void);
void Shop_InitializeCursorWork(void);
void Shop_ReleaseCursorWork(void);
s32 Shop_Run(s32 stockRow, s32 keeperId);
s32 Shop_OpenDebugShop(s32 unused);
void AudioCommand_WaitForStateByteClear(void);
void UiMessage_ShowAndWait(s32 messageId);
void UiMessage_ShowAndRestoreState(s32 messageId);
s32 UiMessage_ShowChoice(s32 choice);
s32 UiMessage_ShowChoiceVariant(s32 choice);
struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state);
void Shop_FillSelector(s32 itemCount, s32 paletteSelector, u8 *tileBuffer);
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition);
struct RenderOutput *Shop_CreatePriceSprite(s32 value, struct UiWindow *window,
                                            s32 x, s32 y);
void Shop_SaveAndTintFieldPalette(s32 layer);
void Shop_RestoreFieldPalette(void);
void ShopCursor_Advance(struct ShopCursor *cursor);
void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor);
void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s32 kind);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);
void Shop_PlaceCursor(struct UiWindow *window, s32 x, s32 y);

s32 Shop_SelectBuy(void);
void Shop_DrawStock(struct UiWindow *window, s32 selected);
void Shop_DrawMoney(void);
void Shop_DrawItemPrice(struct UiWindow *window, s32 itemId, s32 price,
                        s32 priceKind);
/// What Shop_DrawItemPrice says below an item whose price is 0.
enum ShopPriceKind {
  SHOP_PRICE_KIND_BUY = 0,   ///< buying: the price, even when it is 0
  SHOP_PRICE_KIND_SELL = 1,  ///< selling: the shop does not buy it
  SHOP_PRICE_KIND_REPAIR = 2 ///< repairing: it is not broken
};

void Shop_DrawMessage(struct UiWindow *window, s32 message);
void Shop_DrawParty(struct UiWindow *window, s32 selected, s32 itemId);
void Shop_DrawEquipComparison(struct UiWindow *window, s32 unitId, s32 itemId);
void Shop_DrawUnitItem(struct UiWindow *window, s32 unitId, s32 itemId);
s32 Shop_SelectBuyCount(s32 unitId, s32 itemId);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);
s32 Shop_SalePrice(s32 inventoryEntry);
s32 Shop_PickUnit(void);
s32 Shop_SelectSell(s32 unitId);
void Shop_DrawUnitGrid(struct UiWindow *window, s32 unitId);
s32 Shop_SelectSellCount(s32 unitId, s32 slot);
void Shop_SellItem(s32 unitId, s32 slot, s32 count);
s32 Shop_RepairPrice(s32 inventoryEntry);
s32 Shop_SelectUnit(void);
s32 Shop_SelectRepair(s32 unitId);
void Shop_RepairItem(s32 unitId, s32 slot);
void Shop_BuySpecialItem(struct UiWindow *window, struct UiWindow *itemWindow);

s32 ShopStock_GetRowCount(void);
void ShopStock_AddArtifacts(s32 stockRow);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 ShopStock_GetType(s32 stockRow);

s32 Shop_ServicePrice(s32 unitId, s32 serviceKind);
s32 Shop_CanServe(s32 unitId, s32 serviceKind);
s32 Shop_CountUnits(void);
s32 Shop_MessageByMode(s32 messageId);
void UiMessage_ShowResolvedAndWait(s32 messageId);
void UiMessage_ShowResolvedAndRestoreState(s32 messageId);
s32 Shop_ConfirmAct(s32 unitId);
s32 Sanctum_RunPartyService(void);
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind);
void Shop_HiliteUnit(struct UiWindow *window, s32 selected);
void Shop_DrawSelectMessage(struct UiWindow *target, s32 unitId);
void Shop_UpdateBurstSparkle(struct FieldEffectSprite *effect);
void Shop_ResetEffects(void);
void Shop_RunPartyMemberIconBurst(s32 member);

s32 Inn_RoomPrice(s32 innId);
s32 Inn_CheckIn(s32 innId, s32 objectId);
void Inn_PlaySleep(s32 roomPrice);

s32 Shop_PickUnitItem(s32 *selectedUnit, s32 *selectedSlot);
s32 Shop_SelectUse(s32 unitId);
void Shop_DrawUseItem(struct UiWindow *window, s32 unitId, s32 slot);

#endif
