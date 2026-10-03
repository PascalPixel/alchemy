#include "DISPTRAN.H"
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"

/* The battle-effect display transition state: the per-scanline window
   tables fill the block and the control fields follow them. */

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
    state->value = 0;
    state->mask = 0x3f3f;
    state->active = 1;
    Scheduler_AddOrUpdateCallback((s32)DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)&DisplayTransition_UpdateScanlineTable);
    Scheduler_EnableCallbacks((u32)&BattleFx_StartWindowHBlankDma);
}

void *DisplayTransition_AllocateAndClearState(void)
{
    volatile u32 clear_value;
    void *destination;

    destination = Runtime_AllocateBlock(31, 0x540);
    clear_value = 0;
    Dma_Set(&clear_value, destination, 0x85000150, (volatile u32 *)0x040000d4);
    return destination;
}
