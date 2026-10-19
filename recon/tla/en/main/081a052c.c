// Draft, differ 84 of 138: ⚓️'s Graphics_LoadCharacterBlockAndPalette,
// ☀️'s in games/tbs/src/graphics/credits/credits.c: loads a backdrop's tiles
// into one half of BG VRAM through the BattleEffect_Expand kernel (copied
// into a heap block by DMA here, not a RAM-code slot), then queues its
// palette, but first greys it: each colour channel becomes
// (2r + 4g + b + 5 * channel) / 16, in a buffer at gFlash_Handler3 (the
// listing's label for that EWRAM block). Everything before the loop matches
// but for pool offsets. Left: other: the native loads the 0x1f mask from the
// literal pool into lr (a pooled constant, as a halfword one would be) and
// places that pool inside the loop after the blue channel's divide (with a
// branch over it), where here the mask is movs into ip and the pool follows
// the function; with it the loop's registers differ (counter r6 vs r7,
// source r7 vs r0, the channels r0/r1/r4 vs r4/r1/r5). The channel
// extraction from the colour shifted into the high half is the native's
// (red as (value & 0x1f0000) >> 16). Tried: the colour as u16 (168), as s32
// with (u16) casts per channel (129).
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

void Graphics_LoadCharacterBlockAndPalette(u32 resourceId, s32 alternate) {
  const u8 *resourceData;
  const s16 *color;
  u16 *grey;
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
  decoder = (PackedDecoder)MemAlloc(size);
  Dma_Set(BattleEffect_ExpandCode, decoder, 0x84000000 | (size >> 2),
          REG_DMA3);
  decoder(resourceData + 256, tileDestination, paletteOffset);
  MemFree(decoder);

  color = (const s16 *)resourceData;
  grey = gFlash_Handler3;
  for (i = 127; i >= 0; i--) {
    u32 value = *color++ << 16;
    s32 blue = (value >> 26) & 0x1f;
    s32 green = (value >> 21) & 0x1f;
    s32 red = (value & 0x1f0000) >> 16;
    s32 sum = (green * 2 + red) * 2 + blue;

    blue = (sum + blue * 5) / 16;
    green = (sum + green * 5) / 16;
    red = (sum + red * 5) / 16;
    *grey++ = blue << 10 | green << 5 | red;
  }
  IO_WRITE_QUEUE_PUSH(paletteDestination, (u32)gFlash_Handler3, 0x84000040);
}
