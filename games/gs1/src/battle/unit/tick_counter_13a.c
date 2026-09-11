#include "scene.h"
#include "abi/battle/unit/tick_counter_13a.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter13a(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13a != 0) {
        state->counter_13a--;
        if (state->counter_13a == 0)
            return 1;
        if (Battle_Run(value, state->counter_13a, 70) != 0) {
            state->counter_13a = 0;
            return 1;
        }
    }
    return 0;
}
