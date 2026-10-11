#ifndef ALCHEMY_UNIT_FIELD_KAREI_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_KAREI_HEYA_HEYA_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KareiHeya_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActor12DialogueAndSetFlag910(void);
void SceneDialogue_RunActor16Dialogue(void);
void SceneDialogue_RunActor8FlaggedDialogue(void);
void SceneDialogue_RunActor8FacingDialogue(void);
void FieldScene_RunArrivalPlacement(void);
s32 Scene_Initialize(void);
void SceneState_ClearSlotsBySubState(void);

#endif
