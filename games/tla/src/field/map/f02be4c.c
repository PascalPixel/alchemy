#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "heap_release.h"
#include "resource.h"
#include "graphics.h"

#define Ram_Data_02038000 FIXED_ADDRESS((u8 *), 0x02038000, gData_02038000)

/// One word of the table at Ram_MapBlocks, 32 words to a row.
struct Data02020000 {
  u32 unk0 : 24;
  /// Passed to Func_0802d088; region / 10 picks the set. The Broken Seal's
  /// WorldMapCell keeps the encounter region at these bits.
  u32 region : 6;
  u32 unk1e : 2;
};

extern const u32 gData_0802ed34[4][9];
extern u8 gData_02028000[];
extern u8 gWorldMapWork[];

void Func_0802d088(s32 region, s32 mode);
void Func_080c8970(void *palette);
void Func_0802cc9c(const u16 *script);

void Func_0802be4c(s32 x, s32 z) {
  u8 *buffer;
  struct MapViewWork *view;
  struct Data02020000 *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 region;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(512);
  view = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  block =
      &((struct Data02020000 *)Ram_MapBlocks)[((x / (32 << 16)) & 31) +
                                              (((z / (32 << 16)) & 31) << 5)];
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
  backdropColor = *(s16 *)BG_PLTT;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Func_080c8970(buffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]),
                        Ram_MapCollision + 0x1000);
  Func_0802cc9c((const u16 *)(Ram_MapCollision + 0x1000));
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]),
                        Ram_Data_02038000);
  Dma_Set(Ram_Data_02038000, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_Data_02038000 + 0x2000);
  Dma_Set(Ram_Data_02038000 + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]),
                        Ram_Data_02038000 + 0x4000);
  Dma_Set(Ram_Data_02038000 + 0x4000, BG_CHAR_BLOCK(3), DMA_COPY_WORDS(2048),
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]),
                        Ram_Data_02038000 + 0x6000);
  Dma_Set(Ram_Data_02038000 + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
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
