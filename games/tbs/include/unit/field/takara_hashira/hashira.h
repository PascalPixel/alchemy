#ifndef ALCHEMY_UNIT_FIELD_TAKARA_HASHIRA_HASHIRA_H
#define ALCHEMY_UNIT_FIELD_TAKARA_HASHIRA_HASHIRA_H

#include "types.h"

struct FieldActor;
struct MapCell;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct StagedActorProbe;
union FieldObject;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilSettledAndReset(u8 *obj);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *obj, s32 limit);
s32 FieldScene_QueryActorFootprint(s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top);
s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no);
s32 SceneActor_ApplyPlacementQuery(u8 *no);
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst);
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *source);
s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far);
void StagedActor_StepDownUntilClamp(s32 index);
void FieldScene_RunPrimarySequence(void);
s32 FieldScene_RunScene3b3SequenceD(void);
void FieldScene_RunScene3b3SequenceE(union FieldObject *object);
s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunTransitionOrFallback(void);
void SceneState_ApplyPlacementResult(void);
void TakaraHashira_JitterBackgroundScroll(void);
void CopyAndOffsetCoordinatePreset(void);
void FieldScene_RunScene3b3SequenceA(void);
void FieldScene_RunFlaggedDisplayScene(void);
s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front);
s32 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor);
s32 TakaraHashira_UpdateActorPriority(struct FieldActor *actor);
void TakaraHashira_SortPillarActors(void);
void TakaraHashira_UpdatePillarActors(void);
void TakaraHashira_PushPillar(void);
void FieldScene_RunSingleStep(void);
void TakaraHashira_RunStagedCellScene(void);
void TakaraHashira_AttachPillarRider(void);
void TakaraHashira_ReleasePillarRider(void);
s32 SceneActor_UpdateBit1ByPositionToSlotZero(struct FieldActor *actor);
void TakaraHashira_RunActorAction(s32 id);
void FieldScene_RunActor11Step(void);
void FieldScene_RunActor12Step(void);
void TakaraHashira_CheckLeaderOnPillar(void);
void TakaraHashira_RaiseActorFourteen(void);
void SceneEffect_SpawnRandomizedParticle(void);
s32 SceneEffect_SpawnRandomEffectEveryEightFrames(struct FieldActor *actor);
void TakaraHashira_RunMapShiftScene(void);
void StagedActor_PlaceAtObjectTenCell(void);
void TakaraHashira_DropActorTen(void);
void TakaraHashira_RunPushScene(void);
const struct SceneEvent *Scene_GetEvents(void);
void TakaraHashira_PrepLoweredActor(s32 id);
void TakaraHashira_RestoreFlaggedPillar(s32 a0);
void OverlayObject_SetCallbackAndMode2(s32 id);
void SceneActor_CheckActors8To11NearSlotZero(void);
s32 TakaraHashira_SetupArea(void);

#endif
