// Matches: differ 0 (tla-en, 2026-11-08), no device.
// What closed it from 30 (8 November): three plain-C shapes, read from the
// sched2, lreg and greg dumps.
// - The five decode buffers used twice as constant addresses (FIXED_ADDRESS),
//   as The Broken Seal's WorldMap_LoadGraphics writes Ram_BgTileBuffer +
//   0x2000: a symbol is forced into a .LC pool MEM, and sched2 cannot move a
//   memory load over a call, so each buffer loaded one call late; a constant
//   is a plain set of r5, and sched2 lifts it over the calls before it (the
//   GetTableEntry call, and Func_080c8970 or the gMapCollision decode) up to
//   the previous Dma_Set's volatile asm or the previous buffer's last use,
//   where the listing has it (30 to 20). The pool comes out in the listing's
//   order (gData_02038000 before gMapCollision). Ram_MapBlocks, the header's
//   own, closes the block address and the backdrop registers (20 to 6; the
//   gMapBlocks symbol alone is 14). Ram_MapCollision, as Func_0802a6b8 writes
//   it, and the gMapCollision symbol measure the same.
// - The region divided in place (region = block->region; region /= 10): the
//   local then dies twice and lives across the switch, so it goes to
//   global-alloc; local-alloc cannot tie the shifted word to it, and the word
//   takes r3 while the dividend takes r0 (6 to 0). As one expression, or as a
//   separate quotient local, all three tie into r0 (6).
// - The region as a bit-field (bits 24-29, unsigned, promoted to int): the
//   division is __divsi3 with no cast, as the listing calls it.
// All six editions measure differ 0 (en, ja, de, es, fr, it); the literal pool
// holds the listing's words in its order, the buffers and Ram_MapBlocks as the
// addresses their symbols name. On adoption the five
// Ram_Data_ addresses belong in ram_buffer.h, each listed once (O2).
// Earlier rounds' figures (30 and up) were all with symbol buffers: buffer
// variables reached the listing's load positions but swapped r5 and r6 (84),
// because a reused local goes to global-alloc after the resources pointer.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "heap_release.h"
#include "resource.h"
#include "graphics.h"

#define Ram_Data_0202d000 FIXED_ADDRESS((u8 *), 0x0202d000, Data_0202d000)
#define Ram_Data_02038000 FIXED_ADDRESS((u8 *), 0x02038000, gData_02038000)
#define Ram_Data_0203a000 FIXED_ADDRESS((u8 *), 0x0203a000, gData_0203a000)
#define Ram_Data_0203c000 FIXED_ADDRESS((u8 *), 0x0203c000, gData_0203c000)
#define Ram_Data_0203e000 FIXED_ADDRESS((u8 *), 0x0203e000, gData_0203e000)

/// One word of the map's block table (Ram_MapBlocks), 32 blocks to a row.
struct MapBlock {
  u32 unk0 : 24;
  u32 region : 6; ///< passed to Func_0802d088; region / 10 picks the set
  u32 unk30 : 2;
};
#define MAP_BLOCKS ((struct MapBlock *)Ram_MapBlocks)

extern const u32 gData_0802ed34[4][9];
extern u8 gData_02028000[];
extern u8 gWorldMapWork[];

void Func_0802d088(s32 region, s32 mode);
void Func_080c8970(void *palette);
void MapAnimation_StartChannels(const u16 *script);

void Func_0802be4c(s32 x, s32 z) {
  u8 *buffer;
  struct MapViewWork *view;
  struct MapBlock *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 region;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(0x200);
  view = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  block = &MAP_BLOCKS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
  Func_0802d088(block->region, 0);
  // Reused: the region, then its group of ten.
  region = block->region;
  region /= 10;
  switch (region) {
  case 3:
    graphicsSet = 3;
    break;
  case 2:
    graphicsSet = 2;
    break;
  case 4:
    graphicsSet = 1;
    break;
  default:
    graphicsSet = 0;
    break;
  }
  resources = gData_0802ed34[graphicsSet];
  view->resources = (u32 *)resources;
  backdropColor = *(s16 *)0x05000000;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Func_080c8970(buffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]),
                        Ram_Data_0202d000);
  MapAnimation_StartChannels((const u16 *)Ram_Data_0202d000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]),
                        Ram_Data_02038000);
  Dma_Set(Ram_Data_02038000, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_Data_0203a000);
  Dma_Set(Ram_Data_0203a000, BG_CHAR_BLOCK(2) + 0x2000, DMA_COPY_WORDS(2048),
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]),
                        Ram_Data_0203c000);
  Dma_Set(Ram_Data_0203c000, BG_CHAR_BLOCK(3), DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]),
                        Ram_Data_0203e000);
  Dma_Set(Ram_Data_0203e000, BG_CHAR_BLOCK(3) + 0x2000, DMA_COPY_WORDS(2048),
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 1536);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01810180;
  for (row = 0; row < 20; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 24; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 4; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  Heap_Release(buffer);
}
