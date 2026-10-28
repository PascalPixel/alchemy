#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "glyph.h"
#include "resource.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "select.h"

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

    /* FAKEMATCH: retain the size load after saving both arguments. */
    do {
      size = (u32)Tile_Decompress4bppCodeSize;
    } while (0);
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_Decompress4bppCode, code,
            (DMA_ENABLE | DMA_32BIT) << 16 | size, REG_DMA3);
  }
  ((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
  Heap_Release(code);
  if (outlined) {
    u32 size;

    size = (u32)Tile_ExpandMaskedCodeSize;
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_ExpandMaskedCode, code,
            (DMA_ENABLE | DMA_32BIT) << 16 | size, REG_DMA3);
  } else {
    u32 size;

    size = (u32)Tile_ExpandOpaqueCodeSize;
    code = Heap_Alloc(size);
    size >>= 2;
    Dma_Set((const void *)Tile_ExpandOpaqueCode, code,
            (DMA_ENABLE | DMA_32BIT) << 16 | size, REG_DMA3);
  }
  ((void (*)(u8 *, u8 *, u32, u32))code)(glyph->input, glyph->tiles,
                                         (u16)glyph->width, (u16)glyph->height);
  Heap_Release(code);
}
