#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "resource.h"
#include "system.h"
#include "dma.h"
#include "io_reg.h"
#include "callback_scheduler.h"

extern void (*gData_030011f8)(void);
extern char ResourceId_DefaultMapCells;
extern u8 gData_02038000[];
extern u8 gData_0203a000[];
extern u8 gData_0203c000[];
extern u8 gData_0203e000[];
extern u8 gData_02028000[];
extern u8 gWorldMapWork[];
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void MapAnimation_Update(void);
void Map_ShowBg1FromBuffer(void);
void Map_UpdateCurrentTileBlock(void);
void Func_0802d088(s32 area, s32 mode);

s32 Map_LoadAreaGraphics(void) {
  struct MapViewWork *state;
  u32 *resources;
  u8 *buffer;
  s16 value;

  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  buffer = Ram_MapCellBuffer;
  resources = state->resources;
  value = *(s16 *)0x05000000;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]), gData_02038000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]), gData_0203a000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gData_0203c000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]), gData_0203e000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
  gData_030011f8 = Map_ShowBg1FromBuffer;
  state->windowTop = 0;
  state->windowBottom = 159;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), buffer);
  Map_UpdateCurrentTileBlock();
  state->animationActive = 0;
  Scheduler_ResumeTasks(MapAnimation_Update);
  state->framePending = 0;
  state->frameShown = 0;
  Func_0802d088(state->area, 0);
  WaitFrames(1);
}
