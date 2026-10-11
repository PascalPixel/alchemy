#ifndef ALCHEMY_UNIT_FIELD_KARAGORU_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_KARAGORU_DOU_DOU_H

#include "types.h"

struct EffectWork;
struct HeightTrackedObject;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SetEffectRecordMode(struct EffectWork *work, s32 mode);
void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind);
void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunScene3beSequenceB(void);
void ActorPresentation_RunActorElevenRecoveryScene(void);
void KaragoruDou_AskToCross(void);
void ActorPresentation_SelectActorNineScript(void);
void KaragoruDou_LeaveArena(void);
void StagedActorPairScene_RunStep(void);
void ActorPresentation_RunActorEightThresholdScene(void);
void StagedActorPairScene_RunUpdate(void);
void ActorPresentation_RunActorNineThresholdScene(void);
void StagedActorPairScene_NoopActorCallback(void);
void StagedActorPairScene_RotateActorPart(u8 *actor);
void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object, s32 limit);
void StagedActorPairScene_RunSpinningLeap(s32 id);
void StagedActorPairScene_NoopSceneCallback(void);
void StagedActorPairScene_RunActorTwelveCommand(void);
s32 Scene_Initialize(void);

#endif
