#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "DISPTRAN.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"

/* ☀️'s GRAPHICS/PALETTE/PALETTE5.C after its tilemap fill, which is still
   disassembly here, as is the battle palette blend that follows. */

/* Allocate and clear the transition's state in heap slot 31, clear the
   screen, and start the per-frame update. */
void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x7c, sizeof(*state));
    zero = 0;
    Dma_Set(&zero, state, 0x85000000 | (sizeof(*state) / 4), (volatile u32 *)0x040000d4);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->mode = value;
    state->value = 0;
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0x480);
    WaitFrames(0x78);
}

/* Fill the step table: each of the 0x540 colour components' distance to its
   target, divided by the number of frames. */
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
