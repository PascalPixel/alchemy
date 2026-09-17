#include "TYPES.H"

struct BattleEffectCounterState {
    u8 unk_000[0x154];
    s16 counters[12];
};

struct SystemWork {
    u8 unk_00[0x6c];
    struct BattleEffectCounterState *battle_effect_counters;
};

#define IWRAM_BASE ((struct SystemWork *)0x03000000)

void BattleFx_ResetCounters(void)
{
    s16 value;
    struct BattleEffectCounterState *state;

    value = 0;
    state = IWRAM_BASE->battle_effect_counters;
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
