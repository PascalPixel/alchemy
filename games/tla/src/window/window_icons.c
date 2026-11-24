/// @file
/// The 16x16 icons of items, Psynergy and commands, built from their pictures,
/// a frame under them and the marks over them, and the 32x32 icons and
/// portraits loaded with their colours.
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

/// The frames an icon is drawn over (gIcon_BasePointerTable).
enum IconBase {
  ICON_BASE_COMMAND = 0,
  ICON_BASE_COMMAND_RED = 1,
  ICON_BASE_ITEM = 2
};

/// The marks drawn over an item's icon (gIcon_MarkPointers).
enum IconMark {
  ICON_MARK_EQUIPPED = 0, ///< the E of an item equipped
  ICON_MARK_BROKEN = 1,   ///< the cross of a broken item
  /// The phi of an equipped item that cannot be removed (ICON_CURSED)
  ICON_MARK_ARTIFACT = 2
};

/// gIcon_DigitPointers: the digits 0 to 9, then the tens 10 to 30 from this
/// entry.
#define ICON_DIGIT_TENS_FIRST 10

void Icon_Decode(IconTransfer *glyph, s32 overlay);
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
  IconTransfer *state;

  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  state->encoded = gIcon_BasePointerTable[baseIcon];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, 0);
  state->encoded = gIcon_OverlayPointerTable[overlayIcon];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, 1);
  if (reuse == 0) {
    *slot = VramCache_FindFreeEntry();
  }
  *tileOut = VramCache_Load(*slot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
}

/// Unused: builds the icon of a battle action as an item's icon
/// (Icon_BuildItemIconTiles), where Ability_BuildIconTiles builds its Psynergy
/// icon.
UNUSED void Ability_BuildItemIconTiles(s32 action, s32 withBase, s32 *slot,
                                       s32 *tile, s32 reuse) {
  Icon_BuildItemIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                          reuse);
}

/// Builds item icon glyph, over the icon base when withBase is set, and loads
/// it into *cacheSlot (a free slot unless reuse is set). Returns the tile in
/// *tileOut.
void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                             s32 *tileOut, s32 reuse) {
  IconTransfer *state;
  s32 overlay;

  overlay = 0;
  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));

  if (glyph >= Icon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    state->encoded = gIcon_BasePointerTable[ICON_BASE_ITEM];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 0);
    overlay = 1;
  }

  state->encoded = gItemIcons[glyph];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, overlay);

  if (reuse == 0)
    *cacheSlot = VramCache_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
}

/// Draws an item's icon into the glyph buffer from its inventory code
/// (enum InventoryEncoding) with the layers asked for (ICON_ flags): the frame,
/// the equipped, broken and artifact marks, and the count. Returns 256, which
/// no caller reads, or -1 without the glyph buffer.
s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  IconTransfer *state;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_GetDirectFar(code & INVENTORY_ITEM_ID_MASK);
  state = Ram_HeapSlots->glyphTransfer;
  if (state == NULL)
    return -1;
  if (layers & ICON_BASE) {
    state->encoded = gIcon_BasePointerTable[ICON_BASE_ITEM];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 0);
    overlay = 1;
  }
  state->encoded = gItemIcons[item->icon];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, overlay);
  if ((layers & ICON_BROKEN) && (code & INVENTORY_BROKEN)) {
    state->encoded = gIcon_MarkPointers[ICON_MARK_BROKEN];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 1);
  }
  if ((layers & ICON_EQUIPPED) && (code & INVENTORY_EQUIPPED)) {
    state->encoded = gIcon_MarkPointers[ICON_MARK_EQUIPPED];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 1);
  }
  if ((layers & ICON_CURSED) && (code & INVENTORY_EQUIPPED) &&
      (item->flags & ITEM_CURSED) && (item->flags & ITEM_CANNOT_UNEQUIP)) {
    state->encoded = gIcon_MarkPointers[ICON_MARK_ARTIFACT];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 1);
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
  // Counts up to a full stack show: the ones digit, and over it the tens.
  if (count != 0 && count <= INVENTORY_STACK_MAX) {
    s32 ones = count % 10;

    state->encoded = gIcon_DigitPointers[ones];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 1);
    if (count / 10 != 0) {
      state->encoded =
          gIcon_DigitPointers[count / 10 + (ICON_DIGIT_TENS_FIRST - 1)];
      state->width = 2;
      state->height = 2;
      Icon_Decode(state, 1);
    }
  }
  return 256;
}

