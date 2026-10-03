#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_UNIT.H"

/* battle/unit/roll_counter_early_end.c */
s32 BattleRandom16Far(void);
struct BattleUnit *Owner_GetStateFar();
s32 BattleUnit_RollCounterEarlyEnd(s32 value, s32 count, s32 bias);

s32 BattleUnit_RollCounterEarlyEnd(s32 object_id, s32 count, s32 bias)
{
    struct BattleUnit *state = Owner_GetStateFar();

    if (count <= 5) {
        s32 threshold = ((state->luck * 3 - count * 5) + bias) * 0x28f;
        if (threshold >= (BattleRandom16Far() & 0xffff))
            return 1;
    }
    return 0;
}

/* battle/unit/tick_counter_132.c */
s32 BattleUnit_TickCounter132(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();

    if (state->attack_modifier_turns != 0) {
        s32 zero;
        state->attack_modifier_turns--;
        zero = 0;
        if ((s8)state->attack_modifier_turns == 0) {
            state->attack_modifier = zero;
            return 1;
        }
        if (state->attack_modifier < 0) {
            if (BattleUnit_RollCounterEarlyEnd(value, state->attack_modifier_turns, 30) != 0) {
                state->attack_modifier = zero;
                state->attack_modifier_turns = zero;
                return 1;
            }
        }
    }
    return 0;
}

/* battle/unit/tick_counter_134.c */
s32 BattleUnit_TickCounter134(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();

    if (state->defense_modifier_turns != 0) {
        state->defense_modifier_turns--;
        if (state->defense_modifier_turns == 0) {
            state->defense_modifier = 0;
            return 1;
        }
        if (state->defense_modifier < 0 &&
            BattleUnit_RollCounterEarlyEnd(value, state->defense_modifier_turns, 20) != 0) {
            state->defense_modifier = 0;
            state->defense_modifier_turns = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_136.c */
s32 BattleUnit_TickCounter136(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();

    if (state->res_modifier_turns != 0) {
        state->res_modifier_turns--;
        if (state->res_modifier_turns == 0) {
            state->res_modifier = 0;
            return 1;
        }
        if (state->res_modifier < 0 &&
            BattleUnit_RollCounterEarlyEnd(value, state->res_modifier_turns, 20) != 0) {
            state->res_modifier = 0;
            state->res_modifier_turns = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_138.c */
s32 BattleUnit_TickCounter138(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();
    if (state->delusion != 0) {
        state->delusion--;
        if (state->delusion == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(value, state->delusion, 30) != 0) {
            state->delusion = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_139.c */
s32 BattleUnit_TickCounter139(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();
    if ((u8)state->confusion != 0) {
        (*(u8 *)&state->confusion)--;
        if (state->confusion == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(value, (u8)state->confusion, 60) != 0) {
            state->confusion = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13a.c */
s32 BattleUnit_TickCounter13a(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();
    if (state->charm != 0) {
        state->charm--;
        if (state->charm == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(value, state->charm, 70) != 0) {
            state->charm = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13b.c */
s32 BattleUnit_TickCounter13b(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();
    if (state->stun != 0) {
        state->stun--;
        if (state->stun == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(value, state->stun, 40) != 0) {
            state->stun = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13c.c */
s32 BattleUnit_TickCounter13c(s32 value)
{
    struct BattleUnit *state = Owner_GetStateFar();
    if (state->sleep != 0) {
        state->sleep--;
        if (state->sleep == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(value, state->sleep, 50) != 0) {
            state->sleep = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/advance_counter_and_check_chance.c */
s32 BattleUnit_RollCounterEarlyEnd(s32 id, s32 arg1, s32 arg2);

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 t2;
    s32 ret;
    u8 t3;
    s32 t;
    s32 cnt;
    struct BattleUnit *obj;

    obj = Owner_GetStateFar(id);
    cnt = obj->psy_seal;
    t = cnt & 0xFF;
    if (t != 0) {
        if ((u32)t > 7U) {
            t += 0xF8;
            obj->psy_seal = t;
            cnt = t;
        }
        if (cnt & 7) {
            t2 = cnt + 0xFF;
            obj->psy_seal = t2;
            cnt = t2;
        }
        ret = 1;
        t3 = cnt;
        if (t3 != 0) {
            if ((u32)t3 <= 7U &&
                BattleUnit_RollCounterEarlyEnd(id, obj->psy_seal, 0x1E) != 0) {
                obj->psy_seal = 0U;
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

/* battle/unit/tick_counter_13e.c */
s32 BattleUnit_TickCounter13e(void)
{
    u8 *value = &Owner_GetStateFar()->refrain;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}
