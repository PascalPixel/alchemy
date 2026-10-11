#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H

#include "types.h"

struct FieldActor;
struct SceneActor_02000350;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObjectB(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SceneActor_GetPositionDistance(s32 *first_position, s32 *second_position);
s32 SceneActor_UpdateProximity(struct SceneActor_02000350 *actor, struct SceneActor_02000350 *target, s32 range, s32 force);
s32 UpdateActorProximity(u8 *actor);
const void *SceneData_GetScriptTable(void);
int SceneData_ReturnZero(void);
const void *SceneData_GetMessageTable(void);
const void *SceneData_GetActorTable(void);
void ActorPresentation_RunActorModeOneThenZero(s32 actor);
void SceneDialogue_RunActor8FlagScene(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor12TwoFlagScene(void);
void SceneDialogue_ShowLine1CB0ForActor13(void);
void SceneDialogue_RunActorFourteenFlagDialogue(void);
void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void);
void SceneDialogue_RunActor16CountedDialogue(void);
void SceneDialogue_RunActorEightTimedDialogue(void);
void SceneDialogue_RunActor11AcceptanceDialogue(void);
void SceneDialogue_RunActor12TimedTwoFlagScene(void);
void ActorPresentation_RunActor13AcceptanceDialogue(void);
void KuupuappuMuraSai_RunActor14Talk(void);
void SceneDialogue_RunActorFifteenDialogue(void);
void FieldScene_RunScene385SequenceA(void);
void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40);
void FieldScene_RunActor21SequenceOnce(void);
void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay);
void FieldScene_SetupScene5(void);
void ActorPresentation_SetupActorEighteenAt312_304(void);
void FieldScene_SetupScene7(void);
void ActorPresentation_SetupActorZeroForSceneEight(void);
void ActorPresentation_SetupActorZeroForSceneNine(void);
void FieldScene_SetupScene10(void);
void ActorPresentation_SetupActorZeroForSceneTwelve(void);
void SceneState_SetFlag200AndConfigureRegion55_26(void);
void ActorPresentation_SetFlag200AndSceneCell23(void);
void FieldScene_SetActor21Values0And4(void);
void FieldScene_RunActor18MotionSequence(void);
void ActorPresentation_SetPairedSceneCells(void);
void ActorPresentation_SetAlternatePairedSceneCells(void);
void FieldScene_RunActorEighteenDialogue(void);
void KuupuappuMuraSai_InspectWell(void);
const u8 *SceneData_GetExtraTable(void);
s32 SceneSetup_InitializeActorsAndFlags(void);

#endif
