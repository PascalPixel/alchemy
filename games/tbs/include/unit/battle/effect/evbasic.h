#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EVBASIC_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EVBASIC_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/objtable.h"

struct ScenePlacement;

void Battle_UpdateModeFromShoulderButtons(void);
void Battle_WaitMode0(s32 frames);
void Battle_InitializeRenderObject(void);
void Battle_Reset(void);
void BattleFx_FinishAction(void);
void Event_NoOpHookA(void);
void Event_NoOpHook(void);
void Event_RunObjectHookAndWait(struct ScenePlacement *table);
void Event_CallWithLastActiveObjectId(struct ScenePlacement *table);
void Event_SetSceneEvents(s32 value);
void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value);
void Event_PrepareTwoObjectsAndApply(s32 first, s32 second);
s32 Event_ValidatePackedId(u32 packed_id);
void Event_ClearInvalidPackedValues(void);
void Party_RemoveOwnerRestored(s32 owner);
s32 Party_CheckMemberValueTotal(s32 id);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner);
void Inventory_NoOpCallback(void);
void Inventory_ReservedNoOp(void);
void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom);
s32 Object_CallSpawnRoutineAtOrigin(s32 value);
void UiText_DrawQuantityPairWithCue(s32 unit, s32 abil);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_SetDestination(s32 first, s32 second);
void Event_SetSavedDestination(u16 first, u16 second);
void Event_SetExitRequest(u16 value);
void BattleFx_SetWeightedResult(s32 zone_no, s32 slot_no);
void BattleFx_SetPhaseRequest(s32 flag, s32 no);
void Party_SetFields1ceAnd1d0(s32 first, s32 second);
void Event_SetDefeatDestination(s32 first, s32 second);
void Event_RequestDestination(u16 first, u16 second);
void BattleFx_SetQueuedSoundAndPlay(s32 sound_id);
void BattleFx_PlayQueuedSound(void);

#endif
