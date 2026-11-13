#ifndef ALCHEMY_MODULE_SHOP_SHOP_H
#define ALCHEMY_MODULE_SHOP_SHOP_H

/// @file
/// What the shop object defines for the code outside it.

#include "types.h"

struct RenderInput;
struct ShopCursor;

void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);
void Shop_DrawMessage(s32 win, s32 message);
s32 Shop_SelectBuyCount(s32 unit, s32 item);
void Shop_BuyDone(s32 unit, s32 item, s32 quantity);
void Shop_DrawStock(struct RenderInput *window, s32 selected);
void Shop_DrawItemPrice(s32 window, s32 itemId, s32 price, s32 priceKind);
void Shop_DrawParty(struct RenderInput *window, s32 selected, s32 itemId);
void Shop_DrawCarriedItem(struct RenderInput *window, s32 unitId, s32 itemId);
void Shop_DrawEquipComparison(s32 window, s32 unitId, s32 itemId);
void Shop_GiveGameTicket(void *partyWindow, void *itemWindow);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);
void Shop_DrawMemberItems(void *window, s32 item);
void MenuCursor_Set(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind);
void Inn_PlaySleep(s32 price);
s32 Forge_ChooseMember(void);
s32 ItemPicker_Run(s32 *outUnit, s32 *outSlot, s32 itemMode);
s32 ItemPicker_SelectItem(s32 unitId);
void ItemPicker_DrawItem(void *window, s32 unitId, s32 slot);
void MenuCursor_Advance(struct ShopCursor *cursor);
void VramBlock_LoadResource(s32 slot, s32 size, s32 resource);
void ItemIconGrid_Initialize(void);
void ItemIconGrid_Release(void);
s32 ShopStock_GetRowCount(void);
s32 ShopStock_GetType(s32 row);
void ShopStock_AddArtifacts(s32 row);
void MenuCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);
void Shop_ShowKeeperMessage(s32 message);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 Shop_SelectBuy(void);
s32 Shop_ChooseMember(void);
s32 Shop_RunRepairMemberChoice(void);
s32 Sanctum_RunPartyService(void);
void Sanctum_HighlightMembers(void *window, s32 memberIndex);
void Sanctum_DrawFee(s32 window, s32 unitId);
void Sanctum_ApplyService(s32 unitId, s32 serviceKind);
void Sanctum_RunMemberIconBurst(s32 member);
s32 Shop_SelectSell(s32 unitId);
s32 Shop_SelectRepair(s32 unitId);
s32 Forge_SelectItem(s32 unitId);
s32 Shop_RepairPrice(s32 item);
s32 Forge_GetMaterialChance(s32 item);
s32 Shop_SelectSellCount(s32 unitId, s32 slot);
void Shop_RepairItem(s32 unitId, s32 slot);
s32 Forge_OfferItem(s32 unitId, s32 slot);
void Shop_SellItem(s32 unitId, s32 slot, s32 count);
void ItemIconGrid_ShowInventory(s16 *window, s32 x, s32 y, s32 owner);

#endif
