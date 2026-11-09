/// @file
/// Item, Psynergy and command icons.
// Part of window.c.

#include "../window_internal.h"

s32 Icon_CountItemIcons(void) {
  return gItemIconsEnd - gItemIcons;
}

s32 Icon_CountPsynergyIcons(void) {
  return gPsynergyIconsEnd - gPsynergyIcons;
}

extern u8 *gIcon_BasePointerTable[];
extern u8 *gIcon_OverlayPointerTable[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);
s32 Resource_FindFreeEntry(void);

void Icon_BuildCommandIconTiles(s32 no0, s32 no1, s32 *slot, s32 *ret,
                                  s32 flag) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  work->encoded = gIcon_BasePointerTable[no1];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, 0);
  work->encoded = gIcon_OverlayPointerTable[no0];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, 1);
  if (flag == 0)
    *slot = Resource_FindFreeEntry();
  *ret = VramCache_Load(*slot, 0x80, work->tiles);
  HeapSlot_Release(68);
}

extern u8 *gIcon_BasePointerTable[];

void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                               s32 *tileOut, s32 reuse);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);
s32 Icon_CountItemIcons(void);

u8 *BattleAction_Get(s32);

/// Builds the icon of a battle action's item.
// As ☀️'s Ability_RequestGlyph.
void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                          s32 reuse) {
  Icon_BuildItemIconTiles(*(u16 *)(BattleAction_Get(action) + 4), withBase, slot,
                            tile, reuse);
}

