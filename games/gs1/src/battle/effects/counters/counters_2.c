#include "scene.h"
#include "battle_effect_counters.h"

/* battle/effects/counters/reset.c */
void BattleFx_ResetCounters(void)
{
  short cleared_value;
  struct BattleEffectCounterState *state;
  cleared_value = 0;
  state = gWork;
  state->counters[0] = cleared_value;
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

/* battle/effects/counters/sum.c */
s32 BattleFx_SumCounters(void)
{
    struct BattleEffectCounterState *state = gWork;

    return state->counters[0] + state->counters[1] + state->counters[2]
         + state->counters[3] + state->counters[4] + state->counters[5]
         + state->counters[6] + state->counters[7] + state->counters[6]
         + state->counters[8] + state->counters[9] + state->counters[10]
         + state->counters[11];
}
