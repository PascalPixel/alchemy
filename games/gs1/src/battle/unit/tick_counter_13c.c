#include "counter_runtime.h"

s32 BattleUnit_TickCounter13c(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13c != 0) {
        state->counter_13c--;
        if (state->counter_13c == 0)
            return 1;
        if (Func_080bf208(value, state->counter_13c, 50) != 0) {
            state->counter_13c = 0;
            return 1;
        }
    }
    return 0;
}
