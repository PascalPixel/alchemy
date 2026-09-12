#include "types.h"
#include "scene.h"
#include "counter_runtime.h"

/* battle/unit/roll_counter_early_end.c */
s32 FunctionHead_080771a0(void);
struct CounterRuntime *Runtime_GetObject();
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

/* battle/advance_counter_and_check_chance.c */
s32 Battle_Place(s32 id, s32 arg1, s32 arg2);

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 t2;
    s32 ret;
    u8 t3;
    s32 t;
    s32 cnt;
    void *obj;

    obj = Runtime_GetObject(id);
    cnt = FIELD_AT_OFFSET(obj, u8 *, 0x13D);
    t = cnt & 0xFF;
    if (t != 0) {
        if ((u32)t > 7U) {
            t += 0xF8;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t;
            cnt = t;
        }
        if (cnt & 7) {
            t2 = cnt + 0xFF;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t2;
            cnt = t2;
        }
        ret = 1;
        t3 = cnt;
        if (t3 != 0) {
            if ((u32)t3 <= 7U &&
                Battle_Place(id, FIELD_AT_OFFSET(obj, u8 *, 0x13D), 0x1E) != 0) {
                FIELD_AT_OFFSET(obj, u8 *, 0x13D) = 0U;
                return 1;
            }
            goto block_9;
        }
        return ret;
    }
block_9:
    ret = 0;
    return ret;
}
