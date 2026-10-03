#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "CALLBACK_SCHEDULER.H"
#include "IRQ.H"

void System_VBlankHandler(void);

extern u8 Data_03001ac4;
extern u8 gDebugMode;
extern u8 Data_03001f58;
extern s32 Data_03007800;
extern u8 gOamCopyEnabled;
extern u8 Data_03001ca0;


void Resource_LoadWorkHeader(void);
void WaitFrames(s32 frames);
void Resource_InitializeTable(void);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
void Game_ResetForNewGameFar(s32);
void Audio_InitializeRuntimeDefaultsFar(void);

/* The European editions halt on a Game Pak interrupt, when the cartridge
   is pulled, through the halt loop start-up copied into the runtime. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define HALT_ON_GAMEPAK_IRQ 1
void IwramHalt(void);
#endif

/* Stops DMA0, sets the wait states, clears IWRAM below the stack and
   brings up the heap, resources, display, interrupts and task table. */
void System_Initialize(void)
{
    volatile u32 zero;

    {
        volatile u16 *dma0 = (volatile u16 *)0x040000b0;

        dma0[5] &= 0xc5ff;
        dma0[5] &= 0x7fff;
        dma0[5];
    }
    /* FAKEMATCH: the do-while wraps pin the register writes in place. */
    do { u32 value = WAITCNT_GAME; REG_WAITCNT = value; } while (0);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x03000000, 0x85001e00, (volatile u32 *)0x040000d4);
    Runtime_InitializeHeap();
    Runtime_InstallIwramAndIrqs();
    /* Startup clears the count and its adjacent padding in one word. */
    *(u32 *)&gIoWriteQueue.count = 0;
    Data_03001ac4 = 0;
    gDebugMode = 0;
    Data_03001f58 = 0;
    Resource_LoadWorkHeader();
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    *(u16 *)0x04000000 = 0x140;
    Runtime_SetIrqHandler(0, 1, (void (*)(void))System_VBlankHandler);
#if defined(HALT_ON_GAMEPAK_IRQ)
    Runtime_SetIrqHandler(13, 1, IwramHalt);
#endif
    do { u32 value = KEYCNT_SOFT_RESET; REG_KEYCNT = value; } while (0);
    Audio_InitializeRuntimeDefaultsFar();
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    Data_03007800 = 0;
    gOamCopyEnabled = 1;
    Data_03001ca0 = 0;
    WaitFrames(10);
    Game_ResetForNewGameFar(0);
}
