#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "iowrite.h"
#include "dma.h"
#include "mem.h"
#include "irq.h"
#include "task.h"
#include "io_write_queue.h"
#include "oam.h"
#include "frame.h"

extern u8 gData_03001110[];
void Func_080132d0(void);
void Window_LoadGraphics(void);
void Func_08014b70(void);
void VramCache_Reset(void);
void Func_0801399c(void);
void Sound_InitializeFar(void);
void ParentSequence_RunFar(s32);
void WaitFrames(s32 frames);
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
  Window_LoadGraphics();
  Func_08014b70();
  REG_DISPCNT = 0x140;
  Runtime_SetIrqHandler(0, 1, Func_0801399c);
  Runtime_SetIrqHandler(13, 1, Iwram_Halt);
  Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
  Sound_InitializeFar();
  VramCache_Reset();
  Task_ResetAll();
  gCartridgeResetMarker = 0;
  gRenderOamEnabled = 1;
  gOamUsage.limit = 0x100;
  gOamUsage.used = 0;
  gSleepDisabled = 0;
  WaitFrames(10);
  ParentSequence_RunFar(0);
}
