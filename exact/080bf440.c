#include "counter_runtime.h"

s32 Func_080bf440(s32 value)
{
    struct CounterRuntime *state = Func_08077008();
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
