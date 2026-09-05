#include "counter_runtime.h"

s32 BattleUnit_TickCounter138(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_138 != 0) {
        state->counter_138--;
        if (state->counter_138 == 0)
            return 1;
        if (Func_080bf208(value, state->counter_138, 30) != 0) {
            state->counter_138 = 0;
            return 1;
        }
    }
    return 0;
}
