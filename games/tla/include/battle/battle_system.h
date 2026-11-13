#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

#include "battle_event.h"
#include "battle_type.h"
#include "edition.h"

struct BattleAct *Ability_GetDirect(s32 abilityId);
struct BattleUnit *Unit_GetFar(s32 unitId);
struct BattleAct *Ability_Get(s32 abilityId);
s32 GameFlag_TestFar(s32 flag_id);
void Unit_RecalculateGaugesFar(s32 unitId);
s32 Battle_RandomFar(void);
s32 Battle_IsReviveEffectFar(s32 effect_id);
u32 Ability_IsPsynergy(s32 actionId);
s32 Battle_ClassifyEntryKind(const struct BattleAct *entry);

#endif
