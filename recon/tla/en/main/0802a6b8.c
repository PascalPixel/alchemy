// Draft, differ 46 of 406 (2026-10-26, 58 before): the layer's cell pointer set
// after its x position. Earlier notes follow.
// Near miss, differ 74 of 406, instruction order and register choice: the
// twin of The Broken Seal's Map_LoadLayeredScene. Loads layered scene index
// from gData_0802f380: its blocks, collision (filled with -1 first), cells,
// shape grid, tile animations, blend script and the script at 0x0202e000;
// sets up the three layers from the scene header; loads the palette and
// tiles unless flag 0x170 says they are loaded, and starts Func_0802ad84.
// Left: the native loads the table's address before it scales the index (an
// empty asm passing both through, as Func_0802a650 has, fixes it but moves
// the index's copy, differ 74 again), builds the REG_DISPCNT mask after its
// load, computes each origin address one step earlier, and in the layer loop
// issues the cell buffer's pool load and the scaleX byte before the maskY
// shift and keeps the view pointer and MulQ16's address in r2 and r3 the
// other way round. Measured: maskY is (periodY & 127) << 7; Io_Write16 for
// the three BGxCNT writes (direct stores, differ 172) and a word local for
// the final MOSAIC, BLDCNT and DISPCNT stores (direct, differ 93), as ☀️'s;
// Io_Write16 or a word local for the first DISPCNT write (differ 104 to
// 106); the layer's loads in the order their stores constrain (differ 176);
// pointers to maxX and maxY (no change). Permute 6 minutes found only a cast
// that changes MulQ16's address.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "iowrite.h"

struct LayeredSceneEntry {
  u16 resources[6];
};

struct LayeredSceneLayer {
  u8 x;
  u8 y;
  s8 scaleX;
  s8 scaleY;
  s8 speedX;
  s8 speedY;
  u8 periodX;
  u8 periodY;
};

struct LayeredSceneHeader {
  u8 origin[4];
  u8 priority[3];
  u8 charBase[3];
  u16 unk0a;
  struct LayeredSceneLayer layers[3];
  s32 tiles;
  s32 collision;
  s32 tilemap;
  s32 shapes;
  s32 animation;
  s32 blend;
  s32 script;
};

struct MapCell {
  u32 value;
};

struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 cellX, cellY;
  struct MapCell *cells;
  u8 *shapes;
};

struct MapScrollWork {
  u8 unk000[0x14];
  u16 flags;
  u8 unk016[0xe4 - 0x16];
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  u8 unk0fc[5];
  u8 unk101;
  u8 unk102[2];
  u8 enabled[3];
  u8 unk107;
  struct MapLayerScroll layers[3];
};

extern struct LayeredSceneEntry gData_0802f380[];
extern char ResourceId_Map000;
extern u8 gMapShapeGrid[];
extern u8 gData_02028000[];

void Fade_FromBlack(s32);
void Tilemap_DecodeStagedBuffer(s32 size);
void Tilemap_ConvertBuffer(void);
void Func_0802cc9c(const u16 *);
void DisplayBlend_StartScript(u16 *);
s32 Resource_DecodeType2(const void *source, void *destination);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Task_Add(void (*func)(void), s32 order);
void Func_0802ad84(void);

