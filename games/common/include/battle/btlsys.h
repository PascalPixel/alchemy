#ifndef ALCHEMY_BATTLE_RUNTIME_H
#define ALCHEMY_BATTLE_RUNTIME_H

#include "btlev.h"
#include "btltype.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

void		   BattleUnit_Recalculate(s32 unit_id);
#if GAME_TLA
struct BtlAct *BattleAction_GetDirect(s32 action_id);
#endif
#if GAME_TLA
struct BtlUnit *UnitGetFar(s32 unit_id);
struct BtlAct  *BtlActGet(s32 action_id);
s32				GameFlag_TestFar(s32 flag_id);
void			UnitRatioFar(s32 unit_id);
s32				BtlRndFar(void);
s32				BtlEfxRevChkFar(s32 effect_id);
#endif
#if GAME_TLA
u32 Ability_CheckStatusOrSpecialId(s32 action_id);
s32 Battle_ClassifyEntryKind(const struct BtlAct *entry);
#endif

#endif
