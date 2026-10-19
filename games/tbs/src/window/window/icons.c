/// @file
/// Part of window.c, the window object, compiled only through it.
/// Item, Psynergy and command icons.

#include "../window_internal.h"

/// Returns how many item icons there are.
s32 UiIcon_CountItemIcons(void) {
  return gUiIcon_ItemIconPointersEnd - gUiIcon_ItemIconPointers;
}

/// Returns how many Psynergy icons there are.
s32 UiIcon_CountPsynergyIcons(void) {
  return gUiIcon_PsynergyIconPointersEnd - gUiIcon_PsynergyIconPointers;
}

/// Builds a command icon (overlay no0 over frame no1) and loads it into a
/// sprite tile slot: *slot, or a free one when flag is 0. Returns the tile in
/// *ret.
/// Frame decode is opaque; the command layer is then composited with
/// transparency.
void Ui_BuildPairedPatternsToSlot(s32 overlayIcon, s32 frameIcon, s32 *slot,
                                  s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  work->encoded = gUiIcon_FramePointerTable[frameIcon];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, 0);
  work->encoded = gUiIcon_OverlayPointerTable[overlayIcon];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, 1);
  if (reuse == 0) {
    *slot = Resource_FindFreeEntry();
  }
  *tileOut = VramCache_Load(*slot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

/// Builds the icon of a battle action's item (its first operand) into a slot,
/// as UiIcon_BuildItemIconTiles.
void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                          s32 reuse) {
  UiIcon_BuildItemIconTiles(BattleActGet(action)->unk04[0], withBase, slot,
                            tile, reuse);
}

/// Builds item icon glyph, over the icon frame when withBase is set, and loads
/// it into *cacheSlot (a free slot unless reuse is set). Returns the tile in
/// *tileOut.
/// The optional frame is decoded first and selects masked icon expansion.
void UiIcon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                               s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  overlay = 0;
  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

  if (glyph >= UiIcon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gUiIcon_ItemIconPointers[glyph];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, overlay);

  if (reuse == 0)
    *cacheSlot = Resource_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

/// Draws an item's icon into the glyph work buffer from its inventory code
/// (item id in the low 9 bits, broken 0x200, equipped 0x400, count less one in
/// the top 5 bits) with the layers asked for (ICON_ flags): the frame, the
/// equipped, broken and artifact marks, and the count. Returns 256, or -1 when
/// there is no glyph work buffer.
s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_GetDirectFar(code & 0x1ff);
  work = gGlyphWork;
  if (work == NULL)
    return -1;
  if (layers & ICON_FRAME) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    overlay = 1;
  }
  work->encoded = gUiIcon_ItemIconPointers[item->icon];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, overlay);
  if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
    work->encoded = gUiIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if ((layers & ICON_BROKEN) && (code & 0x200)) {
    work->encoded = gUiIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) &&
      (item->flags & 2)) {
    work->encoded = gUiIcon_MarkPointers[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if (layers & ICON_COUNT_ABOVE_ONE) {
    count = (code & 0xf800) >> 11;
    count++;
    if (count <= 1)
      count = 0;
  }
  if (layers & ICON_COUNT) {
    count = (code & 0xf800) >> 11;
    count++;
  }
  if (count != 0 && count <= 30) {
    s32 ones = count % 10;

    work->encoded = gUiIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
    if (count / 10 != 0) {
      work->encoded = gUiIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      UiIcon_Decode(work, 1);
    }
  }
  return 256;
}

/// Composes an inventory icon with the requested layers and loads it into
/// cacheSlot. The temporary glyph block is released after the upload.
s32 UiIcon_CopyResourceToSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  ItemIcon_Compose(inventoryCode, layers);
  VramCache_Load(cacheSlot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return 1;
}

/// Builds one of the other icons and loads it into sprite tile slot slot.
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 cacheSlot) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  Ui_PrepareTransferFromTableEntry(icon);
  VramCache_Load(cacheSlot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return 1;
}
extern const u8 *gRenderResource_PairSourceTable[];

