#include "DMA.H"
#include "SYSTEM.H"

extern u8 Data_03001ecc[];

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
};

void DisplayTransition_FillTilemapAndSolidTile(s32);
void Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void DisplayTransition_UpdateFrame(void);

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    u8 *work = *(u8 **)Data_03001ecc;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = (u32 *)(work + 1288);
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work + 1288, (void *)0x06000000, 0x84000008, (volatile u32 *)0x040000d4);
    }
}

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x1f, 0x540);
    zero = 0;
    Dma_Set(&zero, state, 0x85000150, (volatile u32 *)0x040000d4);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->value = value;
    state->timer = 0;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    WaitFrames(0x78);
}
