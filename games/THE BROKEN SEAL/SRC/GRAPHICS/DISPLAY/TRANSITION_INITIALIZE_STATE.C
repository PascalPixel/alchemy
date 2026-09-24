#include "DMA.H"
#include "SYSTEM.H"

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
};

void DisplayTransition_FillTilemapAndSolidTile(s32);
void Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void DisplayTransition_UpdateFrame(void);

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