s32 Func_0802a6b8(s32 index) {
  struct LayeredSceneEntry *sceneResources;
  struct MapScrollWork *work;
  struct LayeredSceneHeader *sceneHeader;
  struct MapLayerScroll *mapLayer;
  struct LayeredSceneLayer *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  sceneResources = &gData_0802f380[index];
  work = HeapSlot_AllocEwram(32, sizeof(struct MapScrollWork));
  Iwram_ClearWords(work, sizeof(struct MapScrollWork));
  sceneHeader = (struct LayeredSceneHeader *)Resource_GetTableEntry(
      sceneResources->resources[0] + (u32)&ResourceId_Map000);
  encodedData = (u8 *)sceneHeader + sceneHeader->tiles;
  Tilemap_DecodeStagedBuffer(
      Resource_DecodeType01(encodedData, Ram_MapCellBuffer + 1));
  Dma_Fill32(-1, Ram_MapCollision, 0x2000);
  encodedData = (u8 *)sceneHeader + sceneHeader->collision;
  Resource_DecodeType01(encodedData, Ram_MapCollision);
  encodedData = (u8 *)sceneHeader + sceneHeader->tilemap;
  Resource_DecodeType01(encodedData, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  encodedData = (u8 *)sceneHeader + sceneHeader->shapes;
  Resource_DecodeType01(encodedData, gMapShapeGrid);
  if (sceneHeader->animation != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->animation;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1000);
    Func_0802cc9c((const u16 *)(Ram_MapCollision + 0x1000));
  }
  if (sceneHeader->blend != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->blend;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1e00);
    DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
  }
  encodedData = (u8 *)sceneHeader + sceneHeader->script;
  Resource_DecodeType01(encodedData, Ram_MapCollision + 0x2000);
  work->minX = sceneHeader->origin[0] << 19;
  work->minY = sceneHeader->origin[1] << 19;
  work->maxX = sceneHeader->origin[2] << 19;
  work->maxY = sceneHeader->origin[3] << 19;
  viewX = &work->viewX;
  *viewX = 0;
  viewY = &work->viewY;
  *viewY = 0;
  work->enabled[0] = sceneHeader->priority[0];
  work->enabled[1] = sceneHeader->priority[1];
  work->enabled[2] = sceneHeader->priority[2];
  work->unk101 = 2;
  if (work->maxX == 0)
    work->maxX = 0x8000000;
  if (work->maxY == 0)
    work->maxY = 0x8000000;
  mapLayer = work->layers;
  layerSource = sceneHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scaleX;
    s32 scaleY;
    s32 cell;

    mapLayer->offsetX = x << 19;
    mapLayer->offsetY = y << 19;
    scaleX = layerSource->scaleX * 4096;
    mapLayer->scaleY = scaleY = layerSource->scaleY * 4096;
    mapLayer->speedX = layerSource->speedX * 4096;
    mapLayer->speedY = layerSource->speedY * 4096;
    mapLayer->maskX = layerSource->periodX & 127;
    mapLayer->maskY = (layerSource->periodY & 127) << 7;
    mapLayer->phaseX = 0;
    mapLayer->phaseY = 0;
    x >>= 1;
    y = (y >> 1) * 128;
    mapLayer->cellY = y;
    cell = y + x;
    mapLayer->scaleX = scaleX;
    mapLayer->cellX = x;
    mapLayer->shapes = gMapShapeGrid + cell;
    mapLayer->cells = (struct MapCell *)Ram_MapCellBuffer + cell;
    mapLayer->x = Iwram_MulQ16(*viewX, scaleX) + mapLayer->offsetX;
    mapLayer->y = Iwram_MulQ16(*viewY, mapLayer->scaleY) + mapLayer->offsetY;
    layerSource++;
    mapLayer++;
  }
  work->flags = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->flags |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->flags |= DISPCNT_BG1_ON;
  Io_Write16(work->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5),
             &REG_BG3CNT);
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_Test(0x170) != 0) {
    GameFlag_ClearBit(0x170);
  } else {
    graphicsBuffer = Heap_Alloc(0x4000);
    if (graphicsBuffer != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Resource_DecodeType01(
          Resource_GetTableEntry(sceneResources->resources[1] +
                                 (u32)&ResourceId_Map000),
          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      Iwram_CopyWords((void *)BG_PLTT, graphicsBuffer, 0x1c0);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[2] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(1), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[3] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(2), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[4] +
                                                  (u32)&ResourceId_Map000),
                           graphicsBuffer);
      Iwram_CopyWords(BG_CHAR_BLOCK(3), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[5] +
                                                  (u32)&ResourceId_Map000),
                           gData_02028000);
      Heap_Release(graphicsBuffer);
    }
  }
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  REG_DISPCNT = displayFlags;
  Task_Add(Func_0802ad84, 0x485);
  return 2;
}
