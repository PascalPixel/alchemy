#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

extern u8  HitTable[];
extern s32 PpLossTable[];
extern s32 HpRecoveryTable[];
extern s32 PpDmgTable[];
extern s32 HpDmgTable5[];
extern s32 HpDmgTable8[];
extern s32 HpDmgTable6[];
extern s32 PpRecoveryTable[];
extern s32 HpDmgTable[];

#if GAME_TLA
s32 BattleHitCheck(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 BattleAtkCalc(s32 amount, s32 reduction, s32 adjustment, s32 selector);
s32 BattlePowerCalc(s32 value, s32 selector, s32 multiplier);
s32 BattleRecoveryCalc(s32 value, s32 selector, s32 multiplier);
s16 UnitPpAddFar(s32 unit, s32 amount);
s32 BattleUnitSetFar(s32 slot, s32 unit, s32 mask);
#endif
#if GAME_TLA
s32 BattleEffectDeadCheck(s32 effect);
#endif

#endif
