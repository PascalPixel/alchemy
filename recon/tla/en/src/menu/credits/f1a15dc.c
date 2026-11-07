// Shows a scrolling picture on the hidden one of the two BG character blocks.
// With no resource it fills the map buffer with a plain tile word and clears
// the palette half; otherwise it clears the block's first 2 KB, expands
// resource's map into the buffer with BattleEffect_Expand2Code copied to the
// heap (the fill word marks the second block's tiles), and queues the
// resource's 64-colour palette. Then it copies the map at position into the
// tilemap (Func_081a1294, inlined), flips the block and restarts the fade-in
// counter. Func_081a1294's body is inlined here, so in the module it is a GNU
// inline function defined before this one. The size is a linker symbol (the
// pool holds 0x234 where movs and a shift would do).
#include "types.h"
#include "dma.h"
#include "ram_code.h"
#include "heap_release.h"
#include "resource.h"
#include "io_write_queue.h"
#include "display_scroll.h"

extern u32 Data_02007518;
extern u32 gData_0200751c;
extern u16 gData_02007520;
extern u8 *Data_02007528;
extern s16 Data_02007514;
extern s16 Data_02007516;
extern const u8 BattleEffect_Expand2Code[];
extern u8 BattleEffect_Expand2CodeSize[];

static inline void Func_081a1294(const u8 *source, u8 *destination,
                                 s32 position) {
  u32 start;
  s32 column;
  s32 row;
  u32 offset;
  u32 from;

  start = (Data_02007518 >> 19) << 6;
  gScrollTarget = gData_02007520 = position;
  gData_0200751c = position << 16;
  for (column = 0; column < 15; column++) {
    u8 *to = destination + column * 0x800;
    const u8 *strip = source + column * 0x1000;
    from = (position / 8) << 6;
    offset = start & 0x7ff;
    for (row = 0; row < 31; row++) {
      Dma_Set(strip + from, to + offset, 0x84000010,
              (volatile u32 *)0x040000d4);
      from += 64;
      offset = (offset + 64) & 0x7ff;
    }
  }
}

void Func_081a15dc(s32 resource, u8 *map, s32 position) {
  /* FAKEMATCH(volatile): the DMA fill word volatile, so its address stays in a
   * register; as a plain u32, 10 of 209 instructions differ in tla-en. */
  volatile u32 cell;
  u32 word;
  u32 fill = 0;
  u8 *tiles;
  u16 *palette;
  u8 *data;
  s32 (*code)(u8 *, u8 *, u32);

  if (resource == 0) {
    if (Data_02007514 != 0) {
      word = 0x01010101;
      tiles = (u8 *)0x06000000;
      palette = (u16 *)0x05000000;
    } else {
      word = 0x81818181;
      tiles = (u8 *)0x06008000;
      palette = (u16 *)0x05000100;
    }
    Dma_Fill32Cell(cell, word, map, 0xf000);
    Dma_Fill32Cell(cell, 0, palette, 0x100);
  } else {
    if (Data_02007514 != 0) {
      tiles = (u8 *)0x06000000;
      palette = (u16 *)0x05000000;
    } else {
      fill = 0x80808080;
      tiles = (u8 *)0x06008000;
      palette = (u16 *)0x05000100;
    }
    Dma_Fill32Cell(cell, 0, tiles, 0x800);
    data = Resource_GetTableEntry(resource);
    RamCode_Load(code, BattleEffect_Expand2Code, BattleEffect_Expand2CodeSize);
    code(data + 0x100, map, fill);
    Heap_Release(code);
    IO_WRITE_QUEUE_PUSH((u32)palette, (u32)data, 0x84000040);
  }
  Data_02007528 = map;
  Func_081a1294(map, tiles, position);
  Data_02007514 ^= 1;
  Data_02007516 = 0;
}
