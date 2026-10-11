#ifndef ALCHEMY_UNIT_FIELD_KAREI_KYUDEN_KYUDEN_H
#define ALCHEMY_UNIT_FIELD_KAREI_KYUDEN_KYUDEN_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActor13Message1b83(void);
void SceneDialogue_RunActor16Message1b88(void);
void FieldScene_RunActorEightTurnDialogue(void);
void KareiKyuden_LeaveThroughDoor(void);
s32 Scene_Initialize(void);
void FieldScene_DispatchSceneByIndex(void);
void RunEventScript01(void);
void ConfigureFourSceneChannelsAndHandoff(s32 handoff);
void SceneChannel_ConfigureUniformAndHandoff(s32 handoff);
void Scene_RunPartySequence(void);

#endif
