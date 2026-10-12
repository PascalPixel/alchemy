#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_MURA_SAI_SAI_H

#include "types.h"
#include "field_scene.h"

struct SceneActor_02000350;

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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 KuupuappuMuraSai_Actor0[];
extern s32 KuupuappuMuraSai_Actor1[];
extern struct SceneEntrance KuupuappuMuraSai_Scripts[];
extern u32 KuupuappuMuraSai_Messages[];
extern struct ScenePlacement KuupuappuMuraSai_Actors[];
extern u16 KuupuappuMuraSai_Scene5Cells[];
extern u16 KuupuappuMuraSai_Scene6Cells[];
extern u16 KuupuappuMuraSai_Scene7Cells[];
extern u16 KuupuappuMuraSai_Scene8Cells[];
extern u16 KuupuappuMuraSai_Scene9Cells[];
extern u16 KuupuappuMuraSai_Scene10Cells[];
extern u16 KuupuappuMuraSai_Scene12Cells[];
extern struct SceneEvent KuupuappuMuraSai_Extras[];

#endif
