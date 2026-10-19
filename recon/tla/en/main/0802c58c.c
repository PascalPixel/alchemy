// Near miss, differ 45 of 277 (2026-10-18), first pass: The Broken Seal's
// Map_UpdateCurrentTileBlockUntilBlocked, then this game's staged reload of
// the world map graphics when the region under the followed point changes
// (Func_0802d088), one step a frame counted down from 12 at map work 0x958.
// Left: as in 0802be4c, the native forms gMapBlocks in r1 and reads the
// block's region for the division through r3; two stack reloads and the
// reload test's constants sit a step earlier.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "mem.h"
#include "resource.h"
#include "graphics.h"

struct WorldBlock {
  u32 tile : 24;
  u32 region : 6;
  u32 unk : 2;
};

struct MapTileWindow {
  s32 *origin;
  u8 unk004[0x11c];
  const u32 *resources;
  u8 unk124[0x24];
  u16 tileIds[32 * 32];
  u8 unk948[0x10];
  s32 reloadSteps;
};

extern struct WorldBlock gMapBlocks[];
extern const u32 gData_0802ed34[4][9];
extern u8 Data_0202d000[];
extern u8 gData_02038000[];
extern u8 gData_0203a000[];
extern u8 gData_0203c000[];
extern u8 gData_0203e000[];
extern u8 gData_02028000[];
extern u8 gMapCollision[];
extern u8 gWorldMapWork[];

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
void Func_0802d088(s32 region, s32 gradual);
s32 Func_080c8950(void);
void Func_080c8970(void *palette);
void MapAnimation_StartChannels(const u16 *script);

void Func_0802c58c(void) {
  struct MapTileWindow *window =
      (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 y = 0;
  s32 x0;
  s32 y0;
  s32 layer;
  s32 row;
  s32 col;
  s32 tile;
  struct WorldBlock *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 busy;
  u8 *buffer;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    x = *p++;
    y = p[1];
  }
  x0 = (x - 0x1000000) >> 25;
  y0 = (y - 0x1400000) >> 25;
  for (layer = 0; layer <= 1; layer++) {
    for (row = 0; row <= 1; row++) {
      for (col = 0; col <= 1; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = window->tileIds[tile];
        tile += layer << 10;
        if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
          return;
      }
    }
  }
  block = &gMapBlocks[((x / 0x200000) & 31) + (((y / 0x200000) & 31) << 5)];
  Func_0802d088(block->region, 1);
  switch (block->region / 10) {
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
  if (resources != window->resources) {
    window->resources = resources;
    window->reloadSteps = 12;
  }
  busy = Func_080c8950();
  if (window->reloadSteps == 0)
    return;
  if (busy != 0)
    return;
  buffer = MemAlloc(0x200);
  switch (window->reloadSteps) {
  case 12: {
    s16 backdropColor = *(s16 *)0x05000000;

    Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = backdropColor;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
    Func_080c8970(buffer);
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), Data_0202d000);
    MapAnimation_StartChannels((const u16 *)Data_0202d000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), gMapCollision);
    break;
  }
  case 11:
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), gData_02038000);
    break;
  case 10:
    Dma_Set(gData_02038000, (void *)0x06008000, 0x84000800, REG_DMA3);
    break;
  case 9:
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), gData_0203a000);
    break;
  case 8:
    Dma_Set(gData_0203a000, (void *)0x0600a000, 0x84000800, REG_DMA3);
    break;
  case 7:
    Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gData_0203c000);
    break;
  case 6:
    Dma_Set(gData_0203c000, (void *)0x0600c000, 0x84000800, REG_DMA3);
    break;
  case 5:
    Resource_DecodeType01(Resource_GetTableEntry(resources[6]), gData_0203e000);
    break;
  case 4:
    Dma_Set(gData_0203e000, (void *)0x0600e000, 0x84000800, REG_DMA3);
    break;
  case 3:
    Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
    break;
  case 1:
    Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
    break;
  }
  window->reloadSteps--;
  MemFree(buffer);
}
