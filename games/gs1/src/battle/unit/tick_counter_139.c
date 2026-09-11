#include "scene.h"
#include "abi/battle/unit/tick_counter_139.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter139(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_139 != 0) {
        state->counter_139--;
        if (state->counter_139 == 0)
            return 1;
        if (Battle_Run(value, state->counter_139, 60) != 0) {
            state->counter_139 = 0;
            return 1;
        }
    }
    return 0;
}
