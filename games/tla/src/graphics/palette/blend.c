#include "types.h"
#include "mem.h"
#include "display_transition.h"
#include "dma.h"
#include "system.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "io_reg.h"

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;

    state = Runtime_AllocateBlock(0x7c, sizeof(*state));
    Dma_Fill32(0, state, sizeof(*state));
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->mode = value;
    state->value = 0;
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0x480);
    WaitFrames(0x78);
}

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (frames > 0) {
        divide = Iwram_SignedDivide;
        index = 0x53F;
        do {
            first = *source;
            second = *target;
            *step = divide(second - first, frames);
            index--;
            source++;
            target++;
            step++;
        } while (index >= 0);
    }
}
