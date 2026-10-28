#include "types.h"
#include "graphics.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "glyph.h"
#include "dma.h"
#include "io_reg.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1) {
  GlyphTransfer *buffer = HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **slot = &buffer->encoded;
  void *target = (void *)((u32)base + base[arg1]);
  s32 ret;

  *slot = target;
  Resource_DecodeByteLzInRam(target, buffer);
  ret = VramCache_Reload(arg0, (s32)buffer);
  HeapSlot_Release(68);
  return ret;
}

void Text_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph,
                              s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *src;
  u8 *dst;
  s32 i;
  u32 index;
  u32 vram;
  s32 offset;

  decoded = (GlyphTransfer *)HeapSlot_Alloc(68, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Resource_DecodeByteLzInRam(decoded->encoded, decoded);
  remapped = Heap_Alloc(0x400);
  src = decoded->input;
  dst = remapped;
  for (i = 0; i < 0x400; i++) {
    index = *src++;
    if (palette->slot[index] == 255) {
      palette->slot[index] = palette->count;
      if (palette->count < 64) {
        ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
        palette->count++;
      }
    }
    *dst++ = palette->slot[index];
  }
  // The native copies the tile index down, loads the VRAM base and only then
  // shifts the index; plain C shifts before the load (differ 2). Three barriers
  // keep that order: one barrier on the base alone swaps r1 and r2 (differ 6),
  // and the offset as its own barrier-held value puts them back.
  /* FAKEMATCH: keeps the index copy first (differ 2 without) */
  __asm__("" : : "l"(tile));
  vram = 0x06004000;
  /* FAKEMATCH: keeps the base load before the shift (differ 2 without) */
  __asm__("" : : "r"(vram));
  offset = tile * 64;
  /* FAKEMATCH: keeps the offset in r1 and the base in r2 (differ 6 without) */
  __asm__("" : : "r"(offset));
  Dma_Set(remapped, (void *)(offset + vram), 0x84000100, REG_DMA3);
  Heap_Release(remapped);
  HeapSlot_Release(68);
}
