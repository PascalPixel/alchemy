#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

/// @file
/// ShopWork, a shop's work, and the shop screen's tiles.

#include "shop/shop.h"
#include "heap_slots.h"
#include "item.h"
#include "static_assert.h"
#include "render_input.h"
#include "field_particle.h"
#include "sprite.h"
#include "cursor.h"

/// A shop's work: its windows, the party members' icons, the stock, the cursor
/// and its icons, and the field effect sprites.
///
/// It is the HEAP_SLOT_MENU block, the same 2,672 bytes as the field menu's
/// work: Shop_InitializeWork takes and clears it as a shop, the inn,
/// the sanctum or the item picker opens, and Shop_ReleaseWork frees it.
struct ShopWork {
  u8 unused[8]; ///< no view of the menu work names it (see menu/README.md)
  s32 selectedMember; ///< the menu screens' chosen party member; not named here
  struct Window *moneyWindow; ///< the coins window
  /// The menu screens' first window and their two sides; not named here.
  u8 menuSides[16];
  struct Window *itemWindow;
  /// The menu screens' windows, icons and cursor grid; not named here.
  u8 menuWindows[240];
  struct Sprite *partyMemberIcons[8]; ///< the party row's sprites
  s16 partyMemberX[8];                ///< pixels
  s16 partyMemberY[8];                ///< pixels
  s32 partyMemberScale[8];            ///< 16.16 fixed point, Q16_ONE full size
  /// The menu screens' choices, entries and party and the row of four
  /// figures; not named here.
  u8 menuLists[248];
  s16 stockItemIds[129];    ///< the items on sale, ended by 0
  s16 partyMemberIds[9];    ///< the party in order (Party_ListMembers)
  struct MenuCursor cursor; ///< the hand cursor
  /// VRAM cache slots of the six cursor icons, loaded when the work is set
  /// up and freed with it.
  ///@{
  u16 cursorIcon; ///< the hand
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  ///@}
  u16 unusedBeforeSelection;  ///< no view of the menu work names it
  u16 selectedItem;           ///< the item being bought, sold or repaired
  u8 unusedAfterSelection[4]; ///< no view of the menu work names it
  u16 keeperGraphics; ///< the shopkeeper's animation; picks the portrait
  s8 stockCount;
  s8 partyMemberCount;
  u8 mode;        ///< passed on as the hand cursor's kind; set to 12 at first
  u8 shopType;    ///< the stock row's type (enum ShopType)
  s8 action;      ///< the counter chosen (enum ShopAction)
  s8 burstMember; ///< the sanctum's party member whose pose is randomized; -1
                  ///< none
  u8 warriorShop; ///< 1 for stock rows 16 to 18, which greet as warriors'
  u8 padding[3];  ///< up to the word effects starts on
  struct FieldParticle effects[24]; ///< the sanctum service's particles
};

STATIC_ASSERT(sizeof(struct ShopWork) == 0xa70);
STATIC_ASSERT(offsetof(struct ShopWork, moneyWindow) == 0x00c);
STATIC_ASSERT(offsetof(struct ShopWork, itemWindow) == 0x020);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIcons) == 0x114);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberScale) == 0x154);
STATIC_ASSERT(offsetof(struct ShopWork, stockItemIds) == 0x26c);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIds) == 0x36e);
STATIC_ASSERT(offsetof(struct ShopWork, cursor) == 0x380);
STATIC_ASSERT(offsetof(struct ShopWork, stockCount) == 0x3a6);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberCount) == 0x3a7);
STATIC_ASSERT(offsetof(struct ShopWork, action) == 0x3aa);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberX) == 0x134);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberY) == 0x144);
STATIC_ASSERT(offsetof(struct ShopWork, burstMember) == 0x3ab);
STATIC_ASSERT(offsetof(struct ShopWork, warriorShop) == 0x3ac);
STATIC_ASSERT(offsetof(struct ShopWork, effects) == 0x3b0);

extern const u8 gMenuCursor_HandTiles[];
extern const u8 gShop_GemTiles[];
extern const u8 gShop_SmallDownArrowTiles[];
extern const u8 gShop_SmallUpArrowTiles[];
extern const u8 gShop_UpArrowTiles[];
extern const u8 gShop_DownArrowTiles[];

extern const u8 gShop_PriceTiles[];
extern const u8 gShop_QuantityTiles[];


#include "types.h"
struct Window;

struct FieldParticle;
struct SelectionNodeLinkView;
struct SelectionMenuCursorView;
struct WindowSprite;
struct MenuCursor;

s32 Shop_ListAvailableArtifacts(void);
void MenuCursor_Step(void);
void Shop_InitializeWork(void);
void Shop_ReleaseWork(void);
s32 Shop_Run(s32 stockRow, s32 keeperId);
s32 Shop_OpenDebugShop(s32 unused);
void Sound_WaitForJingle(void);
void Shop_ShowKeeperMessage(s32 messageId);
void Shop_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Shop_AskYesNo(s32 choice);
s32 Sanctum_AskYesNo(s32 choice);
struct SelectionNodeLinkView *
NodeChain_GetNodeAtIndex(struct SelectionMenuCursorView *state);
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
void Shop_GiveGameTicket(struct Window *window, struct Window *itemWindow);

s32 ShopStock_GetRowCount(void);
void ShopStock_AddArtifacts(s32 stockRow);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 ShopStock_GetType(s32 stockRow);

s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind);
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind);
s32 Sanctum_CountMembersInNeed(void);
s32 Sanctum_GetServiceMessage(s32 messageId);
void Sanctum_ShowKeeperMessage(s32 messageId);
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Sanctum_Run(s32 unitId);
s32 Sanctum_RunPartyService(void);
void Sanctum_ApplyService(s32 unitId, s32 serviceKind);
void Sanctum_HighlightMembers(struct Window *window, s32 selected);
void Sanctum_DrawFee(struct Window *target, s32 unitId);
void Sanctum_UpdateBurstSparkle(struct FieldParticle *effect);
void Sanctum_UpdateBurst(void);
void Sanctum_RunMemberIconBurst(s32 member);

s32 Inn_RoomPrice(s32 innId);
s32 Inn_CheckIn(s32 innId, s32 actorId);
void Inn_PlaySleep(s32 roomPrice);

s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot);
s32 ItemPicker_SelectItem(s32 unitId);
void ItemPicker_DrawItem(struct Window *window, s32 unitId, s32 slot);


#endif
