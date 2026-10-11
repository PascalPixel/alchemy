#ifndef ALCHEMY_UNIT_FIELD_WORLD_MAP_MAP_H
#define ALCHEMY_UNIT_FIELD_WORLD_MAP_MAP_H

#include "types.h"

struct FieldActor;
struct SceneEvent;
struct ScenePlacement;
struct StoryVerticalEffectActor;

void StoryProgress_TriggerEvent0808(void);
void StoryProgress_TriggerEvent0809(void);
void StoryProgress_TriggerEvent080A(void);
void StoryActor_AdvanceTimer(struct FieldActor *actor);
void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor);
void WorldMap_UpdateBobbingMarker(struct FieldActor *actor);
s32 StoryActor_Initialize(struct FieldActor *actor);
u8 *WorldMap_GetEntrances(void);
s32 WorldMap_GetRegions(void);
u8 *WorldMap_GetExits(void);
s32 StoryActor_ApplyFlaggedMode(struct FieldActor *actor);
s32 StoryActor_ApplyMapRotation(struct FieldActor *actor);
s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor);
s32 StoryActor_ResetPosition(struct FieldActor *actor);
s32 StoryActor_ClearActiveFlag(struct FieldActor *actor);
struct ScenePlacement *StoryScene_SelectPlacementTable(void);
void StoryScene_SetBranchValueFromX(s32 trigger, s32 exit_before, s32 exit_after);
void StoryScene_SetBranchValueFromZ(s32 trigger, s32 exit_before, s32 exit_after);
void SceneState_SetValues130_6_47(void);
void SceneState_ApplyValues150And46And11(void);
void SceneState_ApplyValues116And56And21(void);
void SceneState_ApplyValues151And25And54(void);
void FieldScene_RunStep7D3B1E(void);
struct SceneEvent *WorldMap_GetEvents(void);
void MapActor_UpdateContact(void);
void SceneState_ApplyFlag85aBranch(void);
void FieldScene_RunStep74(void);
s32 WorldMap_EnterScene(void);
void RunEventScript01(void);
void FieldScene_RunActorTransferSequence(void);
void WorldMap_ShakeTransferStartCamera(void);
void WorldMap_ShakeTransferLandingCamera(void);
void WorldMap_UpdateTransferredActorLanding(void);
void WorldMap_TransferToExit102(void);
void WorldMap_TransferToExit103(void);
void WorldMap_TransferToExit104(void);
void WorldMap_TransferToFuneHeya(void);
void WorldMap_TransferToExit106(void);
void WorldMap_CompleteTransfer(void);
void WorldMap_ShowDjinnTutorial(void);
void WorldMap_MeetVenusDjinni(void);
void WorldMap_DisembarkCarriage(void);
void WorldMap_RunBlackOrbScene(void);
void WorldMap_ActivateSite138(s32 actor);
void WorldMap_RestoreExitTrigger(void);
void WorldMap_PromptBlackOrbUse(void);
void WorldMap_StopLeaderLeavingShip(void);
void StoryScene_ShowRewardDialogue(void);
void WorldMap_UseBlackOrb(void);
void FieldScene_RunActorPresentationSequence(void);
void StoryScene_StartTransition(void);
void StoryScene_UpdateTransitionEffect(void);
void FieldScene_RunActorEightApproach(void);
s32 StoryScene_ComputeOpposingSlotDelta(void);
s32 StoryReward_LookupBySelection(u32 selection);
void WorldMap_SpawnActorEightPuff(void);
void WorldMap_RunOpeningRise(void);
void StoryScene_UpdateSelectedActorProgress(void);
void FieldScene_RunOpeningAuxiliarySequence(s32 a0);
void StoryScene_SetReferenceActor(void);
void StoryScene_ActivateSharedState(void);
void StoryScene_CompleteActor98(void);
void WorldMap_RaiseActors(void);
void SceneEffect_RestoreBlendRegisters(void);
void FieldScene_RunLateSequence(void);
void Scene_RunScene371SequenceA(s32 palette);
void SceneActor_UpdateObjectByCounterBits(u8 *obj);
void OverlayObject_UpdateOnFrameParity(u8 *obj);
void SceneState_RunSlotStepOnOddFrames(s32 arg0);
void Effect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect);
void Effect_AnimateVerticalNegative(struct StoryVerticalEffectActor *effect);

#endif
