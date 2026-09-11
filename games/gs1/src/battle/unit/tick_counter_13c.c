#include "scene.h"
#include "abi/battle/unit/tick_counter_13c.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter13c(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13c != 0) {
        state->counter_13c--;
        if (state->counter_13c == 0)
            return 1;
        if (Battle_Run(value, state->counter_13c, 50) != 0) {
            state->counter_13c = 0;
            return 1;
        }
    }
    return 0;
}
