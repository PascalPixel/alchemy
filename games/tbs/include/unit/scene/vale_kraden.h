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
extern s32 gKraden_Actor11RockslideActions[];
extern s32 gKraden_Actor11Actions[];
extern s32 gKraden_StrangerActions[];
extern struct SceneEntrance gPlacement_Scripts[];
extern u32 gPlacement_Messages[];
extern struct ScenePlacement gPlacement_Actors[];
extern struct SceneEvent gPlacement_Effects[];
extern struct SceneEvent gPlacement_Effects834[];
extern struct SceneEvent gPlacement_Effects87a[];
extern u16 gKraden_GateCells[];

#endif