/// Draws an item's icon from its inventory code with the layers asked for
/// (ItemIcon_Compose) and loads it into VRAM cache entry cacheSlot.
s32 ItemIcon_LoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
  IconTransfer *state;

  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  ItemIcon_Compose(inventoryCode, layers);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  return 1;
}

/// Draws one of the other icons (ICON_KIND_PATTERN) and loads it into
/// cacheSlot, as ItemIcon_LoadLayersIntoSlot does an item's.
s32 Icon_BuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot) {
  IconTransfer *state;

  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  Icon_DrawMisc(icon);
  VramCache_Load(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  return 1;
}

/// Decodes item code's icon (the first icon for code 0) into the glyph buffer.
void Icon_DrawItem(s32 code) {
  IconTransfer *state = Ram_HeapSlots->glyphTransfer;
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
void Ability_BuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
                            s32 reuse) {
  Icon_BuildAbilityIconTiles(Ability_Get(action)->icon, withBase, slot, tile,
                             reuse);
}

/// Builds Psynergy icon glyph, over the icon base when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
/// in *tileOut.
void Icon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                s32 *tileOut, s32 reuse) {
  IconTransfer *state;
  s32 overlay;

  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  overlay = 0;

  if (glyph >= Icon_CountPsynergyIcons())
    glyph = 0;

  if (withBase != 0) {
    state->encoded = gIcon_BasePointerTable[ICON_BASE_ITEM];
    state->width = 2;
    state->height = 2;
    Icon_Decode(state, 0);
    overlay = 1;
  }

  state->encoded = gPsynergyIcons[glyph];
  state->width = 2;
  state->height = 2;
  Icon_Decode(state, overlay);

  if (reuse == 0)
    *cacheSlot = VramCache_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
}

/// Draws one of the other icons into the glyph buffer.
void Icon_DrawMisc(u32 index) {
  IconTransfer *state = Ram_HeapSlots->glyphTransfer;

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
  IconTransfer *state;
  u16 *table;
  u8 *entry;
  u32 index;

  state = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  table = Resource_GetTableEntry(RESOURCE_ICONS);
  if (icon < PORTRAIT_FIRST_SPRITE_PORTRAIT)
    index = icon;
  else
    index = icon - PORTRAIT_FIRST_SPRITE_PORTRAIT + PORTRAIT_FIRST_SPRITE_ICON;
  entry = (u8 *)table + table[index];
  state->encoded = entry + PALETTE_COLORS * sizeof(u16);
  state->width = 4;
  state->height = 4;
  Icon_Decode(state, 0);
  if (reuse == 0)
    *slot = VramCache_FindFreeEntry();
  *tile = VramCache_Load(*slot, 4 * 4 * TILE_SIZE_4BPP, state->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/// Unused: does nothing.
UNUSED void Icon_ReservedNoOp(void) {}

/// The tile routines RamCode_Load copies to RAM to run, and their sizes, as
/// these symbols' addresses.
extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

/// Decodes a compressed icon into the glyph buffer and expands it to tiles,
/// opaque for the first layer, or over what is there, its colour 0 clear, when
/// overlay is set. glyph->width and height are the icon's size in tiles.
void Icon_Decode(IconTransfer *glyph, s32 overlay) {
  void *code;

  RamCode_Load(code, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
  Heap_Release(code);
  if (overlay)
    RamCode_Load(code, Tile_ExpandMaskedCode, Tile_ExpandMaskedCodeSize);
  else
    RamCode_Load(code, Tile_ExpandOpaqueCode, Tile_ExpandOpaqueCodeSize);
  ((void (*)(u8 *, u8 *, u32, u32))code)(glyph->input, glyph->tiles,
                                         (u16)glyph->width, (u16)glyph->height);
  Heap_Release(code);
}
