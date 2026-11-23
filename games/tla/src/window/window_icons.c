/// @file
/// Item, Psynergy and command icons.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

/// The encoded icons: the command icons' bases and the pictures drawn over
/// them, the marks an item icon may carry (equipped, broken), and the digits of
/// its count.
extern u8 *gIcon_BasePointerTable[];
extern u8 *gIcon_OverlayPointerTable[];
extern u8 *gIcon_MarkPointers[];
extern u8 *gIcon_DigitPointers[];

void Icon_Decode(GlyphTransfer *glyph, s32 outlined);
void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                             s32 *tileOut, s32 reuse);

/// Returns how many item icons the icon table holds; Icon_BuildItemIconTiles
/// draws icon 0 for a number past them.
s32 Icon_CountItemIcons(void) { return gItemIconsEnd - gItemIcons; }

/// Returns how many Psynergy icons the icon table holds;
/// Icon_BuildAbilityIconTiles draws icon 0 for a number past them.
s32 Icon_CountPsynergyIcons(void) { return gPsynergyIconsEnd - gPsynergyIcons; }

/// Builds a command icon (overlay overlayIcon over base baseIcon) and loads it
/// into a sprite tile slot: *slot, or a free one when reuse is 0.
void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  work->encoded = gIcon_BasePointerTable[baseIcon];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, 0);
  work->encoded = gIcon_OverlayPointerTable[overlayIcon];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, 1);
  if (reuse == 0) {
    *slot = VramCache_FindFreeEntry();
  }
  *tileOut = VramCache_Load(*slot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Builds the icon of a battle action's item.
void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                          s32 reuse) {
  Icon_BuildItemIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                          reuse);
}

/// Builds item icon glyph, over the icon base when withBase is set, and loads
/// it into *cacheSlot (a free slot unless reuse is set). Returns the tile in
/// *tileOut.
void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                             s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  overlay = 0;
  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));

  if (glyph >= Icon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gItemIcons[glyph];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, overlay);

  if (reuse == 0)
    *cacheSlot = VramCache_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Draws an item's icon into the glyph work buffer from its inventory code
/// (enum InventoryEncoding) with the layers asked for (ICON_ flags): the frame,
/// the equipped, broken and artifact marks, and the count. Returns 256, which
/// no caller reads, or -1 without the glyph work buffer.
s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_GetDirectFar(code & INVENTORY_ITEM_ID_MASK);
  work = Ram_HeapSlots->glyphTransfer;
  if (work == NULL)
    return -1;
  if (layers & ICON_BASE) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 0);
    overlay = 1;
  }
  work->encoded = gItemIcons[item->icon];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, overlay);
  if ((layers & ICON_BROKEN) && (code & INVENTORY_BROKEN)) {
    work->encoded = gIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
  }
  if ((layers & ICON_EQUIPPED) && (code & INVENTORY_EQUIPPED)) {
    work->encoded = gIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & INVENTORY_EQUIPPED) &&
      (item->flags & ITEM_CURSED) && (item->flags & ITEM_CANNOT_UNEQUIP)) {
    work->encoded = gIcon_MarkPointers[2];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
  }
  if (layers & ICON_COUNT_ABOVE_ONE) {
    count = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
    count++;
    if (count <= 1)
      count = 0;
  }
  if (layers & ICON_COUNT) {
    count = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
    count++;
  }
  // Counts up to a full stack show: the ones digit from digits 0 to 9, the
  // tens from a second set at 10 to 12.
  if (count != 0 && count <= INVENTORY_STACK_MAX) {
    s32 ones = count % 10;

    work->encoded = gIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
    if (count / 10 != 0) {
      work->encoded = gIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      Icon_Decode(work, 1);
    }
  }
  return 256;
}

/// Composes an inventory icon with the requested layers and loads it into
/// cacheSlot. The temporary glyph block is released after the upload.
s32 Icon_CopyResourceToSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  ItemIcon_Compose(inventoryCode, layers);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return 1;
}

/// Draws one of the other icons (ICON_KIND_PATTERN) and loads it into
/// cacheSlot, as Icon_CopyResourceToSlot does an item's.
s32 Icon_BuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawMisc(icon);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return 1;
}

/// Decodes item code's icon (the first icon for code 0) into the glyph work.
void Icon_DrawItem(s32 code) {
  GlyphTransfer *state = Ram_HeapSlots->glyphTransfer;
  struct ItemDefinition *item =
      Item_GetDirectFar(INVENTORY_ITEM_ID_MASK & code);

  if (code != 0)
    state->encoded = gItemIcons[item->icon];
  else
    state->encoded = gItemIcons[0];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, 0);
}

/// Builds the Psynergy icon of a battle action, as Icon_BuildAbilityIconTiles.
void Ability_LoadGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                       s32 reuse) {
  Icon_BuildAbilityIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                             reuse);
}

/// Builds Psynergy icon glyph, over the icon base when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
/// in *tileOut.
void Icon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  overlay = 0;

  if (glyph >= Icon_CountPsynergyIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gPsynergyIcons[glyph];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, overlay);

  if (reuse == 0)
    *cacheSlot = VramCache_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Draws one of the other icons into the glyph work buffer.
void Icon_DrawMisc(u32 index) {
  GlyphTransfer *state = Ram_HeapSlots->glyphTransfer;

  state->encoded = gIcon_MiscIconPointers[index];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, 0);
}

/// The icon resource's row of the first sprite portrait,
/// PORTRAIT_FIRST_SPRITE_PORTRAIT.
#define PORTRAIT_FIRST_SPRITE_ICON 16

/// Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a
/// free slot unless reuse is set) and its colours into the sprite palette
/// named by palette. Returns the tile in *tile.
/// Each icon of the resource has its PALETTE_COLORS colours before its encoded
/// tiles. Portraits (Portrait_Create) come through here.
void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse) {
  GlyphTransfer *work;
  u16 *table;
  u8 *entry;
  u32 index;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  table = Resource_GetTableEntry(RESOURCE_ICONS);
  if (icon < PORTRAIT_FIRST_SPRITE_PORTRAIT)
    index = icon;
  else
    index = icon - PORTRAIT_FIRST_SPRITE_PORTRAIT + PORTRAIT_FIRST_SPRITE_ICON;
  entry = (u8 *)table + table[index];
  work->encoded = entry + PALETTE_COLORS * sizeof(u16);
  work->width = 4;
  work->height = 4;
  Icon_Decode(work, 0);
  if (reuse == 0)
    *slot = VramCache_FindFreeEntry();
  *tile = VramCache_Load(*slot, 4 * 4 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/// An empty function. Nothing in the tree names it but its prototype.
void Icon_ReservedNoOp(void) {}

/// The tile routines RamCode_Load copies to RAM to run, and their sizes, as
/// these symbols' addresses.
extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

/// Decodes a compressed icon into the glyph buffer and expands it to tiles,
/// opaque for the first layer or masked over what is there when outlined is
/// set. Releases each copied decoder before loading the next routine into the
/// same slot. glyph->width and height are the icon's size in tiles.
void Icon_Decode(GlyphTransfer *glyph, s32 outlined) {
  void *code;

  RamCode_Load(code, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
  Heap_Release(code);
  if (outlined)
    RamCode_Load(code, Tile_ExpandMaskedCode, Tile_ExpandMaskedCodeSize);
  else
    RamCode_Load(code, Tile_ExpandOpaqueCode, Tile_ExpandOpaqueCodeSize);
  ((void (*)(u8 *, u8 *, u32, u32))code)(glyph->input, glyph->tiles,
                                         (u16)glyph->width, (u16)glyph->height);
  Heap_Release(code);
}
