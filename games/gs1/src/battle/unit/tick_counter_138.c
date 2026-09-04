#include "counter_runtime.h"

#define BattleUnit_TickCounter138 Func_080bf37c

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
