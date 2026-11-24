#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

/// @file
/// The counters of a town, shops, sanctums and inns: ShopState, the state they
/// share, the shop screen's tiles, and every routine of the shop object.

#include "types.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "field/field_particle.h"
#include "field/sprite.h"
#include "game/item.h"
#include "graphics/render_input.h"
#include "system/heap_slots.h"
#include "window/cursor.h"

/// The frames the hand cursor takes to glide into a newly opened list.
#define SHOP_CURSOR_ENTER_FRAMES 12

/// The particles a sanctum service's sparkles use (ShopState.effects), and how
/// many of them fly in at the party member's icon in its burst.
#define SANCTUM_PARTICLE_COUNT 24
#define SANCTUM_BURST_SPARKLES 18

/// A shop's state: its windows, the party members' icons, the stock, the cursor
/// and its icons, and the field effect sprites.
///
/// It is the HEAP_SLOT_MENU block, the same 2,672 bytes as the field menu's
/// state: Shop_InitializeState takes and clears it as a shop, the inn,
/// the sanctum or the item picker opens, and Shop_ReleaseState frees it.
struct ShopState {
  u8 unused[8]; ///< no view of the menu state names it (see menu/README.md)
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
  FIXED partyMemberScale[8]; ///< 16.16 fixed point, FIXED_ONE full size
  /// The menu screens' choices, entries and party and the row of four
  /// figures; not named here.
  u8 menuLists[248];
  s16 stockItemIds[129];    ///< the items on sale, ended by 0
  s16 partyMemberIds[9];    ///< the party in order (Party_ListMembers)
  struct MenuCursor cursor; ///< the hand cursor
  /// VRAM cache slots of the six cursor icons, loaded when the state is set
  /// up and freed with it.
  ///@{
  u16 cursorIcon; ///< the hand
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  ///@}
  u16 unusedBeforeSelection;  ///< no view of the menu state names it
  u16 selectedItem;           ///< the item being bought, sold or repaired
  u8 unusedAfterSelection[4]; ///< no view of the menu state names it
  u16 keeperGraphics; ///< the shopkeeper's animation; picks the portrait
  s8 stockCount;
  s8 partyMemberCount;
  /// The frames the hand cursor's next move takes (MenuCursor_Place):
  /// SHOP_CURSOR_ENTER_FRAMES into a new list, then a few a step.
  u8 cursorFrames;
  /// The stock row's type (enum ShopType), which picks the keeper's lines; an
  /// inn takes a weapon shop's, whose lines are given as they are.
  u8 shopType;
  /// The counter chosen (enum ShopAction), or at a sanctum the service (enum
  /// SanctumService).
  s8 action;
  /// The party member whose icon flickers through colours in the sanctum's
  /// burst, or -1 for none.
  s8 burstMember;
  /// 1 at Xian's shops and inn, whose keepers have lines of their own.
  u8 warriorShop;
  u8 padding[3]; ///< up to the word effects starts on
  /// the sanctum service's particles
  struct FieldParticle effects[SANCTUM_PARTICLE_COUNT];
};

STATIC_ASSERT(sizeof(struct ShopState) == 2672);
STATIC_ASSERT(offsetof(struct ShopState, moneyWindow) == 12);
STATIC_ASSERT(offsetof(struct ShopState, itemWindow) == 32);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberIcons) == 276);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberScale) == 340);
STATIC_ASSERT(offsetof(struct ShopState, stockItemIds) == 620);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberIds) == 878);
STATIC_ASSERT(offsetof(struct ShopState, cursor) == 896);
STATIC_ASSERT(offsetof(struct ShopState, stockCount) == 934);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberCount) == 935);
STATIC_ASSERT(offsetof(struct ShopState, action) == 938);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberX) == 308);
STATIC_ASSERT(offsetof(struct ShopState, partyMemberY) == 324);
STATIC_ASSERT(offsetof(struct ShopState, burstMember) == 939);
STATIC_ASSERT(offsetof(struct ShopState, warriorShop) == 940);
STATIC_ASSERT(offsetof(struct ShopState, effects) == 944);

/// The shop's 16-by-16 sprites (graphics/ui/menu_sprites): the hand cursor,
/// the gem and the list arrows.
extern const u8 gMenuCursor_HandTiles[];
extern const u8 gShop_GemTiles[];
extern const u8 gShop_SmallDownArrowTiles[];
extern const u8 gShop_SmallUpArrowTiles[];
extern const u8 gShop_UpArrowTiles[];
extern const u8 gShop_DownArrowTiles[];

/// The eight blank tiles of a price, which the shop writes its digits over,
/// and of the quantity bar, which it fills in colours.
extern const u8 gShop_PriceTiles[];
extern const u8 gShop_QuantityTiles[];

struct Window;
struct FieldParticle;
struct SelectionNodeLinkView;
struct SelectionMenuCursorView;
struct WindowSprite;
struct MenuCursor;

