#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"


extern u8 gDebugMode;
extern u8 gCpuLoadDisplayEnabled;
extern s32 gCartridgeResetMarker;
extern u8 gOamCopyEnabled;
extern u8 gSleepDisabled;

void WaitFrames(s32 frames);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define HALT_ON_GAMEPAK_IRQ 1
#endif

void System_Initialize(void)
{
    volatile u32 zero;

    {
        volatile u16 *dma0 = REG_DMA0;

        dma0[5] &= 0xc5ff;
        dma0[5] &= 0x7fff;
        dma0[5];
    }
    /* FAKEMATCH: the do-while wraps pin the register writes in place. */
    do { u32 value = WAITCNT_GAME; REG_WAITCNT = value; } while (0);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x03000000, 0x85001e00, REG_DMA3);
    Runtime_InitializeHeap();
    Runtime_InstallIwramAndIrqs();
    *(u32 *)&gIoWriteQueue.count = 0;
    gDebugTextEnabled = 0;
    gDebugMode = 0;
    gCpuLoadDisplayEnabled = 0;
    Resource_LoadWorkHeader();
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    REG_DISPCNT = 0x140;
    Runtime_SetIrqHandler(0, 1, (void (*)(void))System_VBlankHandler);
#if defined(HALT_ON_GAMEPAK_IRQ)
    Runtime_SetIrqHandler(13, 1, IwramHalt);
#endif
    do { u32 value = KEYCNT_SOFT_RESET; REG_KEYCNT = value; } while (0);
    Audio_InitializeRuntimeDefaultsFar();
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    gCartridgeResetMarker = 0;
    gOamCopyEnabled = 1;
    gSleepDisabled = 0;
    WaitFrames(10);
    Game_ResetForNewGameFar(0);
}
