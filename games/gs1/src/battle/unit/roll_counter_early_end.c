#include "types.h"
#include "counter_runtime.h"

/* battle/unit/roll_counter_early_end.c */
s32 FunctionHead_080771a0(void);
struct CounterRuntime *Runtime_GetObject(void);
s32 Battle_Run(s32 value, s32 count, s32 bias);

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

/* battle/unit/tick_counter_132.c */
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

/* battle/unit/tick_counter_134.c */
s32 BattleUnit_TickCounter134(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_134 != 0) {
        state->counter_134--;
        if (state->counter_134 == 0) {
            state->state_135 = 0;
            return 1;
        }
        if (state->state_135 < 0 &&
            Battle_Run(value, state->counter_134, 20) != 0) {
            state->state_135 = 0;
            state->counter_134 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_136.c */
s32 BattleUnit_TickCounter136(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_136 != 0) {
        state->counter_136--;
        if (state->counter_136 == 0) {
            state->state_137 = 0;
            return 1;
        }
        if (state->state_137 < 0 &&
            Battle_Run(value, state->counter_136, 20) != 0) {
            state->state_137 = 0;
            state->counter_136 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_138.c */
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

/* battle/unit/tick_counter_139.c */
s32 BattleUnit_TickCounter139(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_139 != 0) {
        state->counter_139--;
        if (state->counter_139 == 0)
            return 1;
        if (Battle_Run(value, state->counter_139, 60) != 0) {
            state->counter_139 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13a.c */
s32 BattleUnit_TickCounter13a(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13a != 0) {
        state->counter_13a--;
        if (state->counter_13a == 0)
            return 1;
        if (Battle_Run(value, state->counter_13a, 70) != 0) {
            state->counter_13a = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13b.c */
s32 BattleUnit_TickCounter13b(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13b != 0) {
        state->counter_13b--;
        if (state->counter_13b == 0)
            return 1;
        if (Battle_Run(value, state->counter_13b, 40) != 0) {
            state->counter_13b = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13c.c */
s32 BattleUnit_TickCounter13c(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13c != 0) {
        state->counter_13c--;
        if (state->counter_13c == 0)
            return 1;
        if (Battle_Run(value, state->counter_13c, 50) != 0) {
            state->counter_13c = 0;
            return 1;
        }
    }
    return 0;
}
