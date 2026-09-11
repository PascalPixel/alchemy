#include "scene.h"
#include "abi/battle/unit/tick_counter_138.h"
#include "counter_runtime.h"

s32 BattleUnit_TickCounter138(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_138 != 0) {
        state->counter_138--;
        if (state->counter_138 == 0)
            return 1;
        if (Battle_Run(value, state->counter_138, 30) != 0) {
            state->counter_138 = 0;
            return 1;
        }
    }
    return 0;
}
