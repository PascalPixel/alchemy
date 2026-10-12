#ifndef ALCHEMY_UNIT_GAME_BTLCALC_H
#define ALCHEMY_UNIT_GAME_BTLCALC_H

#include "types.h"

struct BtlUnit;

s32 Curve_LookupScaledValue(s32 input, s32 halve);
s32 BtlRnd(void);
u32 BattleRandomPercent(void);
s32 Owner_ScaleAdjustedAmount(s32 atk, s32 def, s32 add, s32 bonus);
s32 Owner_ScaleValueByCurve(s32 pow, s32 bonus, s32 rate);
s32 Owner_ScaleValueByOffsetCurve(s32 pow, s32 elm, s32 rate);
s32 Item_GetEquippedElement(s32 id);
s32 Equipment_GetUnleashRateBonus(struct BtlUnit *owner);
s32 RollWeaponUnleash(struct BtlUnit *owner);
s32 BattleFx_GetBaseSuccessRate(s32 effect_id);
s32 BattleTarget_IsWeakToEffect(const struct BtlUnit *target, s32 effect_id);
s32 BtlEfxRevChk(s32 effect_id);
s32 BattleFx_RollSuccess(s32 caster, s32 target, s32 resistance_category, s32 effect_id, s32 success_scale);

#endif
