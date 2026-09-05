#include "counter_runtime.h"

#define BattleUnit_TickCounter132 Func_080bf250

s32 BattleUnit_TickCounter132(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_132 != 0) {
        s32 zero;
        state->counter_132--;
        zero = 0;
        if ((s8)state->counter_132 == 0) {
            state->state_133 = zero;
            return 1;
        }
        if (state->state_133 < 0) {
            if (Func_080bf208(value, state->counter_132, 30) != 0) {
                state->state_133 = zero;
                state->counter_132 = zero;
                return 1;
            }
        }
    }
    return 0;
}
