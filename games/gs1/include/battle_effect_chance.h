#ifndef ALCHEMY_BATTLE_EFFECT_CHANCE_H
#define ALCHEMY_BATTLE_EFFECT_CHANCE_H

#include "types.h"

s32 BattleFx_GetBaseSuccessRate(s32 effect_id);
s32 BattleTarget_IsWeakToEffect(const u8 *target_state, s32 effect_id);
s32 BattleFx_IsRevive(s32 effect_id);
s32 BattleFx_RollSuccess(
    s32 caster,
    s32 target,
    s32 resistance_category,
    s32 effect_id,
    s32 success_scale);

#endif
