#ifndef ALCHEMY_UNIT_BATTLE_TURNEND_H
#define ALCHEMY_UNIT_BATTLE_TURNEND_H

#include "types.h"

struct BtlPlan;

s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias);
s32 BattleUnit_TickAttackModifier(s32 unit_id);
s32 BattleUnit_TickConfusion(s32 unit_id);
s32 Battle_AdvanceCounterAndCheckChance(s32 id);
s32 BattleUnit_TickRefrain(s32 unit_id);
s32 BattleUnit_TickReflect(s32 id);
s32 BattleUnit_TickAgilityModifier(s32 id);
s32 BattlePlacement_UpdateTimedEntries(void);
s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
void Battle_ReservedNoOpF674(void);
s32 Battle_ProcessRoundEnd(void);
s32 BattleUnit_ProcessTurnEnd(struct BtlPlan *plan);

#endif
