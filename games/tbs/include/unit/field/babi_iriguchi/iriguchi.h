#ifndef ALCHEMY_UNIT_FIELD_BABI_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_BABI_IRIGUCHI_IRIGUCHI_H

#include "types.h"

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

#endif
