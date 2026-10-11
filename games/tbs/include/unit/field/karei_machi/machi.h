#ifndef ALCHEMY_UNIT_FIELD_KAREI_MACHI_MACHI_H
#define ALCHEMY_UNIT_FIELD_KAREI_MACHI_MACHI_H

#include "types.h"

struct FieldActor;
struct Obj_020036f8;
struct Object;
struct Resource3a8Effect;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

void SceneState_ApplyValues12And2And3(void);
s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect);
s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KareiMachi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunStepWithValue29df(void);
void FieldScene_RunStepWithValueFd6(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActorNineteenDialogue(void);
void FieldScene_RunSlotZeroFacingSequence(void);
void SceneDialogue_RunActorTenDialogue(void);
void SceneState_BranchOnSlotZeroFacing(void);
void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void);
void FieldScene_RunActorTwentyFourAngleDialogue(void);
void KareiMachi_RunExitDoor(void);
void RunSceneArrivalSetup(void);
void FieldScene_RunStagedGroupSequence(void);
void KareiMachi_ReleaseEffectLeader(void);
void SceneState_LinkRecordZeroWhenFlag200Clear(void);
void SceneActor_SetLeaderPriorityAutomatic(void);
void SceneState_ClearLeaderPriorityAutomatic(void);
void SceneDialogue_RunActor181Line916(void);
s32 Scene_Initialize(void);
void KareiMachi_SetupEntryActors(void);
void SceneState_CheckFlags941And940(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void KareiMachi_StartWindowTransition(void);
void FieldScene_RunMiddleSequence(void);
void SceneState_SetValues27Through34(void);
void SceneEffect_SetSlotVariantAndDescriptor(s32 a);
void KareiMachi_SpeedUpActors(void);
void KareiMachi_AlternateDance(void);
void FieldScene_RunTwoActorCutsceneSequence(void);
void FieldScene_RunSecondaryGroupSequence(void);
struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z);
void FieldScene_RunLateSequence(void);
void FieldScene_RunScene3a8SequenceB(void);
void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p);
void FieldScene_RunSupplementalSequenceOne(s32 a0);
void FieldScene_RunScene3a8SequenceA(s32 a0, s32 a1, s32 a2);
void SceneEffect_UpdateLobeOrbitEffect26(void);
void FieldScene_DrawTilesAndRaiseActor11(void);
void InitializeOrbitingRenderEffect(s32 id);

#endif
