#include "RUNTIME_MEM.H"
#include "DISPTRAN.H"
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "IO_REG.H"

/* The battle-effect display transition state: the per-scanline window
   tables fill the block and the control fields follow them. */



/* Allocates and clears the transition state for a battle effect, arms the
   window mask and installs the two per-frame callbacks that drive it. */
void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
    struct DisplayTransitionState *state = Runtime_AllocateBlock(31, sizeof(*state));
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    state->mode = mode;
    state->value = 0;
    state->mask = 0x3f3f;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks(&DisplayTransition_UpdateScanlineTable);
    Scheduler_EnableCallbacks(&BattleFx_StartWindowHBlankDma);
}

struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void)
{
    volatile u32 clear_value;
    struct DisplayTransitionState *state;

    state = Runtime_AllocateBlock(31, sizeof(*state));
    clear_value = 0;
    Dma_Set(&clear_value, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    return state;
}
