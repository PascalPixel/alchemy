#include "counter_runtime.h"

#define BattleUnit_TickCounter13a Func_080bf400

s32 BattleUnit_TickCounter13a(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13a != 0) {
        state->counter_13a--;
        if (state->counter_13a == 0)
            return 1;
        if (Func_080bf208(value, state->counter_13a, 70) != 0) {
            state->counter_13a = 0;
            return 1;
        }
    }
    return 0;
}
