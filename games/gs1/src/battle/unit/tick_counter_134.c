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
            Func_080bf208(value, state->counter_134, 20) != 0) {
            state->state_135 = 0;
            state->counter_134 = 0;
            return 1;
        }
    }
    return 0;
}
