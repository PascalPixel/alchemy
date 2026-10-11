#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

extern u8  HitTbl[];
extern s32 EpLossTbl[];
extern s32 HpRcvTbl[];
extern s32 EpDmgTbl[];
extern s32 HpDmgTbl5[];
extern s32 HpDmgTbl8[];
extern s32 HpDmgTbl6[];
extern s32 EpRcvTbl[];
extern s32 HpDmgTbl[];

#if GAME_TLA
s32 BtlHitChk(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 BtlAtkCalc(s32 amount, s32 reduction, s32 adjustment, s32 selector);
s32 BtlPowCalc(s32 value, s32 selector, s32 multiplier);
s32 BtlRcvCalc(s32 value, s32 selector, s32 multiplier);
s16 UnitEpAddFar(s32 unit, s32 amount);
s32 BtlUnitSetFar(s32 slot, s32 unit, s32 mask);
#endif
#if GAME_TLA
s32 BtlEfxDeadChk(s32 effect);
#endif

#endif
