#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "unit/shop/shop.h"
#include "global_cells.h"
#include "item.h"
#include "static_assert.h"
#include "render_input.h"
#include "effect_slot.h"
#include "animspr.h"
#include "cursor.h"

struct ShopRuntime {
  u8 unk000[0x0c];
  struct UiWindow *moneyWindow;
  u8 unk010[0x10];
  struct UiWindow *itemWindow;
  u8 unk024[0xf0];
  struct AnimationObject *partyMemberIcons[8];
  s16 partyMemberX[8];
  s16 partyMemberY[8];
  s32 partyMemberScale[8];
  u8 unk174[0xf8];
  s16 stockItemIds[129];
  s16 partyMemberIds[9];
  struct ShopCursor cursor;
  u16 cursorIcon;
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  u16 unk39c;
  u16 selectedItem;
  u8 unk3a0[4];
  u16 keeperResource;
  s8 stockCount;
  s8 partyMemberCount;
  u8 mode;
  u8 shopType;
  s8 partyAction;
  s8 burstMember;
  u8 warriorShop;
  u8 unk3ad[3];
  struct EffectSlot effects[24];
};

STATIC_ASSERT(sizeof(struct ShopRuntime) == 0xa70);
STATIC_ASSERT(offsetof(struct ShopRuntime, moneyWindow) == 0x00c);
STATIC_ASSERT(offsetof(struct ShopRuntime, itemWindow) == 0x020);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberIcons) == 0x114);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberScale) == 0x154);
STATIC_ASSERT(offsetof(struct ShopRuntime, stockItemIds) == 0x26c);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberIds) == 0x36e);
STATIC_ASSERT(offsetof(struct ShopRuntime, cursor) == 0x380);
STATIC_ASSERT(offsetof(struct ShopRuntime, stockCount) == 0x3a6);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberCount) == 0x3a7);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyAction) == 0x3aa);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberX) == 0x134);
STATIC_ASSERT(offsetof(struct ShopRuntime, partyMemberY) == 0x144);
STATIC_ASSERT(offsetof(struct ShopRuntime, burstMember) == 0x3ab);
STATIC_ASSERT(offsetof(struct ShopRuntime, warriorShop) == 0x3ac);
STATIC_ASSERT(offsetof(struct ShopRuntime, effects) == 0x3b0);

extern const u8 gShop_HandTiles[];
extern const u8 gShop_GemTiles[];
extern const u8 gShop_SmallDownArrowTiles[];
extern const u8 gShop_SmallUpArrowTiles[];
extern const u8 gShop_UpArrowTiles[];
extern const u8 gShop_DownArrowTiles[];

extern const u8 gShop_PriceTiles[];
extern const u8 gShop_QuantityTiles[];

#endif
