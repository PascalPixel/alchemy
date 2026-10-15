#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

#include "battle_event.h"
#include "battle_type.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

void BattleUnit_Recalculate(s32 unit_id);
#if GAME_TLA
struct BattleAct *BattleAction_GetDirect(s32 action_id);
#endif
#if GAME_TLA
struct BattleUnit *UnitGetFar(s32 unit_id);
struct BattleAct  *BattleActGet(s32 action_id);
s32				GameFlag_TestFar(s32 flag_id);
void			UnitRatioFar(s32 unit_id);
s32				BattleRandomFar(void);
s32				BattleEffectRevCheckFar(s32 effect_id);
#endif
#if GAME_TLA
u32 Ability_CheckStatusOrSpecialId(s32 action_id);
s32 Battle_ClassifyEntryKind(const struct BattleAct *entry);
#endif

#endif
