#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_HEYA_HEYA_H

#include "types.h"

struct FixedPointPosition;
struct SceneEvent;
struct ScenePlacement;
struct StopRecord;
struct StopSet;

u8 *SceneData_GetScriptTable(void);
u8 *SceneData_GetRegionTable(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
s32 KuupuappuHeya_TryJumpTo(struct FixedPointPosition *pos);
void ActorPresentation_SetSceneCellByAngle(void);
void FieldScene_RunObjectTwentySixPositionCheck(void);
void FieldScene_RunActorNineteenAngleDialogue(void);
void FieldScene_RunActorTwentyAngleDialogue(void);
void FieldScene_RunActorTwentyThreeAngleDialogue(void);
void FieldScene_RunActorEighteenConditionalScene(void);
void SceneDialogue_ShowLine12BB(void);
void SceneDialogue_ShowEmptyBarrel(void);
void SceneDialogue_ShowEmptyChest(void);
const struct SceneEvent *Scene_GetEvents(void);
void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x);
void SceneState_RunGuardedActorStep(s32 x);
void SceneDialogue_PromptAndCountSkip(s32 x);
void SceneDialogue_RunActorElevenDialogue(void);
void KuupuappuHeya_TalkAboutIvan(void);
void SceneState_BranchOnSlotZeroFacingAndFlag855(void);
void SceneDialogue_RunActor9FlaggedLine(void);
void SceneDialogue_RunActorTwelveFlaggedDialogue(void);
void FieldScene_RunFlag856DialogueBranch(void);
void SceneDialogue_ShowLine128E(void);
void SceneActor_StepActor24AnimationByFacing(void);
void KuupuappuHeya_RunActor25Response(void);
void SceneDialogue_RunActor10Line(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor14Line(void);
void FieldScene_RunScene383SequenceC(void);
void KuupuappuHeya_OfferBone(void);
void SceneDialogue_RunActorNineFlaggedDialogue(void);
void SceneDialogue_RunActorElevenFlaggedDialogue(void);
void SceneDialogue_ShowLine124EOr135E(void);
void SceneDialogue_RunActor16FlaggedLine(void);
void SceneDialogue_RunActorEighteenBranchedDialogue(void);
void FieldScene_RunSetupSequence(void);
void Scene_JoinRodSearch(void);
void FieldScene_RunVaultClosingSequence(void);
void SceneState_SetValue123Mode11(void);
void FieldScene_RunOpeningSequenceHead(void);
void FieldScene_RunOpeningSequenceSecond(void);
void FieldScene_RunOpeningSequenceThird(void);
void FieldScene_RunScene383SequenceB(void);
void FieldScene_RunSteps107And250(void);
void FieldScene_ConfigurePairedActors(void);
void FieldScene_SelectActorPair(void);
void KuupuappuHeya_RunBranchingDialogueScene(void);
void KuupuappuHeya_PoseDialogueActors(void);
void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc);
void SceneState_SetFlagByActorPosition(void);
s32 KuupuappuHeya_ApplyEntryState(void);
void SceneActor_FaceActors24And25TowardActorZero(void);
void FieldScene_RunLateSequence(void);
void KuupuappuHeya_RunVaultEvent(void);
s32 KuupuappuHeya_GetIwanVenusDjinnCount(void);
s32 OverlayObject_RunObjectTwoWhenFlagged(void);
void RunEventScript01(void);
void RunDialoguePromptScene(void);
void KuupuappuHeya_OpenWindowTransition(void);
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);
void SceneEffect_ApplyPairWithValue141(s32 a, s32 b);
void SceneState_SetValue2ThenFinish(void);
void OverlayObject_ConfigureObject22WithResource17(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);
u8 *SceneData_FindEntryAtPosition(s32 *o);
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopSet *set, s16 *pos);
s32 KuupuappuHeya_IsRecordNearPoint(s32 x, s32 y, s32 id);
s32 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id);
s32 SceneActor_CheckTileFreeOfKinds(u8 *p);
void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p);
void KuupuappuHeya_UpdateActorStops(void);
void KuupuappuHeya_StartActorStops(void);

#endif
