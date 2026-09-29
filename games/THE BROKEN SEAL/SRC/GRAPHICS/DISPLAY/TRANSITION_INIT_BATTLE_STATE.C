#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"

/* The battle-effect display transition state: the per-scanline window
   tables fill the block and the control fields follow them. */
struct DisplayTransitionState {
    u8 lines[0x528];
    s16 mode;
    s16 timer;
    u8 unknown_52c[8];
    s16 mask;
    s16 active;
};

void *Runtime_AllocateBlock(s32 kind, s32 size);
void DisplayTransition_UpdateScanlineTable(void);
void BattleFx_StartWindowHBlankDma(void);

/* Allocates and clears the transition state for a battle effect, arms the
   window mask and installs the two per-frame callbacks that drive it. */
void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
    struct DisplayTransitionState *state = Runtime_AllocateBlock(31, 0x540);
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000150, (volatile u32 *)0x040000d4);
    state->mode = mode;
    state->timer = 0;
    state->mask = 0x3f3f;
    state->active = 1;
    Scheduler_AddOrUpdateCallback((s32)DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_StartWindowHBlankDma, 0x480);
}
