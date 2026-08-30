#include "battle_effect_counters.h"

void BattleEffect_ResetCounters(void)
{
  short cleared_value;
  struct BattleEffectCounterState *state;
  cleared_value = 0;
  state = Data_03001ebc;
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
