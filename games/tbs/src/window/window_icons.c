/// @file
/// Item, Psynergy and command icons.
// Part of window.c.

#include "window_internal.h"

// The builders that load an icon into VRAM take HEAP_SLOT_GLYPH_TRANSFER
// (gGlyphWork) for the build and free it before returning; ItemIcon_Compose and
// the Icon_Draw functions draw into the slot their caller holds.

/// Returns how many item icons the icon table holds; Icon_BuildItemIconTiles
/// draws icon 0 for a number past them.
s32 Icon_CountItemIcons(void) { return gItemIconsEnd - gItemIcons; }

/// Returns how many Psynergy icons the icon table holds;
/// Icon_BuildAbilityIconTiles draws icon 0 for a number past them.
s32 Icon_CountPsynergyIcons(void) { return gPsynergyIconsEnd - gPsynergyIcons; }

/// Builds a command's icon, its picture over a frame (enum IconFrame), and
/// loads it into a sprite tile slot: *slot, or a free one when reuse is 0.
void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 frame, s32 *slot,
                                s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  work->encoded = gIcon_BasePointerTable[frame];
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

/// Unused: builds an ability's icon from the item icons rather than the
/// Psynergy icons, as Icon_BuildItemIconTiles.
UNUSED void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                                 s32 reuse) {
  Icon_BuildItemIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                          reuse);
}

/// Builds item icon number icon, over the item frame when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set).
void Icon_BuildItemIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
                             s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  overlay = 0;
  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));

  if (icon >= Icon_CountItemIcons())
    icon = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[ICON_FRAME_ITEM];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gItemIcons[icon];
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
/// the equipped, broken and artifact marks, and the count. Returns -1 with no
/// glyph work buffer and 256 otherwise; no caller reads the result.
s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_GetDirectFar(code & INVENTORY_ITEM_ID_MASK);
  work = gGlyphWork;
  if (work == NULL)
    return -1;
  if (layers & ICON_BASE) {
    work->encoded = gIcon_BasePointerTable[ICON_FRAME_ITEM];
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
    work->encoded = gIcon_MarkPointers[ICON_MARK_BROKEN];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
  }
  if ((layers & ICON_EQUIPPED) && (code & INVENTORY_EQUIPPED)) {
    work->encoded = gIcon_MarkPointers[ICON_MARK_EQUIPPED];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & INVENTORY_EQUIPPED) &&
      (item->flags & ITEM_CURSED) && (item->flags & ITEM_CANNOT_UNEQUIP)) {
    work->encoded = gIcon_MarkPointers[ICON_MARK_CURSED];
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
  // Counts up to a full stack show: the ones digit, and a tens digit from the
  // second set (1 to 3) over it.
  if (count != 0 && count <= INVENTORY_STACK_MAX) {
    s32 ones = count % 10;

    work->encoded = gIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 1);
    if (count / 10 != 0) {
      work->encoded =
          gIcon_DigitPointers[ICON_DIGIT_TENS_FIRST - 1 + count / 10];
      work->width = 2;
      work->height = 2;
      Icon_Decode(work, 1);
    }
  }
  return 256;
}

/// Draws an item's icon from its inventory code with the layers asked for
/// (ItemIcon_Compose) and loads it into sprite tile slot cacheSlot.
s32 Icon_CopyResourceToSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  ItemIcon_Compose(inventoryCode, layers);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return 1;
}

/// Builds one of the other icons (gIcon_MiscIconPointers) and loads it into
/// sprite tile slot cacheSlot.
s32 Icon_BuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawMisc(icon);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return 1;
}

/// Loads 32 bytes, one sprite tile, from the start of gMenuTitlePictures into
/// the VRAM cache entry slot, whichever entry is asked for: those bytes are the
/// table's first pointers, not a picture.
s32 Resource_LoadTableEntry(u32 entry, s32 unused, u32 slot) {
  extern const u8 *gMenuTitlePicturesCaseA[] __asm__("gMenuTitlePictures"),
      *gMenuTitlePicturesCaseB[] __asm__("gMenuTitlePictures"),
      *gMenuTitlePicturesCaseC[] __asm__("gMenuTitlePictures");
  void *source;
  switch (entry) {
  case 1:
    source = gMenuTitlePicturesCaseA;
    break;
  case 2:
    source = gMenuTitlePicturesCaseB;
    break;
  case 3:
    source = gMenuTitlePicturesCaseC;
    break;
  case 0:
  default:
    source = gMenuTitlePictures;
    break;
  }
  VramCache_Load(slot, 32, source);
  return 1;
}

