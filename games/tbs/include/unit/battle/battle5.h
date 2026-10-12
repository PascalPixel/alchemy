#ifndef ALCHEMY_UNIT_BATTLE_BATTLE5_H
#define ALCHEMY_UNIT_BATTLE_BATTLE5_H

#include "types.h"

struct BattleActionRecord;
struct BattlePresentationWork;
struct BtlPlan;
struct MotionObject;

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay);
s32 BattlePres_BuildTargetList(struct BtlPlan *plan, struct BattlePresentationWork *work);
s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BtlPlan *plan);
s32 BattlePresentation_RunUnitTransition(struct BtlPlan *plan, s32 mode);
s32 BattlePres_RunActorEntries(struct BtlPlan *plan, s32 unused);
s32 RunBattlePresentation(struct BtlPlan *plan, s32 mode);
s32 BattlePres_RunSimple(struct BtlPlan *input, s32 flags);
s32 BattlePres_RunItem(struct BtlPlan *plan, s32 unused, struct BattleActionRecord *selection);
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value);
s32 BattlePres_RunPsynergyCast(struct BtlPlan *plan, s32 mode);

#endif
