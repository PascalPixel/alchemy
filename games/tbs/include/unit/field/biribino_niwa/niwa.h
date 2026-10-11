#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_NIWA_NIWA_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_NIWA_NIWA_H

#include "types.h"

struct FacingObject;
struct SceneEntity_0200090c;
struct SceneEvent;
struct ScenePlacement;

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object);
u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunStepWithValueFd2(void);
void FieldScene_RunStepWithValue29de(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_AskAboutBarricade(void);
void SceneDialogue_AskIfResponsible(void);
void SceneDialogue_AskIfFineWarrior(void);
void BiribinoNiwa_RunGardenEvent(void);
void BiribinoNiwa_LeaveThroughGate(void);
s32 Scene_Initialize(void);
void BiribinoNiwa_ApplyEntryState(void);
void BiribinoNiwa_RunGardenScene(void);
void FieldScene_OpenGate(void);
s32 SceneEffect_UpdateLobeOrbitEntity(struct SceneEntity_0200090c *entity);
void InitializeOrbitingSceneEntity(s32 id);

#endif
