#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_SUKURETA_SUKURETA_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_SUKURETA_SUKURETA_H

#include "types.h"

struct FacingObject;

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object);
u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_SelectEffectTable(void);
void Scene_RunActorTwelveDialogue(void);
void Scene_PlanSanctumVisit(void);
void Scene_LeaveForMtAleph(void);
void FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear(void);
void Scene_OverhearSaturosAndMenardi(void);
void HaidiaSukureta_RunArrivalScene(void);
s32 HaidiaSukureta_RestoreEntryState(void);
void HaidiaSukureta_RunActorSequence(void);
void SceneState_SetWorkAndFlag87d(void);
void SceneState_SetWorkAndFlag87e(void);
void SceneDialogue_RunActorSixteenDialogue(void);

#endif
