#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes and the types two
/// or more parts use. shop.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "stddef.h"
#include "static_assert.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "edition.h"
#include "graphics/render_input.h"
#include "system/main_routine_work.h"
#include "battle/battle.h"
#include "module/window/window.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/scene.h"
#include "system/io_reg.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "version.h"
#include "menu/shop.h"
#include "minigame.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "battle/battle_system.h"
#include "game/game_flags.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "field_event.h"
#include "item_icon.h"
#include "game/character_ids.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/window.h"
#include "module/shop/shop.h"
#include "module/tla.h"

struct FieldObject {
  u8 unk000[0x34];
  u16 savedX;
  u16 savedY;
  s16 x;
  u16 y;
  u8 unk03c[0x14];
  struct ObjectSpriteInfo *sprite;
};

void WindowSprite_RedrawSavedRectFar(void *window);

struct ShopCursorSprite {
  u8 unk00[5];
  u8 active;
};

struct UiSprite {
  u8 unk00[24];
  u16 tile : 10;
  u16 flags : 6;
};

struct SpriteAttr2 {
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct ShopCursorSprite3 {
  struct WindowSprite *next;
  u8 unk04[2];
  u16 x;
  u16 y;
  u8 unk0a[0x0a];
  struct SpriteAttr2 oam;
};

struct ForgeRecipe {
  s16 item;
  s16 chance;
  s16 results[8];
  s16 weights[8];
};

/// The inn's view of the shop work (heap slot 220).
struct InnRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[0x2a];
  s8 partyMemberCount;
  u8 unk505;
  u8 busy;
};

struct InnUnit {
  u8 unk00[0x38];
  s16 hp;
};

// The inn work block, as this function reads it.
struct InnRuntimeState {
  u8 unk000[0x0c];
  s32 window;
  u8 unk010[0x4ea];
  u16 resourceId;
  u8 unk4fc[0x0a];
  u8 active;
};

struct InnObjectComponent {
  u8 padding00[0x28];
  u16 *resourceId;
};

struct InnObject {
  u8 padding00[0x50];
  struct InnObjectComponent *component;
};

enum InnMessageId {
  INN_MESSAGE_WELCOME,
  INN_MESSAGE_STAY_COMPLETE,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_GOODBYE,
  INN_MESSAGE_REST_COMPLETE,
};

struct Effect_080b2f4c {
  u8 unk000[0xc];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  s32 filler1C;
  s32 velocity;
  s32 acceleration;
  u8 unk028[0x18];
  s8 state;
  u8 filler41;
  s8 flag;
  u8 unk043[5];
};

struct BurstSprite {
  void *object;
  s32 x, z, targetX, targetZ, originX, originZ, speed, speedMax, acceleration;
  s32 scaleX;
  s32 scaleY;
  u16 heading;
  s16 turnStepMax;
  void *callback;
  u16 age;
  s16 callbackDelay;
  s16 orbitRadius;
  s16 orbitAngle;
  s8 state;
  s8 stopAtTarget;
  s8 turnStepLimited;
  s8 updateMotion;
  s8 render;
  s8 active;
};

/// What the player chose at a shop's counter, in the order of its menu's
/// Buy, Sell, Artifacts and Repair, held in shop->action while it runs.
enum ShopAction {
  SHOP_ACTION_BUY,
  SHOP_ACTION_SELL,
  SHOP_ACTION_ARTIFACTS,
  SHOP_ACTION_REPAIR
};

/// A shop's type, the last column of its stock row: the equipment group it
/// sells, as Item_GetEquipmentGroup sorts items, or a general store, which
/// The Lost Age adds and which lists artifacts of every group. It picks the
/// artifacts the shop lists and which of the keepers' lines it says.
enum ShopType {
  SHOP_TYPE_ITEMS = 0, ///< an item shop
  SHOP_TYPE_WEAPONS = 1,
  SHOP_TYPE_ARMOR = 2,
  SHOP_TYPE_GENERAL_STORE = 3
};

