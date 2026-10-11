#ifndef ALCHEMY_UNIT_FIELD_SHIAN_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_SHIAN_HEYA_HEYA_H

#include "types.h"

struct FieldActor;

void OverlayObject_SetEntryField(struct FieldActor *object, s32 priority);
void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 type);
s32 ShianHeya_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 ShianHeya_GetExits(void);
s32 ShianHeya_SelectPlacements(void);
void SceneDialogue_RunActor14FlaggedDialogue(void);
void SceneDialogue_RunActor15FlaggedDialogue(void);
void SceneDialogue_RunActor16FlaggedDialogue(void);
void SceneDialogue_RunActor17FlaggedDialogue(void);
void SceneDialogue_RunActor10Dialogue(void);
void SceneDialogue_RunActor12Dialogue(void);
void SceneDialogue_RunActor9MotionDialogue(void);
s32 ShianHeya_SelectEvents(void);
s32 ShianHeya_PrepareWindowTransition(void);

#endif
