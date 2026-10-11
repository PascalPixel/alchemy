#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_HEYA_SAI_PLACEMENT_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_HEYA_SAI_PLACEMENT_H

#include "types.h"
#include "field_scene.h"

void SceneState_CheckPositionWindow(void);
u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetPreparedActors(void);
void SceneActor_RunActorStep(s32 arg0);
void SceneDialogue_RunActor9Line(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor12Line(void);
void FieldScene_RunActor16Sequence(void);
void SceneDialogue_RunActor16Line(void);
void SceneDialogue_RunActor23Line(void);
void FieldScene_RunActor18FlaggedSequence(void);
void SceneActor_RunActor16StepWithFlag91(void);
void FieldScene_RunActor18ConditionalCue(void);
void FieldScene_RunActor19StepByPlace(void);
void FieldScene_RunActor20StepByPlace(void);
void FieldScene_RunActor21StepByPlace(void);
void FieldScene_RunActor22StepByPlace(void);
void SceneDialogue_RunActor18FlaggedLine(void);
u8 *SceneData_GetEffectTable(void);
s32 FieldScene_InitSceneStateByStep(void);
void OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];

#endif
