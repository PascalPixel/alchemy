#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "resource.h"
#include "system.h"
#include "frame.h"
#include "callback_scheduler.h"

void MapAnimation_PresentPicture(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80b;
  REG_BG3CNT = control;
  control = 0xaa0f;
  REG_BG2CNT = control;
  control = 0x682;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH: ⚓️ addresses the DMA channel before loading the transfer
   * operands */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(Ram_MapCellBuffer, (void *)0x06006000, 0x84002800, dma);
}

extern void (*gData_030011f8)(void);
extern u8 gData_02038000[];
extern u8 gData_0203a000[];
extern u8 gData_0201c000[];
void MapAnimation_Update(void);
void MapAnimation_PresentPicture(void);
void Func_0802dd08(void *source, void *destination);

void MapAnimation_Start(void) {
  u8 *pages = Ram_HeapSlots->unk1c[0];
  struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  u32 *resources;
  s32 one;
  s8 *active;

  active = &work->animationActive;
  one = 1;
  *active = one;
  Scheduler_SuspendTasks(MapAnimation_Update);
  work->picturePending = one;
  resources = work->resources;
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]), gData_02038000);
  if (work->extraTiles != 0)
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), gData_0203a000);
  work->extraTiles = 0;
  Dma_Set((void *)0x06004000, gData_0201c000, 0x84000800, REG_DMA3);
  WaitFrames(1);
  Func_0802dd08(pages + (gFrameCount & one) * 0x1400 + 0xc80,
                Ram_MapCellBuffer);
  work->windowTop = 200;
  work->windowBottom = 255;
  gData_030011f8 = MapAnimation_PresentPicture;
  work->pictureShown = one;
}

extern u8 gData_02038000[];

void Map_ShowBg1FromBuffer(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80a;
  REG_BG3CNT = control;
  control = 0xaa0e;
  REG_BG2CNT = control;
  control = 0x501;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH: ⚓️ addresses the DMA channel before loading the transfer
   * operands */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(gData_02038000, (void *)0x06008000, 0x84002000, dma);
}
