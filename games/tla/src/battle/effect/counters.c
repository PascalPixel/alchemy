#include "types.h"
#include "ram_buffer.h"

struct BattleEffectCounterState {
    u8 unk000[0x154];
    s16 counters[12];
};

s32 Func_080caf6c(void)
{
    return 0;
}

void BattleFx_ResetCounters(void)
{
    s16 value;
    struct BattleEffectCounterState *state;

    value = 0;
    state = Ram_HeapSlots->eventWork;
    state->counters[0] = value;
    state->counters[1] = 0;
    state->counters[2] = 0;
    state->counters[3] = 0;
    state->counters[4] = 0;
    state->counters[5] = 0;
    state->counters[6] = 0;
    state->counters[7] = 0;
    state->counters[8] = 0;
    state->counters[9] = 0;
    state->counters[10] = 0;
    state->counters[11] = 0;
}

s32 BattleFx_SumCounters(void)
{
    struct BattleEffectCounterState *state = Ram_HeapSlots->eventWork;

    return state->counters[0] + state->counters[1] + state->counters[2]
        + state->counters[3] + state->counters[4] + state->counters[5]
        + state->counters[6] + state->counters[7] + state->counters[8]
        + state->counters[9] + state->counters[10] + state->counters[11];
}

s32 GameFlag_Test(s32 flag);
s32 Party_FindOwnerWithActionFar(s32 action);

void Func_080cb05c(void)
{
    u8 *work = Ram_HeapSlots->eventWork;

    if (GameFlag_Test(0x8df) && Party_FindOwnerWithActionFar(153) != -1)
        work[0xd39] = 1;
    else
        work[0xd39] = 0;
}
