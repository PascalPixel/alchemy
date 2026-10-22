// Draft, differ 8 of 88: The Broken Seal's System_Initialize with ⚓️'s
// changes: the gamepak interrupt halts (Iwram_Halt), the OAM limit is set to
// 256 with no sprites used, and gDebugTextEnabled is gData_03001110. All
// eight rows are four one-slot load swaps: the native schedules each load
// (the DMA0 control read, its reread, the WAITCNT and KEYCNT addresses) one
// slot before sched2 ranks it here, the same one-slot load order as this
// lane's other near misses.
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
void WaitFrames(s32 frames);
void Iwram_Halt(void);

void System_Initialize(void) {
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
  WaitFrames(10);
  Game_RunFar(0);
}