/// Draws an item's icon from its inventory code into the glyph work buffer,
/// with no frame or marks; code 0 draws icon 0. The caller holds the buffer.
void Icon_DrawItem(s32 code) {
  GlyphTransfer *glyph = gGlyphWork;
  struct ItemDefinition *item =
      Item_GetDirectFar(INVENTORY_ITEM_ID_MASK & code);

  if (code != 0) {
    glyph->encoded = gItemIcons[item->icon];
  } else {
    glyph->encoded = gItemIcons[0];
  }
  glyph->width = 2;
  glyph->height = 2;
  Icon_Decode(glyph, 0);
}

/// Builds an ability's Psynergy icon, as Icon_BuildAbilityIconTiles.
void Ability_LoadGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                       s32 reuse) {
  Icon_BuildAbilityIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                             reuse);
}

/// Builds Psynergy icon number icon, over the item frame when withBase is set,
/// and loads it into *cacheSlot (a free slot unless reuse is set).
void Icon_BuildAbilityIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
                                s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  overlay = 0;

  if (icon >= Icon_CountPsynergyIcons())
    icon = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[ICON_FRAME_ITEM];
    work->width = 2;
    work->height = 2;
    Icon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gPsynergyIcons[icon];
  work->width = 2;
  work->height = 2;
  Icon_Decode(work, overlay);

  if (reuse == 0)
    *cacheSlot = VramCache_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Draws one of the other icons into the glyph work buffer.
void Icon_DrawMisc(u32 iconIndex) {
  GlyphTransfer *glyph = gGlyphWork;

  glyph->encoded = gIcon_MiscIconPointers[iconIndex];
  glyph->width = 2;
  glyph->height = 2;
  Icon_Decode(glyph, 0);
}

/// Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a
/// free slot unless reuse is set) and its colours into the sprite palette named
/// by palette.
void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse) {
  GlyphTransfer *work;
  u16 *offsets;
  u8 *encodedEntry;
  u32 index;

  work = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  offsets = Resource_GetTableEntry(RESOURCE_ICONS);
  // A number below PORTRAIT_SPRITE_MAP_FIRST is the entry itself; the other
  // speakers' portraits, numbered from it (Portrait_GetForSpeaker), are the
  // entries from PORTRAIT_JUPITER_DJINNI on. Each entry's colours come before
  // its encoded tiles.
  if (icon < PORTRAIT_SPRITE_MAP_FIRST)
    index = icon;
  else
    index = icon - PORTRAIT_SPRITE_MAP_FIRST + PORTRAIT_JUPITER_DJINNI;
  encodedEntry = (u8 *)offsets + offsets[index];
  work->encoded = encodedEntry + PALETTE_COLORS * sizeof(u16);
  work->width = 4;
  work->height = 4;
  Icon_Decode(work, 0);
  if (reuse == 0)
    *slot = VramCache_FindFreeEntry();
  *tile = VramCache_Load(*slot, 4 * 4 * TILE_SIZE_4BPP, work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  Dma_Set(encodedEntry, (void *)&OBJ_PLTT_COLOR(palette, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/// Unused: an empty routine among the icon builders.
UNUSED void Icon_ReservedNoOp(void) {}

/// Decodes a compressed icon into the glyph buffer and expands it to tiles:
/// opaque for the first layer, or drawn over what is there when overlay is set.
void Icon_Decode(GlyphTransfer *glyph, s32 overlay) {
  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_Decompress4bpp,
                   Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))gHeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
      glyph->encoded, glyph->input);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
  if (overlay)
    RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_ExpandMasked,
                     Tile_ExpandMaskedCodeSize);
  else
    RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_ExpandOpaque,
                     Tile_ExpandOpaqueCodeSize);
  ((void (*)(u8 *, u8 *, u32, u32))gHeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
      glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
}
