#include "counter_runtime.h"

s32 Func_080771a0(void);

s32 Func_080bf208(s32 object_id, s32 count, s32 bias)
{
    struct CounterRuntime *state = Func_08077008();

    if (count <= 5) {
        s32 threshold = ((state->value_42 * 3 - count * 5) + bias) * 0x28f;
        if (threshold >= (Func_080771a0() & 0xffff))
            return 1;
    }
    return 0;
}
