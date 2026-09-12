#include "types.h"
#include "counter_runtime.h"

/* battle/unit/roll_counter_early_end.c */
s32 FunctionHead_080771a0(void);
struct CounterRuntime *Runtime_GetObject(void);

s32 BattleUnit_RollCounterEarlyEnd(s32 object_id, s32 count, s32 bias)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (count <= 5) {
        s32 threshold = ((state->value_42 * 3 - count * 5) + bias) * 0x28f;
        if (threshold >= (FunctionHead_080771a0() & 0xffff))
            return 1;
    }
    return 0;
}
