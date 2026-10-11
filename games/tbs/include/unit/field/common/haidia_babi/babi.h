#ifndef ALCHEMY_UNIT_FIELD_COMMON_HAIDIA_BABI_BABI_H
#define ALCHEMY_UNIT_FIELD_COMMON_HAIDIA_BABI_BABI_H

#include "types.h"

struct FacingObject;
union FieldObject;

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *obj);
s32 *HaidiaBabi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 HaidiaBabi_SelectExits(void);
s32 HaidiaBabi_SelectPlacements(void);
void HaidiaBabi_AskAboutKraden(s32 object);
s32 HaidiaBabi_SelectEvents(void);
void HaidiaBabi_RunHeyBoyScene(void);
void SceneDialogue_RunActorFourteenDialogue11AA(void);
void HaidiaBabi_CloseScreenAndExit(s32 object);
void SceneState_SetValue123Mode1(void);
void FieldScene_RunStep7BThen2(void);
void SceneState_SetValue123Mode3(void);
void FieldScene_RunStep7BThen4(void);
void FieldScene_RunStep80Then5(void);
void FieldScene_RunStep7BThen6(void);
void FieldScene_RunStep80Then7(void);
void SceneState_SetValue129Mode8(void);
void SceneState_SetValue129Mode9(void);
void FieldScene_RunStep7BThen10(void);
void SceneState_ApplyValues123And11(void);
s32 HaidiaBabi_RestoreEntryState(void);
void FieldScene_RunComplexActorSequence(void);
void FieldScene_RunPaletteRampSequence(void);
void HaidiaBabi_RunInnkeeperTalk(void);
void SceneDialogue_ShowLine1C13WithActor16Steps(void);
void SceneDialogue_RunActorThirteenDialogue(void);
void SceneDialogue_RunActor16LineAndFlag81c(void);
void HaidiaBabi_RunSickbedVisit(void);
void HaidiaBabi_RunActorEightMessageScene(void);
void ActorPresentation_SetTwoSceneCells(void);
void FieldScene_RunSupplementalSequenceOne(void);
void SceneEffect_UpdateObjectByFrameParity(union FieldObject *object);
void OverlayObject_UpdateOnFrameParity(union FieldObject *object);
void OverlayObject_ApplyRandomSlotOnOddFrames(union FieldObject *object);
void SceneEffect_UpdateAnchoredRiseFrame(union FieldObject *object);
void OverlayObject_UpdateArcFromAnchor(union FieldObject *object);
void HaidiaBabi_SpawnEffectPair(union FieldObject *object);
void SceneState_SetValue140Mode0(void);
void FieldScene_RunSingleStep(void);
void FieldScene_RunActor8TwoStep(void);
void FieldScene_RunStep17(void);
void FieldScene_RunSixStepSequence17e4(void);

#endif
