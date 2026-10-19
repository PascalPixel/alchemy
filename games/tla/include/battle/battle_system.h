#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

#include "battle_event.h"
#include "battle_type.h"
#include "edition.h"

void BattleUnit_Recalculate(s32 unitId);
struct BattleAct *BattleAction_GetDirect(s32 actionId);
struct BattleUnit *UnitGetFar(s32 unitId);
struct BattleAct *BattleActGet(s32 actionId);
s32 GameFlag_TestFar(s32 flag_id);
void UnitRatioFar(s32 unitId);
s32 BattleRandomFar(void);
s32 Battle_IsReviveEffectFar(s32 effect_id);
u32 Ability_IsPsynergy(s32 actionId);
s32 Battle_ClassifyEntryKind(const struct BattleAct *entry);

#endif
