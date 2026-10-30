#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

/// @file
/// ShopWork, a shop's work, and the shop screen's tiles.

#include "module/shop/shop.h"
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

#endif
