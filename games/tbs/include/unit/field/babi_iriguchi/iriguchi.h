#ifndef ALCHEMY_UNIT_FIELD_BABI_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_BABI_IRIGUCHI_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

struct MotionEffect;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct StagedActor;

void OverlayObject_SetOwnerMode(u8 *object, s32 mode);
u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind);
u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind);
void SceneState_SetValue8Mode66(void);
void OverlayObject_WaitUntilIdle(s32 *obj);
void Effect_AdvanceMotion(struct MotionEffect *effect);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunScene3c5SequenceA(s32 exit);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiIriguchi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void BabiIriguchi_JumpFromLedge(void);
void FieldScene_RunFourCallSequence(void);
void SceneState_BranchOnActorEightOrNineTile(void);
void ResetSceneParametersAndFinishSetup(void);
void FieldScene_RunStep11(void);
void FieldScene_RunStep12WithPosition(void);
void FieldScene_RunStep13WithTwoPositions(void);
void FieldScene_RunStep15(void);
void FieldScene_RunStepWithValue2693(void);
void FieldScene_RunBranchingActorSequence(void);
void FieldScene_RunActorEventSequence(void);
void ActorPresentation_SetSceneCellByFlag985(void);
void SceneState_ApplyRectAt32x78(void);
void BabiIriguchi_OpenTruthDoor(void);
void BabiIriguchi_CloseTruthDoor(void);
void BabiIriguchi_FlipTruthDoorSwitch(void);
struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor);
void BabiIriguchi_EnableEffectChildOption(void);
void ActorPresentation_PlaceActorTwelveAtTile20And12(void);
void SceneActor_PushObjectAheadIfLevel(void);
void SceneActor_RunSlotZeroFacingCheck(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneState_ConfigureRegion82_7AndApply768(void);
void SceneState_ApplyRectsAtActors8And9(void);
void BabiIriguchi_MatchLeaderPriority(void);
s32 BabiIriguchi_SetupScene(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gBabiIriguchiPlacements3_Data[];
extern struct SceneEntrance gBabiIriguchiEntrances3[];
extern struct SceneEntrance gBabiIriguchiEntrances2[];
extern struct SceneEntrance gBabiIriguchiEntrances1[];
extern struct SceneEntrance gBabiIriguchiEntrancesOther[];
extern s16 gBabiIriguchiRegions3[];
extern u32 gBabiIriguchiExits[];
extern struct ScenePlacement gBabiIriguchiNoPlacements[];
extern struct ScenePlacement gBabiIriguchiPlacements3[];
extern struct ScenePlacement gBabiIriguchiPlacements2[];
extern struct ScenePlacement gBabiIriguchiPlacements1[];
extern struct ScenePlacement gBabiIriguchiPlacementsOther[];
extern struct SceneEvent gBabiIriguchiEvents3[];
extern struct SceneEvent gBabiIriguchiEvents2[];
extern struct SceneEvent gBabiIriguchiEvents1[];
extern struct SceneEvent gBabiIriguchiEventsOther[];

#endif
