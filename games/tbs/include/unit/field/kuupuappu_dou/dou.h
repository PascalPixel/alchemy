#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_DOU_DOU_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
u8 *KuupuappuDou_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz);
void KuupuappuDou_PushBlockAhead(void);
void SceneState_ApplyFlag300(void);
void KuupuappuDou_ShowLockedDoorMessage(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 IsActor9AtTile15x54(void);
void FieldScene_RunFlag9a9GuardedScene(void);
void SceneState_ApplyThreeRects(void);
void FieldScene_RunScene3a7SequenceA(void);
void FieldScene_NoOp(void);
void SceneState_ApplyThreeRectsRows9And10(void);
s32 SceneActor_IsActor10AtTile16x12(void);
void FieldScene_RunGuardedStep9AAAfterSetup(void);
void Resource3a7_NoOpCallback(void);
void FieldScene_RunGuardedStep9AA(void);
void SceneState_ApplyRectAndMarkActor16(void);
void SceneState_ConfigureRegion26_30AndMarkActor17(void);
void SceneState_ConfigureRegion26_30AndClearActor18Mode(void);
void SceneState_ApplyRectAndSetupActor19(void);
void SceneActor_SetupSlotTwenty(void);
void SceneActor_MarkSlot21AndSetFlag205(void);
void SceneState_DispatchByActorZeroDepth(void);
void KuupuappuDou_RaiseActorPriorities(void);
void KuupuappuDou_EnableGroupAutomaticPriority(void);
void KuupuappuDou_RunRumble(void);
void FieldScene_RunScene3a7SequenceB(void);
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a7SequenceD(void);
void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1);
void KuupuappuDou_SpawnPuffs(struct FieldActor *source);
void SceneActor_InitSlots10To15AndStartTask(void);
void SceneActor_SetupActors11To14AndInstallTask(void);
s32 KuupuappuDou_ApplyEntryState(void);
extern const s16 KuupuappuDou_DirectionSteps[];
extern const s32 KuupuappuDou_PuffScript[];
extern struct SceneEntrance gKuupuappuDouEntrancesOther[];
extern struct SceneEntrance gKuupuappuDouEntrances1[];
extern struct SceneEntrance gKuupuappuDouEntrances2[];
extern struct SceneEntrance gKuupuappuDouEntrances3[];
extern s16 gKuupuappuDouRegions[];
extern u32 gKuupuappuDouExitsOther[];
extern u32 gKuupuappuDouExits1[];
extern u32 gKuupuappuDouExits2[];
extern u32 gKuupuappuDouExits3[];
extern struct ScenePlacement gKuupuappuDouPlacementsOther[];
extern struct ScenePlacement gKuupuappuDouPlacements1[];
extern struct ScenePlacement gKuupuappuDouPlacements2[];
extern struct ScenePlacement gKuupuappuDouPlacements3[];
extern struct SceneEvent gKuupuappuDouEventsOther[];
extern struct SceneEvent gKuupuappuDouEvents1[];
extern struct SceneEvent gKuupuappuDouEvents2[];
extern struct SceneEvent gKuupuappuDouEvents3[];
extern u32 KuupuappuDou_ScheduleFrames[];
extern s32 KuupuappuDou_ScheduleTimer;
extern s32 KuupuappuDou_ScheduleIndex;
extern s32 KuupuappuDou_TickCounter;
extern s32 KuupuappuDou_TickValue;

#endif
