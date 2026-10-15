#ifndef ALCHEMY_UNIT_SCENE_VALE_KRADEN_H
#define ALCHEMY_UNIT_SCENE_VALE_KRADEN_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_SelectEffectTable(void);
s32 ValeKraden_RestoreEntryState(void);
extern s32 Kraden_Actor11RockslideActions[];
extern s32 Kraden_Actor11Actions[];
extern s32 Kraden_StrangerActions[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern struct SceneEvent Placement_Effects834[];
extern struct SceneEvent Placement_Effects87a[];
extern u16 Kraden_GateCells[];

#endif
