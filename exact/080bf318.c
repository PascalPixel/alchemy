#include "counter_runtime.h"

s32 Func_080bf318(s32 value)
{
    struct CounterRuntime *state = Func_08077008();

    if (state->counter_136 != 0) {
        state->counter_136--;
        if (state->counter_136 == 0) {
            state->state_137 = 0;
            return 1;
        }
        if (state->state_137 < 0 &&
            Func_080bf208(value, state->counter_136, 20) != 0) {
            state->state_137 = 0;
            state->counter_136 = 0;
            return 1;
        }
    }
    return 0;
}
