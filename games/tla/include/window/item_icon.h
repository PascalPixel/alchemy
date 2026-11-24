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

void ItemIcon_LoadLayersIntoSlotFar(s32 item, s32 mode, s32 resource);

void ItemIconGrid_Draw(void);
void ItemIconGrid_SetPosition(s32 index, s32 x, s32 y);
void ItemIconGrid_SetVisible(s32 active);
void ItemIconGrid_LoadItems(u16 *items, s32 count, s32 mode);

/// The compressed item icons, indexed by an item's icon field.
extern u8 *gItemIcons[];
/// The end of gItemIcons, so Icon_CountItemIcons can count them.
extern u8 *gItemIconsEnd[];
/// The other compressed icons, among them the Status screen's status icons.
extern u8 *gIcon_MiscIconPointers[];
/// The compressed Psynergy icons, drawn over the shared icon base.
extern u8 *gPsynergyIcons[];
/// The end of gPsynergyIcons, for Icon_CountPsynergyIcons.
extern u8 *gPsynergyIconsEnd[];
/// Draws an item's icon into the glyph buffer from its inventory code with the
/// layers asked for (ICON_ flags).
s32 ItemIcon_Compose(u32 code, u32 layers);
/// Draws an item's icon as ItemIcon_Compose does and loads it into VRAM cache
/// entry cacheSlot.
s32 ItemIcon_LoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot);
/// Draws one of the other icons into the glyph buffer.
void Icon_DrawMisc(u32 index);
/// Builds a command icon, overlayIcon over baseIcon, and loads it into a sprite
/// tile slot: *slot, or a free one when reuse is 0.
void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tile, s32 reuse);
/// Builds a Psynergy icon, over the icon base when withBase is set, and loads
/// it into *slot, or a free slot unless reuse is set.
void Icon_BuildAbilityIconTiles(u32 icon, s32 withBase, s32 *slot, s32 *tile,
                                s32 reuse);
/// Builds the Psynergy icon of a battle action, as Icon_BuildAbilityIconTiles.
void Ability_BuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
                            s32 reuse);

#endif
