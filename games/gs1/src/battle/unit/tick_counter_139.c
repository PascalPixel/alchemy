#include "counter_runtime.h"

#define BattleUnit_TickCounter139 Func_080bf3bc

s32 BattleUnit_TickCounter139(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_139 != 0) {
        state->counter_139--;
        if (state->counter_139 == 0)
            return 1;
        if (Func_080bf208(value, state->counter_139, 60) != 0) {
            state->counter_139 = 0;
            return 1;
        }
    }
    return 0;
}
