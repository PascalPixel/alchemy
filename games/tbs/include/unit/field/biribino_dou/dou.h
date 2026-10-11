#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_DOU_DOU_H

#include "types.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *BiribinoDou_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneState_ConfigureRegion1_0_21x14(void);
void SceneState_ConfigureRegion0_0_21x14(void);
void SceneState_ApplyTwoRects(void);
void FieldScene_RunTwoLayoutSteps(void);
void FieldScene_RunActor9Flag882Scene(void);
void FieldScene_RunScene398SequenceA(void);
void FieldScene_RunActorFifteenScene(void);
void FieldScene_RunActorSixteenScene(void);
void FieldScene_RunActor17Steps28AndD2(void);
void FieldScene_RunScene398SequenceB(void);
void ActorPresentation_SetSceneCell31AndFlag305(void);
void BiribinoDou_SetMapWorkFlag(void);
void BiribinoDou_ClearMapWorkFlag(void);
s32 Scene_Initialize(void);
void RunGuardedSceneSetup(void);
void BiribinoDou_PrepareWindowTransition(void);
void FieldScene_RunScene398SequenceC(void);
struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z);
void StagedActor_PushActorAhead(void);
void Resource398_ImportBankNoOp(void);

#endif
