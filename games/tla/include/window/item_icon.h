#ifndef ALCHEMY_ITEM_ICON_H
#define ALCHEMY_ITEM_ICON_H

/// @file
/// Item and ability icons as window sprites, and the shops' grid of item
/// icons.

#include "types.h"
#include "system/ram_buffer.h"
#include "system/task.h"
#include "graphics/vram_tab.h"

/// An icon slot that holds no VRAM cache entry.
#define ICON_NONE VRAM_CACHE_ENTRY_COUNT

/// An icon sprite's OAM attributes, as OAM lays them out.
struct IconAttr {
  u8 y;
  u8 attr0High;
  u16 x : 9;
  u16 attr1High : 7;
  u16 tile : 10;
  u16 attr2High : 6;
  u16 affineParam; ///< OAM's fourth halfword, the affine parameter slot
};

/// An icon's sprite: its link in the sprite list and its OAM attributes.
struct IconSprite {
  u32 link;
  struct IconAttr attr;
};

s32 VramCache_FreeEntry(u32 entry);
s32 VramCache_FindFreeEntry(void);
void Icon_CopyResourceToSlotFar(s32 item, s32 mode, s32 resource);
struct Unit *Unit_Get(u32 unitId);

void ItemIconGrid_Draw(void);
void ItemIconGrid_SetPosition(s32 index, s32 x, s32 y);
void ItemIconGrid_SetVisible(s32 active);
void ItemIconGrid_LoadItems(u16 *items, s32 count, s32 mode);

#endif
