#include "counter_runtime.h"

s32 Func_080bf484(s32 value)
{
    struct CounterRuntime *state = Func_08077008();
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
