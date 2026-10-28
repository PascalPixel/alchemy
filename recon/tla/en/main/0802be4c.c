// Near miss, differ 38 of 218 (2026-10-18), first pass: like The Broken
// Seal's WorldMap_LoadGraphics, with this game's four graphics sets chosen by
// the region (bits 24-29) of the block under (x, z) through Func_0802d088
// and region / 10. Left: the native loads each decode buffer into r5 one call
// earlier (Data_0202d000 before Func_080c8970), forms gMapBlocks in r2, and
// keeps the backdrop colour store before the palette DMA's operands. Naming
// the buffers as variables assigned there gives r5 to the resources and r6
// to the buffers (differ 92).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "mem.h"
#include "resource.h"
#include "graphics.h"

extern u32 gMapBlocks[];
extern const u32 gData_0802ed34[4][9];
extern u8 Data_0202d000[];
extern u8 gData_02038000[];
extern u8 gData_0203a000[];
extern u8 gData_0203c000[];
extern u8 gData_0203e000[];
extern u8 gData_02028000[];
extern u8 gMapCollision[];
extern u8 gWorldMapWork[];

void Func_0802d088(s32 region, s32 mode);
void Func_080c8970(void *palette);
void MapAnimation_StartChannels(const u16 *script);

void Func_0802be4c(s32 x, s32 z) {
  u8 *buffer;
  struct MapViewWork *view;
  u32 *block;
  const u32 *resources;
  s32 graphicsSet;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(0x200);
  view = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  block = &gMapBlocks[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
  Func_0802d088((*block << 2) >> 26, 0);
  switch ((s32)((*block << 2) >> 26) / 10) {
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
  Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
  Func_080c8970(buffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]), Data_0202d000);
  MapAnimation_StartChannels((const u16 *)Data_0202d000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]), gMapCollision);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]), gData_02038000);
  Dma_Set(gData_02038000, (void *)0x06008000, 0x84000800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]), gData_0203a000);
  Dma_Set(gData_0203a000, (void *)0x0600a000, 0x84000800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gData_0203c000);
  Dma_Set(gData_0203c000, (void *)0x0600c000, 0x84000800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]), gData_0203e000);
  Dma_Set(gData_0203e000, (void *)0x0600e000, 0x84000800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
  Dma_Fill32(0xf07ff07f, (void *)0x06002800, 0x600);
  screenTiles = (u32 *)0x06003000;
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
