#ifndef ALCHEMY_UNIT_FIELD_COMMON_SHIAN_JIIN_JIIN_H
#define ALCHEMY_UNIT_FIELD_COMMON_SHIAN_JIIN_JIIN_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
s32 StopXianActor(void *actor);
s32 FaceXianActorToPlayer(struct FieldActor *actor);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 GetXianInitialState(void);
s32 GetXianMessageData(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void ShianJiin_AskBridgeCrossing(void);
void FieldScene_RunFlag88FBranch(void);
void ShianJiin_RunTempleWalkScene(void);
void Scene_RunActorNineTransition(void);
void Scene_RunScene39eSequenceB(void);
void FieldScene_ShowDialogue17B1(void);
void FieldScene_ShowDialogue1825(void);
void FieldScene_RunRoofSceneExit(void);
void FieldScene_ShowDialogue182D(void);
void FieldScene_RunForwardArcBurst(void);
void ShianJiin_SpinAway(void);
void FieldScene_RunDescentBurst(void);
void Scene_RunPrimarySequence(void);
void ShianJiin_WalkByFacing(void);
void ShianJiin_RunMasterScene(s32 mode);
void ShianJiin_RunGatheringScene(void);
void FieldScene_DispatchApproachByFacing(void);
void FieldScene_DispatchByFacing(void);
void FieldScene_DispatchByFacingAndFlags(void);
void FieldScene_RunSecondEnsembleBeat(void);
void FieldScene_RunSkippableStoryBeat(void);
void FieldScene_RunEnsembleStoryBeat(void);
void FieldScene_ShowDialogue1A58(void);
void StartSchoolDoorEvent(void);
void FieldScene_DispatchByRange(void);
void FieldScene_ShowDialogue17DF(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_SpawnRandomizedParticle(void);
void FieldScene_ApplyOffset0Neg32(void);
void FieldScene_ApplyOffset0Pos32(void);
void FieldScene_ApplyOffsetNeg32_0(void);
void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1);
void FieldScene_RunScene39eSequenceA(void);
void FieldScene_PlaySound123AndEnable(void);
void ShianJiin_RunExitTransition(void);
void ShianJiin_AskIfCurious(void);
void FieldScene_RunRoofEnsembleSequence(void);
s32 ShianJiin_ApplyEntryState(void);
void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1);
void FieldScene_FinishSequence(void);
void FieldScene_SpawnEightShots(void);
void FieldScene_SelectActorModeFromInputBit(s32 arg0);
void FieldScene_RunParticleRain(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 ShianJiin_ActorTwelveScript[];
extern s32 gShianJiinPlacement1[];
extern u16 ShianJiin_CellStepsA[];
extern u16 ShianJiin_CellStepsB[];
extern u16 ShianJiin_CellStepsC[];
extern struct SceneEntrance gShianJiinEntrances1[];
extern struct SceneEntrance gShianJiinEntrancesOther[];
extern u32 ShianJiin_XianMessages[];
extern struct ScenePlacement gShianJiinPlacements1[];
extern struct ScenePlacement gShianJiinPlacements[];
extern struct ScenePlacement gShianJiinPlacementsEntrance3[];
extern struct SceneEvent gShianJiinEvents1[];
extern struct SceneEvent gShianJiinEvents[];
extern struct SceneEvent gShianJiinEventsEntrance3[];
extern s32 ShianJiin_ShotScript[];

#endif
