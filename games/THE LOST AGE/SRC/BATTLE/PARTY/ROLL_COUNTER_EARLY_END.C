#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "OWNER_STATE.H"
#include "SCENE.H"
#include "BATTLE_UNIT.H"

/* battle/unit/roll_counter_early_end.c */
s32 BattleRandom16Far(void);

s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias)
{
    struct BattleUnit *state = Owner_GetState(unit_id);

    if (turns <= 5) {
        s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
        if (threshold >= (BattleRandom16Far() & 0xffff))
            return 1;
    }
    return 0;
}

/* battle/unit/tick_counter_132.c */
s32 BattleUnit_TickCounter132(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);

    if (state->attack_modifier_turns != 0) {
        s32 zero;
        state->attack_modifier_turns--;
        zero = 0;
        if ((s8)state->attack_modifier_turns == 0) {
            state->attack_modifier = zero;
            return 1;
        }
        if (state->attack_modifier < 0) {
            if (BattleUnit_RollCounterEarlyEnd(unit_id, state->attack_modifier_turns, 30) != 0) {
                state->attack_modifier = zero;
                state->attack_modifier_turns = zero;
                return 1;
            }
        }
    }
    return 0;
}

/* battle/unit/tick_counter_134.c */
s32 BattleUnit_TickCounter134(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);

    if (state->defense_modifier_turns != 0) {
        state->defense_modifier_turns--;
        if (state->defense_modifier_turns == 0) {
            state->defense_modifier = 0;
            return 1;
        }
        if (state->defense_modifier < 0 &&
            BattleUnit_RollCounterEarlyEnd(unit_id, state->defense_modifier_turns, 20) != 0) {
            state->defense_modifier = 0;
            state->defense_modifier_turns = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_136.c */
s32 BattleUnit_TickCounter136(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);

    if (state->res_modifier_turns != 0) {
        state->res_modifier_turns--;
        if (state->res_modifier_turns == 0) {
            state->res_modifier = 0;
            return 1;
        }
        if (state->res_modifier < 0 &&
            BattleUnit_RollCounterEarlyEnd(unit_id, state->res_modifier_turns, 20) != 0) {
            state->res_modifier = 0;
            state->res_modifier_turns = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_138.c */
s32 BattleUnit_TickCounter138(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);
    if (state->delusion != 0) {
        state->delusion--;
        if (state->delusion == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(unit_id, state->delusion, 30) != 0) {
            state->delusion = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_139.c */
s32 BattleUnit_TickCounter139(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);
    if ((u8)state->confusion != 0) {
        (*(u8 *)&state->confusion)--;
        if (state->confusion == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(unit_id, (u8)state->confusion, 60) != 0) {
            state->confusion = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13a.c */
s32 BattleUnit_TickCounter13a(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);
    if (state->charm != 0) {
        state->charm--;
        if (state->charm == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(unit_id, state->charm, 70) != 0) {
            state->charm = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13b.c */
s32 BattleUnit_TickCounter13b(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);
    if (state->stun != 0) {
        state->stun--;
        if (state->stun == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(unit_id, state->stun, 40) != 0) {
            state->stun = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13c.c */
s32 BattleUnit_TickCounter13c(s32 unit_id)
{
    struct BattleUnit *state = Owner_GetState(unit_id);
    if (state->sleep != 0) {
        state->sleep--;
        if (state->sleep == 0)
            return 1;
        if (BattleUnit_RollCounterEarlyEnd(unit_id, state->sleep, 50) != 0) {
            state->sleep = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/advance_counter_and_check_chance.c */

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 next;
    s32 expired;
    u8 remaining;
    s32 turns;
    s32 packed;
    struct BattleUnit *unit;

    unit = Owner_GetState(id);
    packed = unit->psy_seal;
    turns = packed & 0xFF;
    if (turns != 0) {
        if ((u32)turns > 7U) {
            turns += 0xF8;
            unit->psy_seal = turns;
            packed = turns;
        }
        if (packed & 7) {
            next = packed + 0xFF;
            unit->psy_seal = next;
            packed = next;
        }
        expired = 1;
        remaining = packed;
        if (remaining != 0) {
            if ((u32)remaining <= 7U &&
                BattleUnit_RollCounterEarlyEnd(id, unit->psy_seal, 0x1E) != 0) {
                unit->psy_seal = 0U;
                return 1;
            }
            goto block_9;
        }
        return expired;
    }
block_9:
    expired = 0;
    return expired;
}

