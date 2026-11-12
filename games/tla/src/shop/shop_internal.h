#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes and the types two
/// or more parts use. shop.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "task.h"
#include "edition.h"
#include "render_input.h"
#include "main_routine_work.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene.h"
#include "io_reg.h"
#include "input.h"
#include "text/msg_ids.h"
#include "version.h"
#include "shop.h"
#include "minigame.h"
#include "battle_unit.h"
#include "item.h"
#include "battle_system.h"
#include "game_flags.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "field_event.h"
#include "item_icon.h"

struct Record_080b06c0 {
  u8 filler0[4];
  u8 values[21];
};

struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
  u8 unk00e[2];
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

struct FieldObject {
  u8 unk000[0x34];
  u16 savedX;
  u16 savedY;
  s16 x;
  u16 y;
  u8 unk03c[0x14];
  struct ObjectSpriteInfo *sprite;
};

// What several parts call, declared once here for all of them.
s32 Item_GetEquipmentGroupFar(s32 item);
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);
void Inventory_EquipFar(s32 owner, s32 slot);
void Message_CloseFinishedWindowsFar(void);
s32 Message_AllTypedFar(void);
void Func_080f8060(void *window, s32 a, s32 b);
void Func_080f8058(s32 first);
s32 Inventory_CountFar(s32 unit);
void WindowSprite_PrepareForRedrawFar(void *window);
void Menu_ReleaseEntryObjectsFar(void);
void Func_0810a108(s32 unitId, s32 slot, s32 count);
void Func_0810bf98(s16 *window, s32 x, s32 y, s32 owner);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
void *Actor_GetByIdFar(s32 id);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
void WindowSprite_RedrawSavedRectFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

#endif
