#ifndef ALCHEMY_BATTLE_EFFECT_CHANCE_H
#define ALCHEMY_BATTLE_EFFECT_CHANCE_H

#include "types.h"

#define BattleEffect_GetBaseSuccessRate Func_08079d7c
#define BattleTarget_IsWeakToEffect      Func_08079e9c
#define BattleEffect_IsRevive            Func_08079ef8
#define BattleEffect_RollSuccess         Func_08079f10

s32 BattleEffect_GetBaseSuccessRate(s32 effect_id);
s32 BattleTarget_IsWeakToEffect(const u8 *target_state, s32 effect_id);
s32 BattleEffect_IsRevive(s32 effect_id);
s32 BattleEffect_RollSuccess(
    s32 caster,
    s32 target,
    s32 resistance_category,
    s32 effect_id,
    s32 success_scale);

#endif
