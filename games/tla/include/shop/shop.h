#ifndef ALCHEMY_SHOP_SHOP_H
#define ALCHEMY_SHOP_SHOP_H

/// @file
/// What the shop object defines for the code outside it.

#include "types.h"

struct Window;
struct MenuCursor;
struct WindowSprite;

/// The colours of a quantity bar's segments. QuantityBar_Fill paints the first
/// so many segments in one, and the bar is built by painting shorter runs over
/// longer: the whole bar out of reach, then what could be chosen, then what is
/// chosen, then what the party member already carries.
#define QUANTITY_BAR_OUT_OF_REACH 14
#define QUANTITY_BAR_OPEN 0
#define QUANTITY_BAR_CHOSEN 10
#define QUANTITY_BAR_HELD 2

/// Rows of the shop stock table, as an event passes them to Shop_Run: Daila's
/// weapon, armor and item shops, and Izumo's general store, which the debug
/// rooms' shopkeepers also open. A town no other room opens a shop of names
/// its rows itself.
enum {
  SHOP_DAILA_WEAPONS = 1,
  SHOP_DAILA_ARMOR = 2,
  SHOP_DAILA_ITEMS = 3,
  SHOP_IZUMO = 24
};

/// The lines of one shop, as many as the weapon shop's, and of a sanctum, as
/// the debug rooms' message browsers count them (text/msg_ids.h).
#define SHOP_MESSAGES (MsgArmorShopWelcome - MsgWeaponShopWelcome)
#define SANCTUM_MESSAGES (MsgRightWhenI - MsgWelcomeWearyWanderers)

/// Daila's inn, as Inn_CheckIn numbers the inns, which the debug rooms'
/// innkeepers also open. A town no other room opens an inn of names its inn
/// itself.
enum { DAILA_INN = 0 };

void Shop_InitializeState(void);
void Shop_ReleaseState(void);
s32 Shop_SelectBuy(void);
void Shop_DrawStock(struct Window *window, s32 selected);
void Shop_DrawItemPrice(struct Window *window, s32 itemId, s32 price,
                        s32 priceKind);
void Shop_DrawMessage(struct Window *window, s32 message);
void Shop_DrawParty(struct Window *window, s32 selected, s32 itemId);
void Shop_DrawEquipComparison(struct Window *window, s32 unitId, s32 itemId);
void Shop_DrawCarriedItem(struct Window *window, s32 unitId, s32 itemId);
s32 Shop_SelectBuyCount(s32 unitId, s32 itemId);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);
void Shop_GiveGameTicket(struct Window *window, struct Window *itemWindow);
s32 Shop_ChooseMember(void);
s32 Shop_SelectSell(s32 unitId);
void Shop_DrawMemberItems(struct Window *window, s32 unitId);
s32 Shop_SelectSellCount(s32 unitId, s32 slot);
void Shop_SellItem(s32 unitId, s32 slot, s32 count);
s32 Shop_RepairPrice(s32 inventoryEntry);
s32 Shop_RunRepairMemberChoice(void);
s32 Shop_SelectRepair(s32 unitId);
void Shop_RepairItem(s32 unitId, s32 slot);
void Shop_ShowKeeperMessage(s32 messageId);
struct WindowSprite *Shop_CreatePriceSprite(s32 value, struct Window *window,
                                            s32 x, s32 y);

void MenuCursor_Set(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                    s8 frames);
void MenuCursor_Advance(struct MenuCursor *cursor);
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y);
void VramBlock_LoadResource(s32 entryId, s32 size, s32 resourceId);

s32 ShopStock_GetRowCount(void);
s32 ShopStock_GetType(s32 stockRow);
void ShopStock_AddArtifacts(s32 stockRow);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);

s32 Inn_RoomPrice(s32 innId);
void Inn_PlaySleep(s32 roomPrice);

s32 Forge_ChooseMember(void);
s32 Forge_SelectItem(s32 unitId);
s32 Forge_GetItemCondition(s32 itemId);
s32 Forge_OfferItem(s32 unitId, s32 slot);

s32 Sanctum_RunPartyService(void);
void Sanctum_HighlightMembers(struct Window *window, s32 selected);
void Sanctum_DrawFee(struct Window *window, s32 unitId);
void Sanctum_ApplyService(s32 unitId, s32 serviceKind);
void Sanctum_RunMemberIconBurst(s32 member);

s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot, s32 itemMode);
s32 ItemPicker_SelectItem(s32 unitId);
void ItemPicker_DrawItem(struct Window *window, s32 unitId, s32 slot);
void ItemIconGrid_Initialize(void);
void ItemIconGrid_Release(void);
void ItemIconGrid_ShowInventory(struct Window *window, s32 x, s32 y,
                                s32 unitId);

/// The frames the hand cursor takes to glide into a newly opened list.
#define SHOP_CURSOR_ENTER_FRAMES 12

/// The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price
/// sprite's tiles (Shop_DrawPriceDigit).
extern u8 gShop_GlyphBytes[];
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind);

#endif
