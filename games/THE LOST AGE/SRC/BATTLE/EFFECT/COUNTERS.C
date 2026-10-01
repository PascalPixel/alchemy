#include "TYPES.H"
#include "RAM_BUFFER.H"

struct BattleEffectCounterState {
    u8 unk_000[0x154];
    s16 counters[12];
};

void BattleFx_ResetCounters(void)
{
    s16 value;
    struct BattleEffectCounterState *state;

    value = 0;
    state = Ram_HeapSlots->event_work;
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
