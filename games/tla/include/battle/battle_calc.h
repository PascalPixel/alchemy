#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "types.h"
#include "edition.h"

extern u8 gHitTable[];
extern s32 gPpLossTable[];
extern s32 gHpRecoveryTable[];
extern s32 gPpDmgTable[];
extern s32 gHpDmgTable5[];
extern s32 gHpDmgTable8[];
extern s32 gHpDmgTable6[];
extern s32 gPpRecoveryTable[];
extern s32 gHpDmgTable[];

s32 BattleEffect_RollSuccessFar(s32 actor, s32 target, s32 range, s32 effect,
                                s32 table);
s32 Battle_CalculateAttackDamageFar(s32 amount, s32 reduction, s32 adjustment,
                                    s32 selector);
s32 Battle_CalculatePsynergyPowerFar(s32 value, s32 selector, s32 multiplier);
s32 Battle_CalculateRecoveryFar(s32 value, s32 selector, s32 multiplier);
s16 Unit_AddPpFar(s32 unit, s32 amount);
s32 BattleUnit_SetFar(s32 slot, s32 unit, s32 mask);
s32 BattleEffect_CheckDead(s32 effect);

#endif
