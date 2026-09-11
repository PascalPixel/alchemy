#include "scene.h"
#include "abi/battle/unit/tick_counter_132.h"
#include "counter_runtime.h"

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
            if (Battle_Run(value, state->counter_132, 30) != 0) {
                state->state_133 = zero;
                state->counter_132 = zero;
                return 1;
            }
        }
    }
    return 0;
}
