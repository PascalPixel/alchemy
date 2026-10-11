#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_SUKURETA_SUKURETA_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_SUKURETA_SUKURETA_H

#include "types.h"
#include "field_scene.h"

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
extern s32 Sukureta_Actor11RockslideActions[];
extern s32 Sukureta_Actor11Actions[];
extern s32 Sukureta_StrangerActions[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern struct SceneEvent Placement_Effects834[];
extern struct SceneEvent Placement_Effects87a[];
extern u16 Sukureta_GateCells[];

#endif
