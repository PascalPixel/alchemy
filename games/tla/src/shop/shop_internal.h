#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes and the types two
/// or more parts use. shop.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "edition.h"
#include "graphics/render_input.h"
#include "system/main_routine_work.h"
#include "battle/battle.h"
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

struct ShopWork_08109188 {
  u8 unk00[12];
  void *coinWindow;
};

struct ShopWork {
  u8 unk000[0x24];
  void *itemWindow;
  u8 unk028[0x4cc];
  u16 statUpIcon;
  u16 statDownIcon;
  u8 unk4f8[0xd];
  u8 mode;
  u8 unk506;
  s8 partyAction;
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

struct ShopFxWork {
  u8 unk000[248];
  void *children[0x104];
  s8 selected;
  u8 unk509[3];
  u8 slots[24][72];
};

struct BurstSprite {
  s32 object;
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

struct BurstShop {
  u8 unk000[0xf8];
  s32 partyMemberIcons[8];
  s16 partyMemberX[8];
  s16 partyMemberY[8];
  u8 unk138[0x4dc - 0x138];
  struct ShopCursor cursor;
  u8 unk4ec[0x507 - 0x4ec];
  s8 action;
  s8 burstMember;
  u8 unk509[3];
  struct BurstSprite effects[24];
};

struct ShopSellWork {
  u8 unk000[0x4dc];
  struct ShopCursor cursor;
  u8 unk4ec[0x19];
  u8 mode;
};

/// What the player chose at a shop's counter, in the order of its menu's
/// Buy, Sell, Artifacts and Repair, held in shop->partyAction while it runs.
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

/// The shop's work (heap slot 220), as far as these functions use it.
struct ShopRuntime {
  u8 unk000[0xc];
  void *moneyWindow;
  u8 unk010[0x14];
  void *itemWindow;
  u8 unk028[0xd0];
  void *partyMemberIcons[4];
  u8 unk108[0x30];
  s32 partyMemberScale[4];
  u8 unk148[0x128];
  s16 stockItemIds[0x12d];
  s16 partyMemberIds[8];
  u8 unk4da[2];
  struct ShopCursor cursor;
  u16 cursorIcon;
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  u8 unk4f8[2];
  u16 keeperResource;
  u16 selectedItem;
  u8 unk4fe[4];
  s16 stockCount;
  s8 partyMemberCount;
  u8 mode;
  s8 shopType;
  s8 partyAction;
  u8 unk508;
  u8 detailsHidden;
  u8 unk50a;
  s8 itemMode;
  u8 unk50c[0x7dc];
  void *listWindow;
};

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