/// Loads one of four small fixed images into sprite tile slot slot.
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot) {
  /* FAKEMATCH: three more names of the one table keep each case's own literal
   * slot; with the one name in every case the switch folds away, differ 21 on
   * tbs-en (9 instructions for 24). */
  extern const u8 *PairSource1[] __asm__("gRenderResource_PairSourceTable"),
      *PairSource2[] __asm__("gRenderResource_PairSourceTable"),
      *PairSource3[] __asm__("gRenderResource_PairSourceTable");
  void *source;
  switch (value) {
  case 1:
    source = PairSource1;
    break;
  case 2:
    source = PairSource2;
    break;
  case 3:
    source = PairSource3;
    break;
  case 0:
  default:
    source = gRenderResource_PairSourceTable;
    break;
  }
  VramCache_Load(slot, 32, source);
  return 1;
}

/// Draws an item's icon from its inventory code into the glyph work buffer.
void Ui_PrepareTransferForItem(s32 code) {
  GlyphTransfer *glyph = gGlyphWork;
  struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

  if (code != 0) {
    glyph->encoded = gUiIcon_ItemIconPointers[item->icon];
  } else {
    glyph->encoded = gUiIcon_ItemIconPointers[0];
  }
  glyph->width = 2;
  glyph->height = 2;
  UiIcon_Decode(glyph, 0);
}

/// Builds the Psynergy icon of a battle action, as
/// UiIcon_BuildAbilityIconTiles.
void Ability_LoadGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                       s32 reuse) {
  UiIcon_BuildAbilityIconTiles(BattleActGet(action)->unk04[0], withBase, slot,
                               tile, reuse);
}

#define ROUTINE_BLOCK 49

/// Builds Psynergy icon glyph, over the icon frame when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set).
/// Returns the tile in *tileOut.
/// The optional frame is decoded first and selects masked icon expansion.
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                  s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  overlay = 0;

  if (glyph >= UiIcon_CountPsynergyIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    overlay = 1;
  }

  work->encoded = gUiIcon_PsynergyIconPointers[glyph];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, overlay);

  if (reuse == 0)
    *cacheSlot = Resource_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

/// Draws one of the other icons into the glyph work buffer.
void Ui_PrepareTransferFromTableEntry(u32 iconIndex) {
  GlyphTransfer *glyph = gGlyphWork;

  glyph->encoded = gUiIcon_MiscIconPointers[iconIndex];
  glyph->width = 2;
  glyph->height = 2;
  UiIcon_Decode(glyph, 0);
}

/// Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a
/// free slot unless reuse is set) and its colours into sprite palette palette.
/// Returns the tile in *tile.
/// This resource stores its palette before the encoded tile payload.
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse) {
  GlyphTransfer *work;
  u16 *offsets;
  u8 *encodedEntry;
  u32 index;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  offsets = Resource_GetTableEntry((s32)&ResourceId_Icons);
  if (icon <= 127)
    index = icon;
  else
    index = icon - 112;
  encodedEntry = (u8 *)offsets + offsets[index];
  work->encoded = encodedEntry + 32;
  work->width = 4;
  work->height = 4;
  UiIcon_Decode(work, 0);
  if (reuse == 0)
    *slot = Resource_FindFreeEntry();
  *tile = VramCache_Load(*slot, 0x200, work->tiles);
  Runtime_ReleaseHeapBlock(17);
  Dma_Set(encodedEntry, (void *)&OBJ_PLTT_COLOR(palette, 0), DMA_COPY16(16),
          REG_DMA3);
}

/// Does nothing.
void UiGlyph_ReservedNoOp(void) {}

/// Decodes a compressed icon into the glyph buffer and expands it to tiles,
/// opaque for the first layer or masked over what is there when overlay is set.
/// Release each copied decoder before loading the next routine into the same
/// slot.
void UiIcon_Decode(GlyphTransfer *glyph, s32 outlined) {
  RamCode_LoadSlot(ROUTINE_BLOCK, Tile_Decompress4bpp,
                   Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))gWorkSlot[ROUTINE_BLOCK])(glyph->encoded,
                                                           glyph->input);
  Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
  if (outlined)
    RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandMasked,
                     Tile_ExpandMaskedCodeSize);
  else
    RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandOpaque,
                     Tile_ExpandOpaqueCodeSize);
  ((void (*)(u8 *, u8 *, u32, u32))gWorkSlot[ROUTINE_BLOCK])(
      glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
  Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}
