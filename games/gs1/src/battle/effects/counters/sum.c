#include "battle_effect_counters.h"

s32 BattleEffect_SumCounters(void)
{
    struct BattleEffectCounterState *state = Data_03001ebc;

    return state->counters[0] + state->counters[1] + state->counters[2]
         + state->counters[3] + state->counters[4] + state->counters[5]
         + state->counters[6] + state->counters[7] + state->counters[6]
         + state->counters[8] + state->counters[9] + state->counters[10]
         + state->counters[11];
}