/// A counter's state and the shop's visit, and what every counter shares: the
/// keeper's lines, the Yes or No choice and waiting out a jingle (shop_run.c).
s32 Shop_ListAvailableArtifacts(void);
void MenuCursor_Step(void);
void Shop_InitializeState(void);
void Shop_ReleaseState(void);
s32 Shop_Run(s32 stockRow, s32 keeperId);
s32 Shop_OpenDebugShop(s32 unused);
void Sound_WaitForJingle(void);
void Shop_ShowKeeperMessage(s32 messageId);
void Shop_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Shop_AskYesNo(s32 choice);
s32 Sanctum_AskYesNo(s32 choice);
struct SelectionNodeLinkView *
NodeChain_GetNodeAtIndex(struct SelectionMenuCursorView *state);

/// The quantity bar's colours (QuantityBar_Fill), painted over each other
/// longest first: past what can be chosen, what can be chosen, the number
/// chosen and what is held already. Each segment is a black pixel and a pixel
/// of its colour, in the window sprites' palette.
enum QuantityBarColor {
  QUANTITY_BAR_CHOOSABLE = 0,     ///< clear, the window showing through
  QUANTITY_BAR_HELD = 2,          ///< red
  QUANTITY_BAR_CHOSEN = 10,       ///< orange
  QUANTITY_BAR_OUT_OF_REACH = 14, ///< grey
};

/// The quantity bar and price sprites, the field's tint behind a counter and
/// the hand cursor's moves (shop_cursor.c).
void QuantityBar_Fill(s32 itemCount, s32 color, u8 *tileBuffer);
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition);
struct WindowSprite *Shop_CreatePriceSprite(s32 value, struct Window *window,
                                            s32 x, s32 y);
void Shop_SaveAndTintFieldPalette(s32 tint);
void Shop_RestoreFieldPalette(void);
void MenuCursor_Advance(struct MenuCursor *cursor);
void MenuCursor_MoveTowardTarget(struct MenuCursor *cursor);
void MenuCursor_Set(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                    s32 frames);
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y);
void MenuCursor_Place(struct Window *window, s32 x, s32 y);

/// What Shop_DrawItemPrice says below an item whose price is 0.
enum ShopPriceKind {
  SHOP_PRICE_KIND_BUY = 0,   ///< buying: the price, even when it is 0
  SHOP_PRICE_KIND_SELL = 1,  ///< selling: the shop does not buy it
  SHOP_PRICE_KIND_REPAIR = 2 ///< repairing: it is not broken
};

/// Buying, and the windows the other counters share: the coins, an item's
/// price and description, the party and the quantity bar (shop_buy.c).
s32 Shop_SelectBuy(void);
void Shop_DrawStock(struct Window *window, s32 selected);
void Shop_DrawMoney(void);
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
s32 Shop_SalePrice(s32 inventoryEntry);

/// Selling, and the party member's item grid (shop_sell.c).
s32 Shop_ChooseMember(void);
s32 Shop_SelectSell(s32 unitId);
void Shop_DrawMemberItems(struct Window *window, s32 unitId);
s32 Shop_SelectSellCount(s32 unitId, s32 slot);
void Shop_SellItem(s32 unitId, s32 slot, s32 count);

/// Repairing (shop_repair.c).
s32 Shop_RepairPrice(s32 inventoryEntry);
s32 Shop_RunRepairMemberChoice(void);
s32 Shop_SelectRepair(s32 unitId);
void Shop_RepairItem(s32 unitId, s32 slot);

/// The Game Ticket and every shop's stock (shop_stock.c).
void Shop_GiveGameTicket(struct Window *window, struct Window *itemWindow);
s32 ShopStock_GetRowCount(void);
void ShopStock_AddArtifacts(s32 stockRow);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 ShopStock_GetType(s32 stockRow);

/// The sanctum (shop_sanctum.c).
s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind);
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind);
s32 Sanctum_CountMembersInNeed(void);
s32 Sanctum_GetServiceMessage(s32 messageId);
void Sanctum_ShowKeeperMessage(s32 messageId);
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId);
s32 Sanctum_Run(s32 keeperId);
s32 Sanctum_RunPartyService(void);
void Sanctum_ApplyService(s32 unitId, s32 serviceKind);
void Sanctum_HighlightMembers(struct Window *window, s32 selected);
void Sanctum_DrawFee(struct Window *target, s32 unitId);
void Sanctum_UpdateBurstSparkle(struct FieldParticle *effect);
void Sanctum_UpdateBurst(void);
void Sanctum_RunMemberIconBurst(s32 member);

/// The inn (shop_inn.c).
s32 Inn_RoomPrice(s32 innId);
s32 Inn_CheckIn(s32 innId, s32 actorId);
void Inn_PlaySleep(s32 roomPrice);

/// The item picker (shop_item_picker.c).
s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot);
s32 ItemPicker_SelectItem(s32 unitId);
void ItemPicker_DrawItem(struct Window *window, s32 unitId, s32 slot);

#endif
