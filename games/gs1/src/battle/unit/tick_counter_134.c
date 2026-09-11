#include "scene.h"
#include "abi/battle/unit/tick_counter_134.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter134(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_134 != 0) {
        state->counter_134--;
        if (state->counter_134 == 0) {
            state->state_135 = 0;
            return 1;
        }
        if (state->state_135 < 0 &&
            Battle_Run(value, state->counter_134, 20) != 0) {
            state->state_135 = 0;
            state->counter_134 = 0;
            return 1;
        }
    }
    return 0;
}
