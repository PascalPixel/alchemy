#ifndef ALCHEMY_UNIT_FIELD_KARAGORU_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_KARAGORU_DOU_DOU_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct HeightTrackedObject;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gKaragoruDouEntrancesOther[];
extern struct SceneEntrance gKaragoruDouEntrances1[];
extern struct SceneEntrance gKaragoruDouEntrances2[];
extern struct SceneEntrance gKaragoruDouEntrances3[];
extern u32 gKaragoruDouExits[];
extern struct ScenePlacement gKaragoruDouPlacementsOther[];
extern struct ScenePlacement gKaragoruDouPlacements1[];
extern struct ScenePlacement gKaragoruDouPlacements1Flag96f[];
extern struct ScenePlacement gKaragoruDouPlacements2[];
extern struct ScenePlacement gKaragoruDouPlacements3[];
extern struct SceneEvent gKaragoruDouEventsOther[];
extern struct SceneEvent gKaragoruDouEvents1[];
extern struct SceneEvent gKaragoruDouEvents1Flag96f[];
extern struct SceneEvent gKaragoruDouEvents2[];
extern struct SceneEvent gKaragoruDouEvents3[];

#endif
