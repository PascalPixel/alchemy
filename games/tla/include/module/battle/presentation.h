#ifndef ALCHEMY_MODULE_BATTLE_PRESENTATION_H
#define ALCHEMY_MODULE_BATTLE_PRESENTATION_H

/// @file
/// What games/tla/src/battle/presentation/f11e3ac/ declares for the files that
/// use it, written once.

#include "types.h"

struct BattleActionRecord;
struct BattlePlan;

u32 BattleParty_IsUnitListed(u32 id);

s32 Func_08127cd4(s32 recordId);

void Func_081263c4(void);
void Func_081263fc(s32 mode, s32 resource, s32 level);

void Func_08125d74(s32 arg, s32 kind);

void Func_08124cc0(void);

void Func_081263f0(void);

s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);

s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan);

s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode);

s32 Func_08124cc4(s32 side);

void Func_0812628c(s32 mode, s32 background, s32 value);

s32 Func_08122514(struct BattleActionRecord *request, s32 retry);

u32 BattleClass_GetAttackEffect(u32 index);
u32 BattleClass_GetDefeatSoundKind(s32 index);

s32 Func_0811f4d4(s32 party);

s32 BattleParty_CountStandbyDjinn(u32 arg0, u8 *counts);

#endif
