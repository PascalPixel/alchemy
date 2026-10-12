#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_HEYA_HEYA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
s32 BiribinoHeya_PreparePlacements(void);
void FieldScene_RunActor16MessageBranch(void);
void FieldScene_RunActor18MessageBranch(void);
u8 *SceneData_GetTertiaryTable(void);
void FieldScene_RunActor17MessageBranch(void);
void FieldScene_ConfigureActor21Scene(void);
void FieldScene_RunActor24Sequence(void);
void FieldScene_RunActor27Sequence(void);
void FieldScene_RunActor8Message(void);
void FieldScene_RunActor13Message(void);
void FieldScene_RunActor19MessageBranch(void);
void FieldScene_RunActor21SequenceOnFlag300(void);
void FieldScene_ConfigureActor22Scene(void);
void FieldScene_ConfigureActor23Scene(void);
void FieldScene_RunActor27Message(void);
void FieldScene_RunActor10MessageBranch(void);
s32 FieldScene_SetupActor27OnEntry(void);
extern struct SceneEntrance BiribinoHeya_PrimaryTable[];
extern u32 BiribinoHeya_SecondaryTable[];
extern struct ScenePlacement gBiribinoHeyaPlacements[];
extern struct SceneEvent BiribinoHeya_TertiaryTable[];

#endif
