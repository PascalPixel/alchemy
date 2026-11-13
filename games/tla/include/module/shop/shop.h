#ifndef ALCHEMY_MODULE_SHOP_SHOP_H
#define ALCHEMY_MODULE_SHOP_SHOP_H

/// @file
/// What games/tla/src/shop/ declares for the files that use it, written once.

#include "types.h"
#include "module/tla.h"

struct ShopCursor;

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

struct FieldEffectState {
  u8 padding0[0x1AC];
  s32 effect;
  u8 padding1B0[4];
  s32 delay;
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
  /// ★ Named from the game: "Weapons Shop" (message 4973)
  SHOP_TYPE_WEAPONS = 1,
  SHOP_TYPE_ARMOR = 2,
  /// ★ Named from the game: "General Storekeeper" (message 4982)
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

extern const u8 gShop_QuantityTiles[];

s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);

void Shop_DrawMemberItems(void *window, s32 item);

extern const u8 gShop_PriceTiles[];

void MenuCursor_Set(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind);

extern struct ForgeRecipe gData_0810cc34[];
/// The price per guest of each inn.
extern const s8 gData_0810cf2c[];

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

extern s8 Data_0810cf28[];

s32 Shop_SelectSell(s32 unitId);

s32 Shop_SelectRepair(s32 unitId);

s32 Forge_SelectItem(s32 unitId);

s32 Shop_RepairPrice(s32 item);

s32 Forge_GetMaterialChance(s32 item);

s32 Shop_SelectSellCount(s32 unitId, s32 slot);

void Shop_RepairItem(s32 unitId, s32 slot);

s32 Forge_OfferItem(s32 unitId, s32 slot);

extern const s16 Data_0810c38e[];

extern const s16 gData_0810c3f4[][33];

void Shop_SellItem(s32 unitId, s32 slot, s32 count);

void ItemIconGrid_ShowInventory(s16 *window, s32 x, s32 y, s32 owner);

#endif
