#include "types.h"
#include "battle_random.h"
#include "battle_effect_chance.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);
u8 *Owner_GetState(s32);
s32 Owner_GetResistanceValue(s32, s32);
s32 BattleRandomPercent(void);

s32 BattleEffect_RollSuccess(
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
    u8 *flag13A;
    u8 *flag13B;
    u8 *flag13C;

    if (BattleEffect_IsRevive(effect_id) != 0 &&
        M2C_FIELD(state, s16 *, 0x38) != 0) {
        return 0;
    }

    if (effect_id == 3 && M2C_FIELD(state, s8 *, 0x131) == 0) {
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
    flag13A = state + 0x13A;
    flag13B = state + 0x13B;
    flag13C = state + 0x13C;
    if (effect_id == 0x40 &&
        *flag131 == 0 &&
        *flag138 == 0 &&
        *flag139 == 0 &&
        *flag13A == 0 &&
        *flag13B == 0 &&
        *flag13C == 0 &&
        M2C_FIELD(state, u8 *, 0x13D) == 0 &&
        M2C_FIELD(state, u8 *, 0x141) == 0 &&
        M2C_FIELD(state, u8 *, 0x140) == 0) {
        return 0;
    }

    if (effect_id == 0x1C && M2C_FIELD(state, u8 *, 0x141) == 1) {
        return 0;
    }

    score = BattleEffect_GetBaseSuccessRate(effect_id);
    if (score > 0) {
        s32 difference = Owner_GetResistanceValue(caster, resistance_category) -
            Owner_GetResistanceValue(target, resistance_category) -
            (M2C_FIELD(state, u8 *, 0x42) >> 1);
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
        if (FixedPoint_Ratio(score * success_scale, 100) >= BattleRandomPercent()) {
            return 1;
        }
    }
fail:
    return 0;
}
