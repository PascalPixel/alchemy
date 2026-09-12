#include "battle_effect_chance.h"
#include "types.h"
#include "runtime_interfaces.h"
#include "battle_random.h"

s32 BattleFx_GetBaseSuccessRate(s32 effect_id)
{
    s32 battle_result;
    u32 entry_index;

    entry_index = effect_id - 8;
    switch (entry_index) {
    case 4:
    case 5:
        return 0x46;
    case 8:
    case 9:
        return 0x4B;
    case 14:
        return 0x1E;
    case 15:
        return 0x28;
    case 16:
        return 0x2D;
    case 10:
    case 11:
    case 17:
        return 0x37;
    case 18:
        return 0x19;
    case 19:
        return 0x14;
    case 12:
    case 23:
        return 0x41;
    case 13:
    case 26:
        return 0x23;
    case 27:
        return 0x32;
    case 48:
        battle_result = 0x3C;
        goto block_18;
    case 49:
        battle_result = 0x5A;
        goto block_18;
    case 0:
    case 1:
    case 20:
    case 24:
        return 0x3C;
    default:
        battle_result = 0x64;
        break;
    }
block_18:
    return 0 - battle_result;
}

s32 Func_08079ad8(s32 arg0);

s32 BattleTarget_IsWeakToEffect(const u8 *state, s32 effect_id)
{
    u8 *entries;
    const u8 *field;
    s32 entry_index;
    s32 offset = 0x129;
    s32 battle_value;

    field = state + offset;
    if (*field == 0) {
        offset--;
        field = state + offset;
        entries = (u8 *)Owner_GetRecord(*field) + 0x48;
        entry_index = 0;
first_loop:
        if (*entries != effect_id) {
            entry_index++;
            entries++;
            if (entry_index > 2) {
                goto not_found;
            }
            goto first_loop;
        }
        goto found;
    }

    offset = 0x129;
    field = state + offset;
    entries = Func_08079ad8(*field) + 0x50;
    entry_index = 0;
second_loop:
    battle_value = *entries++;
    if (battle_value == effect_id) {
found:
        return 1;
    }
    entry_index++;
    if (entry_index > 2) {
not_found:
        return 0;
    }
    goto second_loop;
}

s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);
u8 *Owner_GetState(s32);
s32 Owner_GetResistanceValue(s32, s32);
s32 BattleRandomPercent(void);

s32 BattleFx_RollSuccess(
    s32 caster,
    s32 target,
    s32 resistance_category,
    s32 effect_id,
    s32 success_scale) {
    u8 *state = Owner_GetState(target);
    s32 attempts = 1;
    s32 score;
    s32 attempt;
    s8 *flag131;
    u8 *flag138;
    u8 *flag139;
    u8 *flag_13a;
    u8 *flag_13b;
    u8 *flag_13c;

    if (BattleEffect_IsRevive(effect_id)!= 0 &&
        FIELD_AT_OFFSET(state, s16 *, 0x38) != 0) {
        return 0;
    }

    if (effect_id == 3 && FIELD_AT_OFFSET(state, s8 *, 0x131) == 0) {
        goto fail;
    }

    goto action4_check;
action4_tail:
    if (state[0x13B] == 0 && state[0x13C] == 0 &&
        state[0x13D] == 0 && state[0x141] == 0) {
        goto fail;
    }
    goto action4_done;
action4_check:
    if (effect_id == 4) {
        if (state[0x138] == 0 && state[0x139] == 0 && state[0x13A] == 0) {
            goto action4_tail;
        }
    }

action4_done:
    flag131 = (s8 *)(state + 0x131);
    flag138 = state + 0x138;
    flag139 = state + 0x139;
    flag_13a = state + 0x13A;
    flag_13b = state + 0x13B;
    flag_13c = state + 0x13C;
    if (effect_id == 0x40 &&
        *flag131 == 0 &&
        *flag138 == 0 &&
        *flag139 == 0 &&
        *flag_13a == 0 &&
        *flag_13b == 0 &&
        *flag_13c == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x13D) == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x141) == 0 &&
        FIELD_AT_OFFSET(state, u8 *, 0x140) == 0) {
        return 0;
    }

    if (effect_id == 0x1C && FIELD_AT_OFFSET(state, u8 *, 0x141) == 1) {
        return 0;
    }

    score = BattleEffect_GetBaseSuccessRate(effect_id);
    if (score > 0) {
        s32 difference = Owner_GetResistanceValue(caster, resistance_category) -
            Owner_GetResistanceValue(target, resistance_category) -
            (FIELD_AT_OFFSET(state, u8 *, 0x42) >> 1);
        score += difference * 3;
        if (BattleTarget_IsWeakToEffect(state, effect_id) != 0) {
            score += 25;
        }
    } else {
        score = -score;
    }

    if (effect_id == 0x43) {
        attempts = 3;
    }

    for (attempt = 0; attempt < attempts; attempt++) {
        if (FixedPoint_Ratio(score *success_scale, 100) >= BattleRandomPercent()) {
            return 1;
        }
    }
fail:
    return 0;
}
