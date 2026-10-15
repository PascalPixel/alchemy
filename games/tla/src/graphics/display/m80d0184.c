#include "types.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

void Func_080cf78c(void);
void Func_080cf6fc(void);

/* The Spanish and Italian far-call tables reach this function under another slot's name (Func_080d6b90); those two editions keep it on their scaffold (S4). */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
    struct DisplayTransitionState *state = Runtime_AllocateBlock(124, sizeof(*state));
    volatile u32                   zero;

    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    state->mode = mode;
    state->value = 0;
    state->mask = 0x3f3f;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Scheduler_AddOrUpdateCallback(Func_080cf78c, 0x480);
    Scheduler_AddOrUpdateCallback(Func_080cf6fc, 0xc76);
}
#endif

void Func_080d0184(void)
{
    Scheduler_EnableCallbacks(Func_080cf78c);
    Scheduler_EnableCallbacks(Func_080cf6fc);
}

void *Func_080d019c(void)
{
    void        *block = Runtime_AllocateBlock(124, 0x540);
    volatile u32 zero = 0;

    Dma_Set(&zero, block, 0x85000150, (volatile u32 *)0x040000d4);
    return block;
}
