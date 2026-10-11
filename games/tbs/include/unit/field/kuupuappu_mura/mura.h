#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_MURA_H

#include "types.h"

struct FieldActor;
struct OverlayEffectMotion;

void SceneEffect_SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SceneActor_UpdateProximityToLeader(struct FieldActor *self);
s32 ActorPresentation_UpdateEntityFromLeader(struct FieldActor *entity);
void *SceneData_GetScriptTable(void);
int SceneData_ReturnZero(void);
void *SceneData_GetMessageTable(void);
void *SceneData_SelectActorTableByFlag855(void);
int KuupuappuMura_GetIwanVenusDjinnCount(void);
s32 OverlayObject_RunObject2WhenFlagged(void);
void KuupuappuMura_SayFarewellToIvan(void);
void KuupuappuMura_InspectWell(void);
void *KuupuappuMura_SelectEvents(void);
void SceneDialogue_RunActor9LineAndAdvance(void);
void ActorPresentation_RunActorThirteenSceneSetup(void);
void ActorPresentation_RunActorSeventeenSceneSetup(void);
void ActorPresentation_RunActorEighteenSceneSetup(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor16Line(void);
void SceneDialogue_RunActor19Line(void);
void ActorPresentation_RunActorFourteenDialogue(void);
void ActorPresentation_RunActorFifteenDialogue(void);
void Villager_LookForRoofer(void);
void SceneActor_RunActorStep(s32 actor);
void SceneActor_RunActorCommandWithFlag91(s32 x);
void ActorPresentation_RunActorEightSceneSetup(void);
void SceneDialogue_RunActor11SecondLine(void);
void SceneDialogue_RunActor12LineAndAdvance(void);
void SceneDialogue_RunActor13Line(void);
void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void);
void ActorPresentation_RunActorFifteenFollowupDialogue(void);
void ActorPresentation_RunActorSixteenSceneSetup(void);
void ActorPresentation_RunActorEighteenFollowupSceneSetup(void);
void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void);
void SceneDialogue_RunActor20Line(void);
void SceneDialogue_RunActor11FlaggedLine(void);
void SceneDialogue_RunActor13FlaggedLine(void);
void ActorPresentation_RunActorFourteenFlaggedDialogue(void);
void ActorPresentation_RunActorFifteenScriptBranch(void);
void ActorPresentation_RunActorSixteenScriptBranch(void);
void ActorPresentation_RunActorNineteenScriptBranch(void);
void SceneDialogue_RunActor21Line(void);
void SceneState_Apply200ThenPlace55_26(void);
void SceneState_Apply200ThenPlace23_23(void);
void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation);
void FieldScene_SetupScene5At408_320(void);
void FieldScene_SetupScene6At312_304(void);
void FieldScene_SetupScene7At216_288(void);
void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void);
void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void);
void FieldScene_SetupScene10At120_144(void);
void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void);
void FieldScene_SetupScene13At152_264(void);
void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40);
void FieldScene_RunActor23SequenceOnceByFlag867(void);
void FieldScene_RunActor19MotionSequence(void);
s32 KuupuappuMura_RestoreEntryState(void);
s32 SceneActor_GetPositionDistance(s32 *a, s32 *b);
s32 SceneActor_CheckFacingAndRange(struct FieldActor *actor, struct FieldActor *target, s32 range, s32 force);
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);
void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect);
void KuupuappuMura_SpawnDriftingEffect(s32 flags);

#endif
