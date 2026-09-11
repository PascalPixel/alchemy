#include "scene.h"
#include "abi/battle/unit/tick_counter_136.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter136(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_136 != 0) {
        state->counter_136--;
        if (state->counter_136 == 0) {
            state->state_137 = 0;
            return 1;
        }
        if (state->state_137 < 0 &&
            Battle_Run(value, state->counter_136, 20) != 0) {
            state->state_137 = 0;
            state->counter_136 = 0;
            return 1;
        }
    }
    return 0;
}
