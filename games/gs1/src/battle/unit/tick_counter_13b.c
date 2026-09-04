#include "counter_runtime.h"

#define BattleUnit_TickCounter13b Func_080bf440

s32 BattleUnit_TickCounter13b(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13b != 0) {
        state->counter_13b--;
        if (state->counter_13b == 0)
            return 1;
        if (Func_080bf208(value, state->counter_13b, 40) != 0) {
            state->counter_13b = 0;
            return 1;
        }
    }
    return 0;
}
