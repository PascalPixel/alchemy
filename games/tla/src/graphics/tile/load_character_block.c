/// @file
/// Tiles: load a backdrop into one half of the BG character and palette
/// storage, its colours greyed.

#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "mem.h"

typedef s32 (*PackedDecoder)(const u8 *source, void *destination, u32 fill);

extern const u8 BattleEffect_ExpandCode[];
extern u16 gFlash_Handler3[];
extern u8 BattleEffect_ExpandCodeSize[];

void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
const u8 *Resource_GetTableEntry(u32 resourceId);

/// Loads a backdrop resource (its 128 colours, then its packed tiles) into
/// one half of BG VRAM and queues its palette for the next VBlank, as The
/// Broken Seal's Credits_LoadBackground does, but greyed: each channel
/// becomes (2 red + 4 green + blue + 5 * channel) / 16. A zero resource
/// clears that half instead. The tiles are unpacked by the
/// BattleEffect_Expand kernel, copied by DMA into a heap block for the call;
/// the grey colours are built in the EWRAM buffer at gFlash_Handler3.
void Func_081a052c(u32 resourceId, s32 alternate) {
  const u8 *resourceData;
  u32 paletteDestination;
  u32 paletteOffset;
  void *tileDestination;
  PackedDecoder decoder;
  u32 size;
  s32 i;

  if (resourceId == 0) {
    Graphics_ClearCharacterBlockAndPalette(alternate);
    return;
  }
  resourceData = Resource_GetTableEntry(resourceId);
  if (alternate == 0) {
    paletteOffset = 0;
    tileDestination = (void *)0x06000000;
    paletteDestination = 0x05000000;
  } else {
    paletteOffset = 0x80808080;
    tileDestination = (void *)0x06008000;
    paletteDestination = 0x05000100;
  }
  size = (u32)BattleEffect_ExpandCodeSize;
  decoder = (PackedDecoder)Heap_Alloc(size);
  Dma_Set(BattleEffect_ExpandCode, decoder,
          (DMA_ENABLE | DMA_32BIT) << 16 | (size >> 2), REG_DMA3);
  decoder(resourceData + 256, tileDestination, paletteOffset);
  Heap_Release(decoder);

  for (i = 0; i < 128; i++) {
    u16 color = *(const s16 *)resourceData;
    s32 blue = (color >> 10) & 0x1f;
    s32 green = (color >> 5) & 0x1f;
    s32 red = color & 0x1f;
    s32 sum = (green * 2 + red) * 2 + blue;

    resourceData += 2;
    blue = (sum + blue * 5) / 16;
    green = (sum + green * 5) / 16;
    red = (sum + red * 5) / 16;
    gFlash_Handler3[i] = blue << 10 | green << 5 | red;
  }
  IO_WRITE_QUEUE_PUSH(paletteDestination, (u32)gFlash_Handler3,
                      DMA_COPY_WORDS(64));
}
