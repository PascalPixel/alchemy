// Differ 30 of 218 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08, second pass: gMapBlocks + index, the z term first (50), z * 32, a
// volatile backdrop store, the backdrop read before the resources, the resources
// assigned inside the store (40), *resources, the shifted block word kept in a
// local for the switch (82): none under 30. A 10-minute permute found nothing.
// 2026-11-08: a buffer variable for Data_0202d000 alone already swaps r5 and
// r6 (84); a palette pointer local, an s32 or u16 backdrop colour, once-only
// blocks around the stores, and other block-index and region spellings gain
// nothing (30 to 77).
// 30 on 2026-11-06; 38 on 2026-10-18. A buffer
// variable set one call early, reused for the next buffer: 84. A 5-minute
// permute found nothing under 44; the rest is allocation and scheduling.
// First pass: like The Broken
// Seal's WorldMap_LoadGraphics, with this game's four graphics sets chosen by
// the region (bits 24-29) of the block under (x, z) through Func_0802d088
// and region / 10. Left: the native loads each decode buffer into r5 one call
// earlier (Data_0202d000 before Func_080c8970), forms gMapBlocks in r2, and
// keeps the backdrop colour store before the palette DMA's operands. Naming
// the buffers as variables assigned there gives r5 to the resources and r6
// to the buffers (differ 92).
// 2026-11-06: one buffer variable set right after its previous use (before
// Func_080c8970, after StartChannels, after each Dma_Set) puts every buffer
// load where the native has it; what is left (84) is r5 and r6 swapped
// between the resources and the buffers. The --rtl dumps say why: the reused
// buffer variable dies in 4 places, so it goes to global-alloc, while the
// resources pointer is local to its block and local-alloc gives it r5 first;
// the native's r6 for it needs a higher-priority local in r5 across its life.
// Separate buffer variables are local but rank below it (3 refs over 22
// insns against 11 over 99). Also tried, no gain: the region for the
// division through a variable (69, 73), masked and shifted (36), divided by
// 10U (32); the palette address in the buffer variable (127).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "heap_release.h"
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
