#ifndef ALCHEMY_UNIT_SCENE_SORU_MEIRO_H
#define ALCHEMY_UNIT_SCENE_SORU_MEIRO_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
extern struct SceneEntrance SoruMeiro_SceneTable0[];
extern u32 SoruMeiro_SceneTable1[];
extern struct ScenePlacement SoruMeiro_SceneTable2[];
extern struct SceneEvent SoruMeiro_SceneTable3[];

#endif
