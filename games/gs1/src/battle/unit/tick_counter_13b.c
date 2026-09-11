#include "scene.h"
#include "abi/battle/unit/tick_counter_13b.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter13b(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13b != 0) {
        state->counter_13b--;
        if (state->counter_13b == 0)
            return 1;
        if (Battle_Run(value, state->counter_13b, 40) != 0) {
            state->counter_13b = 0;
            return 1;
        }
    }
    return 0;
}