/// Byte view of the six colour writes in a quantity-bar segment, laid over a
/// 4-bit tile at the segment's byte column (QuantityBar_Fill, cursor.c).
struct TileQuantityBarSegmentView {
  u8 filler0[4]; ///< the 4 bytes before the segment's byte in row 1, untouched
  /// Rows 1 to 6: index 4n is the segment's byte in row n + 1.
  u8 values[21];
};

/// The shop's work (heap slot 220, the menus' work), as The Broken Seal's
/// struct ShopWork, its lists longer here: the windows, the party row, the
/// stock, the hand cursor and its icons, what the player chose, and the
/// sanctum service's particles.
struct ShopWork {
  u8 unused[8];       ///< no view of the menu work names it
  s32 selectedMember; ///< the menu screens' chosen party member; not named here
  void *moneyWindow;  ///< the coins window
  /// The menu screens' first window and their two sides; not named here.
  u8 menuSides[20];
  void *itemWindow;
  /// The menu screens' windows, icons and cursor grid; not named here.
  u8 menuWindows[208];
  void *partyMemberIcons[8]; ///< the party row's sprites
  s16 partyMemberX[8];       ///< pixels
  s16 partyMemberY[8];       ///< pixels
  s32 partyMemberScale[8];   ///< 16.16 fixed point, Q16_ONE full size
  /// The menu screens' choices, entries and party and the row of figures; not
  /// named here.
  u8 menuLists[280];
  s16 stockItemIds[301];    ///< the items on sale, ended by 0
  s16 partyMemberIds[9];    ///< the party in order
  struct ShopCursor cursor; ///< the hand cursor
  /// VRAM cache slots of the six cursor icons, loaded when the work is set up
  /// and freed with it.
  ///@{
  u16 cursorIcon; ///< the hand
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  ///@}
  u8 unusedBeforeKeeper[2]; ///< no view of the menu work names it
  u16 keeperGraphics;       ///< the shopkeeper's animation; picks the portrait
  u16 selectedItem;         ///< the item being bought, sold or repaired
  u8 unusedAfterSelection[4]; ///< no view of the menu work names it
  s16 stockCount;
  s8 partyMemberCount;
  u8 mode;        ///< passed on as the hand cursor's kind
  s8 shopType;    ///< the stock row's type (enum ShopType)
  s8 action;      ///< the counter chosen (enum ShopAction)
  s8 burstMember; ///< the sanctum's party member whose pose is randomized; -1
                  ///< none
  u8 detailsHidden;
  u8 unk50a;
  s8 itemMode;
  struct BurstSprite effects[24]; ///< the sanctum service's particles
  u8 unkbcc[0x11c];
  void *listWindow;
};

STATIC_ASSERT(offsetof(struct ShopWork, itemWindow) == 0x024);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIcons) == 0x0f8);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberScale) == 0x138);
STATIC_ASSERT(offsetof(struct ShopWork, stockItemIds) == 0x270);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIds) == 0x4ca);
STATIC_ASSERT(offsetof(struct ShopWork, cursor) == 0x4dc);
STATIC_ASSERT(offsetof(struct ShopWork, keeperGraphics) == 0x4fa);
STATIC_ASSERT(offsetof(struct ShopWork, stockCount) == 0x502);
STATIC_ASSERT(offsetof(struct ShopWork, action) == 0x507);
STATIC_ASSERT(offsetof(struct ShopWork, effects) == 0x50c);
STATIC_ASSERT(offsetof(struct ShopWork, listWindow) == 0xce8);
STATIC_ASSERT(sizeof(struct ShopWork) == 0xcec);

struct ShopCursorAnchor {
  u8 unk00[5];
  u8 active;
  u8 unk006[2];
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

struct ObjectSpriteInfo {
  u8 unk00[0x28];
  u16 *portrait;
};

extern const u8 gShop_QuantityTiles[];

extern const u8 gShop_PriceTiles[];

extern struct ForgeRecipe gData_0810cc34[];

/// The price per guest of each inn.
extern const s8 gData_0810cf2c[];

extern s8 sSanctumServiceCues[];

extern const s16 sGameTicketCreditCosts[];

extern const s16 sStockTable[][33];

#endif
