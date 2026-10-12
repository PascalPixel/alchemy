#ifndef ALCHEMY_UNIT_BATTLE_BATTLE6_H
#define ALCHEMY_UNIT_BATTLE_BATTLE6_H

#include "types.h"

struct BattleActionRecord;
struct BattleEventState;
struct BtlAct;
struct BtlPlan;

void BattleMotion_RunValueSequence(s32 id, s32 unused0, s32 unused1);
void BattleActor_RemoveFromLists(s32 id);
void BattleMotion_InitializeActorRecords(s32 id);
s32 BattleTarget_SelectRandomPosition(s32 require_living_unit);
s32 BattleTarget_SelectForAction(s32 actor_id, struct BtlAct *action);
s32 BattleActor_ResetRuntimeFields(s32 id);
s32 BattlePresentation_WaitForAdvance(void);
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
void BattleEv_MarkDisplay(void);
s32 BattleActor_DestroyTemporaryObject(s32 arg0);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
u32 BattleEv_DispatchQueued(void);
u32 BtlEvPush(u32 opcode, u32 operand);
s32 BtlEfxDeadChk(s32 efx);
s32 BtlTgtExec(struct BtlPlan *plan, s32 slot);
u32 Ability_CheckStatusOrSpecialId(s32 act_no);
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry);
void Dma_StopAllThenRunHook(void);
void BattleEventRuntime_BeginPhase(s32 cue);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEvent_Playback(void);
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_WaitForReady(void);
u32 BattleParty_IsUnitListed(u32 id);
s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts);
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req, struct BtlPlan *plan);

#endif
