#ifndef ALCHEMY_UNIT_SHOP_SHOP_H
#define ALCHEMY_UNIT_SHOP_SHOP_H

#include "types.h"
struct Window;

struct FieldEffectSprite;
struct NodeChainNode;
struct NodeChainState;
struct WindowSprite;
struct MenuCursor;

s32 Shop_ListAvailableArtifacts(void);
void MenuCursor_Step(void);
void Shop_InitializeWork(void);
void Shop_ReleaseWork(void);
s32 Shop_Run(s32 stockRow, s32 keeperId);
s32 Shop_OpenDebugShop(s32 unused);
void Sound_WaitForFanfare(void);
void Shop_ShowKeeperMessage(s32 messageId);
void Shop_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Shop_AskYesNo(s32 choice);
s32 Sanctum_AskYesNo(s32 choice);
struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state);
void QuantityBar_Fill(s32 itemCount, s32 paletteSelector, u8 *tileBuffer);
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition);
struct WindowSprite *Shop_CreatePriceSprite(s32 value, struct Window *window,
                                            s32 x, s32 y);
void Shop_SaveAndTintFieldPalette(s32 layer);
void Shop_RestoreFieldPalette(void);
void MenuCursor_Advance(struct MenuCursor *cursor);
void MenuCursor_MoveTowardTarget(struct MenuCursor *cursor);
void MenuCursor_Set(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                    s32 kind);
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y);
void MenuCursor_Place(struct Window *window, s32 x, s32 y);

s32 Shop_SelectBuy(void);
void Shop_DrawStock(struct Window *window, s32 selected);
void Shop_DrawMoney(void);
void Shop_DrawItemPrice(struct Window *window, s32 itemId, s32 price,
                        s32 priceKind);
/// What Shop_DrawItemPrice says below an item whose price is 0.
enum ShopPriceKind {
  SHOP_PRICE_KIND_BUY = 0,   ///< buying: the price, even when it is 0
  SHOP_PRICE_KIND_SELL = 1,  ///< selling: the shop does not buy it
  SHOP_PRICE_KIND_REPAIR = 2 ///< repairing: it is not broken
};

void Shop_DrawMessage(struct Window *window, s32 message);
void Shop_DrawParty(struct Window *window, s32 selected, s32 itemId);
void Shop_DrawEquipComparison(struct Window *window, s32 unitId, s32 itemId);
void Shop_DrawCarriedItem(struct Window *window, s32 unitId, s32 itemId);
s32 Shop_SelectBuyCount(s32 unitId, s32 itemId);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);
s32 Shop_SalePrice(s32 inventoryEntry);
s32 Shop_ChooseMember(void);
s32 Shop_SelectSell(s32 unitId);
void Shop_DrawMemberItems(struct Window *window, s32 unitId);
s32 Shop_SelectSellCount(s32 unitId, s32 slot);
void Shop_SellItem(s32 unitId, s32 slot, s32 count);
s32 Shop_RepairPrice(s32 inventoryEntry);
s32 Shop_RunRepairMemberChoice(void);
s32 Shop_SelectRepair(s32 unitId);
void Shop_RepairItem(s32 unitId, s32 slot);
void Shop_BuySpecialItem(struct Window *window, struct Window *itemWindow);

s32 ShopStock_GetRowCount(void);
void ShopStock_AddArtifacts(s32 stockRow);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 ShopStock_GetType(s32 stockRow);

s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind);
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind);
s32 Sanctum_CountMembersInNeed(void);
s32 Shop_MessageByMode(s32 messageId);
void Sanctum_ShowKeeperMessage(s32 messageId);
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Sanctum_Run(s32 unitId);
s32 Sanctum_RunPartyService(void);
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind);
void Sanctum_HighlightMembers(struct Window *window, s32 selected);
void Shop_DrawSelectMessage(struct Window *target, s32 unitId);
void Sanctum_UpdateBurstSparkle(struct FieldEffectSprite *effect);
void Shop_ResetEffects(void);
void Sanctum_RunMemberIconBurst(s32 member);

s32 Inn_RoomPrice(s32 innId);
s32 Inn_CheckIn(s32 innId, s32 objectId);
void Inn_PlaySleep(s32 roomPrice);

s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot);
s32 Shop_SelectUse(s32 unitId);
void Shop_DrawUseItem(struct Window *window, s32 unitId, s32 slot);

#endif
