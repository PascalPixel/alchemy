#include "types.h"
#include "battle_party.h"
#include "owner_state.h"
#include "scene.h"
#include "battle_unit.h"

s32 BattleRandomFar(void);

s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias)
{
    struct BattleUnit *state = UnitGet(unit_id);

    if (turns <= 5) {
        s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
        if (threshold >= (BattleRandomFar() & 0xffff))
            return 1;
    }
    return 0;
}

s32 BattleUnit_TickAttackModifier(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);

    if (state->atk_cnt != 0) {
        s32 zero;
        state->atk_cnt--;
        zero = 0;
        if ((s8)state->atk_cnt == 0) {
            state->atk_lv = zero;
            return 1;
        }
        if (state->atk_lv < 0) {
            if (BattleUnit_RollCounterEarlyEnd(unit_id, state->atk_cnt, 30) != 0) {
                state->atk_lv = zero;
                state->atk_cnt = zero;
                return 1;
            }
        }
    }
    return 0;
}

s32 BattleUnit_TickDefenseModifier(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);

    if (state->def_cnt != 0) {
        state->def_cnt--;
        if (state->def_cnt == 0) {
            state->def_lv = 0;
            return 1;
        }
        if (state->def_lv < 0 &&
            BattleUnit_RollCounterEarlyEnd(unit_id, state->def_cnt, 20) != 0) {
            state->def_lv = 0;
            state->def_cnt = 0;
            return 1;
        }
    }
    return 0;
}

s32 BattleUnit_TickResistanceModifier(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);

    if (state->res_cnt != 0) {
        state->res_cnt--;
        if (state->res_cnt == 0) {
            state->res_lv = 0;
            return 1;
        }
        if (state->res_lv < 0 &&
            BattleUnit_RollCounterEarlyEnd(unit_id, state->res_cnt, 20) != 0) {
            state->res_lv = 0;
            state->res_cnt = 0;
            return 1;
        }
    }
    return 0;
}

s32 BattleUnit_TickDelusion(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);
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

s32 BattleUnit_TickConfusion(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);
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

s32 BattleUnit_TickCharm(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);
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

s32 BattleUnit_TickStun(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);
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

s32 BattleUnit_TickSleep(s32 unit_id)
{
    struct BattleUnit *state = UnitGet(unit_id);
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

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 next;
    s32 expired;
    u8 remaining;
    s32 turns;
    s32 packed;
    struct BattleUnit *unit;

    unit = UnitGet(id);
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
        if (remaining == 0)
            return expired;
        if ((u32)remaining <= 7U &&
            BattleUnit_RollCounterEarlyEnd(id, unit->psy_seal, 0x1E) != 0) {
            unit->psy_seal = 0U;
            return 1;
        }
    }
    expired = 0;
    return expired;
}

