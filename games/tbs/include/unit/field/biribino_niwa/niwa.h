#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_NIWA_NIWA_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_NIWA_NIWA_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

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
extern u16 Niwa_GateCells[];
extern s32 BiribinoNiwa_GuardScript[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoNiwaPlacementsOther[];
extern struct ScenePlacement gBiribinoNiwaPlacements[];
extern struct SceneEvent gBiribinoNiwaEventsOther[];
extern struct SceneEvent gBiribinoNiwaEvents[];

#endif
