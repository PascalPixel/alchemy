#include "DMA.H"

/* WAITCNT and SIOCNT values, link-time symbols loaded from the pool. */
extern u8 Value_00004014;
extern u8 Value_0000c00f;

extern s32 Data_02002090;
extern u8 Data_03001ac4;
extern u8 Data_03001f54;
extern u8 Data_03001f58;
extern s32 Data_03007800;
extern u8 Data_03001d18;
extern u8 Data_03001ca0;

#define Resource_LoadWorkHeader Func_08002f10
#define WaitFrames Func_080030f8
#define Resource_InitializeTable Func_0800403c
#define Scheduler_ResetTaskTable Func_080040e8
#define Bg0_ClearTilemap Func_08004760
#define Runtime_InitializeHeap Func_08004858
#define System_Initialize Func_08002e00

void Resource_LoadWorkHeader(void);
void Func_0800300c(void);
void Func_0800307c(s32, s32, void (*)(void));
void WaitFrames(s32 frames);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void Bg0_ClearTilemap(void);
void Func_0800479c(void);
void Runtime_InitializeHeap(void);
void Func_0808a008(s32);
void Func_080f9008(void);

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
    do { u32 value = (u32)&Value_00004014; *(u16 *)0x04000204 = value; } while (0);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x03000000, 0x85001e00, (volatile u32 *)0x040000d4);
    Runtime_InitializeHeap();
    Func_0800300c();
    Data_02002090 = 0;
    Data_03001ac4 = 0;
    Data_03001f54 = 0;
    Data_03001f58 = 0;
    Resource_LoadWorkHeader();
    Func_0800479c();
    Bg0_ClearTilemap();
    *(u16 *)0x04000000 = 0x140;
    Func_0800307c(0, 1, (void (*)(void))0x08003651);
    do { u32 value = (u32)&Value_0000c00f; *(u16 *)0x04000132 = value; } while (0);
    Func_080f9008();
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    Data_03007800 = 0;
    Data_03001d18 = 1;
    Data_03001ca0 = 0;
    WaitFrames(10);
    Func_0808a008(0);
}
