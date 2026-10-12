#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_DOU_DOU_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SceneState_SetValues8_3_4(void);
void SceneActor_WaitActorDescent(struct FieldActor *actor);
s32 SceneActor_FaceActorZero(struct FieldActor *obj);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_ConfigureRegionAtRow15(void);
void FieldScene_ConfigureRegionAtRow17(void);
void HaidiaDou_SinkPillarColumn23(void);
void HaidiaDou_SinkPillarColumn27(void);
void FieldScene_RunInitBracketThenSequence(void);
s32 FieldScene_RunPrimarySequence(s32 a0);
void FieldScene_RunScene3a6SequenceA(void);
void FieldScene_RunScene3a6SequenceB(void);
void FieldScene_RunScene3a6SequenceC(void);
void FieldScene_RunActor8ZeroStep(void);
void HaidiaDou_RunProbedColumnScene(void);
void HaidiaDou_ToppleActorNine(void);
void HaidiaDou_RunLoweredActorScene(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 HaidiaDou_RunSceneScript(void);
void HaidiaDou_PlayCueAndRequestTouchedExit(void);
void DialogueLayout_ConfigureRowsByFlag301(void);
void SceneActor_PositionPair(s32 a0, s32 a1, s32 a2);
void FieldScene_RunShiftAndSetFlag301(void);
void FieldScene_RunActor11Transition301(void);
void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y);
void HaidiaDou_ApplyEntryState(void);
void ActorPresentation_AdvanceActorElevenStates(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 HaidiaDou_WalkTargets[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gHaidiaDouEntrancesOther[];
extern struct SceneEntrance gHaidiaDouEntrances1[];
extern struct SceneEntrance gHaidiaDouEntrances2[];
extern struct SceneEntrance gHaidiaDouEntrances3[];
extern u32 HaidiaDou_Exits[];
extern struct ScenePlacement gHaidiaDouPlacements1[];
extern struct ScenePlacement gHaidiaDouPlacements2[];
extern struct ScenePlacement gHaidiaDouPlacements3[];
extern struct SceneEvent gHaidiaDouEventsOther[];
extern struct SceneEvent gHaidiaDouEvents1[];
extern struct SceneEvent gHaidiaDouEvents2[];
extern struct SceneEvent gHaidiaDouEvents3[];

#endif
