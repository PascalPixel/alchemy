#ifndef ALCHEMY_UNIT_BATTLE_BATTLE3_H
#define ALCHEMY_UNIT_BATTLE_BATTLE3_H

#include "types.h"

struct BattleActionRecord;

s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionRecord *entries);
s32 BattlePres_RunAction(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 object_id);
s32 BattleEscape_PlayRun(struct BattleActionRecord *action);
s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action);
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input);
s32 BattlePres_RunUnitAction(struct BattleActionRecord *action);
void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context);
void BattleMotion_SetMode5AndActivateSlot(s32 unit_id);
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action);

#endif