/// Builds item icon glyph, over the icon base when withBase is set, and loads
/// it into *cacheSlot (a free slot unless reuse is set). Returns the tile in
/// *tileOut.
// As ☀️'s function of the same name.
void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                               s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  overlay = 0;
  work = HeapSlot_Alloc(68, sizeof(GlyphTransfer));

  if (glyph >= Icon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }

  // Plainly, work->encoded = gItemIcons[glyph] differs in 2 of
  // 83 instructions in tla-en: ⚓️ loads the table's address between the two
  // instructions of encoded's split offset 0x604, where agscc finishes the
  // shift first.
  {
    u32 offset = 192;
    u8 **table = gItemIcons;
    /* FAKEMATCH(pin): the field's address held in r1, as the native keeps it;
     * as a plain local, 12 of 83 instructions differ in tla-en. */
    register u8 **field asm("r1");

    /* FAKEMATCH(barrier): an empty asm reading the table with the offset's
     * first part, which loads the table before the shift; as no barrier, 11 of
     * 83 instructions differ in tla-en. */
    asm volatile("" : "+l"(offset) : "l"(table));
    offset = (offset << 3) + 4;
    field = (u8 **)((u8 *)work + offset);
    /* FAKEMATCH(barrier): an empty asm on the field's address, which keeps the
     * + 4 out of the store; as no barrier, 9 of 83 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(field));
    // Indexed as an integer sum: table[glyph] reads [index, table] (differ 2).
    *field = *(u8 **)((u32)table + (glyph << 2));
  }
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);

  if (reuse == 0)
    *cacheSlot = Resource_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 0x80, work->tiles);
  HeapSlot_Release(68);
}

extern u8 *gIcon_BasePointerTable[];
extern u8 *gItemIcons[];
extern u8 *gIcon_MarkPointers[];
extern u8 *gIcon_DigitPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

#define ICON_BASE 1 ///< the base under the icon
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_Get(code & 0x1ff);
  work = Ram_HeapSlots->glyphWork;
  if (work == NULL)
    return -1;
  if (layers & ICON_BASE) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }
  work->encoded = gItemIcons[item->icon];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);
  if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
    work->encoded = gIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_BROKEN) && (code & 0x200)) {
    work->encoded = gIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) &&
      (item->flags & 2)) {
    work->encoded = gIcon_MarkPointers[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
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

    work->encoded = gIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (count / 10 != 0) {
      work->encoded = gIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
  }
  return 256;
}

s32 Icon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(0x44, sizeof(GlyphTransfer));
  ItemIcon_Compose(arg0, arg1);
  VramCache_Load(arg2, 0x80, work->tiles);
  HeapSlot_Release(0x44);
  return 1;
}

s32 Icon_BuildMiscIconTiles(s32 no, s32 unused, s32 slot) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(0x44, sizeof(GlyphTransfer));
  Icon_DrawMisc(no);
  VramCache_Load(slot, 0x80, work->tiles);
  HeapSlot_Release(0x44);
  return 1;
}

extern u8 *gItemIcons[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);

/// Decodes item code's icon (the first icon for code 0) into the glyph work.
void Icon_DrawItem(s32 code) {
  GlyphTransfer *state = Ram_HeapSlots->glyphWork;
  struct ItemDefinition *item = Item_Get(0x1ff & code);

  if (code != 0)
    state->encoded = gItemIcons[item->icon];
  else
    state->encoded = gItemIcons[0];
  state->width = 2;
  state->height = 2;
  UiGlyph_DecodeWithHeapRoutines(state, 0);
}

struct BattleActionGlyph {
  u8 unk00[4];
  u16 icon;
};

extern u8 *gIcon_BasePointerTable[];
extern u8 *gPsynergyIcons[];

void Icon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                  s32 *tileOut, s32 reuse);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);
s32 Icon_CountPsynergyIcons(void);

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse) {
  Icon_BuildAbilityIconTiles(
      ((struct BattleActionGlyph *)BattleAction_Get(action))->icon, with_base,
      slot, tile, reuse);
}

/// Builds Psynergy icon glyph, over the icon base when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
/// in *tileOut.
// As ☀️'s function of the same name.
void Icon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                  s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  work = HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  overlay = 0;

  if (glyph >= Icon_CountPsynergyIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }

  // Plainly, work->encoded = gPsynergyIcons[glyph] differs in 2
  // of 83 instructions in tla-en, as in Icon_BuildItemIconTiles: ⚓️ loads
  // the table's address between the two instructions of encoded's split
  // offset 0x604, where agscc finishes the shift first.
  {
    u32 offset = 192;
    u8 **table = gPsynergyIcons;
    /* FAKEMATCH(pin): the field's address held in r1, as the native keeps it;
     * as a plain local, 12 of 83 instructions differ in tla-en. */
    register u8 **field asm("r1");

    /* FAKEMATCH(barrier): an empty asm reading the table with the offset's
     * first part, which loads the table before the shift; as no barrier, 11 of
     * 83 instructions differ in tla-en. */
    asm volatile("" : "+l"(offset) : "l"(table));
    offset = (offset << 3) + 4;
    field = (u8 **)((u8 *)work + offset);
    /* FAKEMATCH(barrier): an empty asm on the field's address, which keeps the
     * + 4 out of the store; as no barrier, 9 of 83 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(field));
    // Indexed as an integer sum: table[glyph] reads [index, table] (differ 2).
    *field = *(u8 **)((u32)table + (glyph << 2));
  }
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);

  if (reuse == 0)
    *cacheSlot = Resource_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 0x80, work->tiles);
  HeapSlot_Release(68);
}

extern char ResourceId_Icons;

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

void Icon_DrawMisc(u32 index) {
  GlyphTransfer *state = Ram_HeapSlots->glyphWork;

  state->encoded = gIcon_MiscIconPointers[index];
  state->width = 2;
  state->height = 2;
  UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse) {
  GlyphTransfer *work;
  u16 *table;
  u8 *entry;
  u32 index;

  work = HeapSlot_Alloc(68, 0x608);
  table = Resource_GetTableEntry((s32)&ResourceId_Icons);
  if (icon <= 127)
    index = icon;
  else
    index = icon - 112;
  entry = (u8 *)table + table[index];
  work->encoded = entry + 32;
  work->width = 4;
  work->height = 4;
  UiGlyph_DecodeWithHeapRoutines(work, 0);
  if (reuse == 0)
    *slot = Resource_FindFreeEntry();
  *tile = VramCache_Load(*slot, 0x200, work->tiles);
  HeapSlot_Release(68);
  Dma_Set(entry, (void *)(0x05000200 + palette * 32), DMA_COPY_HALFWORDS(16),
          REG_DMA3);
}

void UiGlyph_ReservedNoOp(void) {}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined) {
  void *code;

  {
    u32 size;

    // The size's load in a once-only block keeps it after both arguments are
    // saved; as a plain assignment, 4 of 82 instructions differ in tla-en.
    do {
      size = (u32)Tile_Decompress4bppCodeSize;
    } while (0);
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_Decompress4bppCode, code, DMA_COPY_WORDS(size),
            REG_DMA3);
  }
  ((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
  Heap_Release(code);
  if (outlined) {
    u32 size;

    size = (u32)Tile_ExpandMaskedCodeSize;
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_ExpandMaskedCode, code, DMA_COPY_WORDS(size),
            REG_DMA3);
  } else {
    u32 size;

    size = (u32)Tile_ExpandOpaqueCodeSize;
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_ExpandOpaqueCode, code, DMA_COPY_WORDS(size),
            REG_DMA3);
  }
  ((void (*)(u8 *, u8 *, u32, u32))code)(glyph->input, glyph->tiles,
                                         (u16)glyph->width, (u16)glyph->height);
  Heap_Release(code);
}

#undef ICON_BASE
#undef ICON_COUNT_ABOVE_ONE
#undef ICON_COUNT
#undef ICON_EQUIPPED
#undef ICON_BROKEN
#undef ICON_ARTIFACT
