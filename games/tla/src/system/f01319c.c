#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "iowrite.h"
#include "dma.h"
#include "mem.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "oam.h"
#include "frame.h"

extern u8 gData_03001110[];
void Func_080132d0(void);
void Ui_LoadWindowGraphics(void);
void Func_08014b70(void);
void VramCache_Reset(void);
void Func_0801399c(void);
void Audio_InitializeRuntimeDefaultsFar(void);
void Game_RunFar(s32);
void WaitTicks(s32 ticks);
void Iwram_Halt(void);

void Func_0801319c(void) {
  {
    volatile u16 *dma0 = REG_DMA0;

    dma0[5] &= 0xc5ff;
    dma0[5] &= 0x7fff;
    dma0[5];
  }
  Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
  Dma_Fill32(0, (void *)0x03000000, 0x7800);
  Runtime_InitializeHeap();
  Runtime_InstallIwramAndIrqs();
  *(u32 *)&gIoWriteQueue.count = 0;
  gData_03001110[0] = 0;
  gDebugMode = 0;
  gCpuLoadDisplayEnabled = 0;
  Func_080132d0();
  Ui_LoadWindowGraphics();
  Func_08014b70();
  REG_DISPCNT = 0x140;
  Runtime_SetIrqHandler(0, 1, Func_0801399c);
  Runtime_SetIrqHandler(13, 1, Iwram_Halt);
  Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
  Audio_InitializeRuntimeDefaultsFar();
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  gCartridgeResetMarker = 0;
  gRenderOamEnabled = 1;
  gOamUsage.limit = 0x100;
  gOamUsage.used = 0;
  gSleepDisabled = 0;
  WaitTicks(10);
  Game_RunFar(0);
}
