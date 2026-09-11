#include "types.h"
#include "scene.h"
#include "object_lookup.h"
#include "battle_effect_counters.h"

/* object/table/clear_battle_slots.c */
/* object/table/clear_battle_slots.c */
extern u8 *gWork;

void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 offset;
    s32 zero;
    s32 count;
    offset = 0x8C;
    zero = 0;
    count = 0x41;
    current = (s32 *)(gWork + offset * 2);
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

/* object/table/get.c */
extern u8 *gWork;

void *ObjectTable_Get(u32 arg0)
{
    u8 *base = gWork;
    u32 offset;
    if (arg0 > 0xbf)
        return 0;
    offset = (arg0 * 4) + 0x14;
    return *(void **)(base + offset);
}

/* battle/effects/counters/fx_reset_counters.c */
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
