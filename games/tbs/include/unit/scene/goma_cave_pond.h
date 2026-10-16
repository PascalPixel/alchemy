#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_POND_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_POND_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
u8 *SceneData_GetTertiaryTable(void);
u8 *SceneData_GetQuaternaryTable(void);
s32 Scene_PlaceActor8OnGate300(void);
extern struct SceneEntrance gGomaCavePond_SceneTable0[];
extern u32 gGomaCavePond_SceneTable1[];
extern struct ScenePlacement gGomaCavePond_SceneTable2[];
extern struct SceneEvent gGomaCavePond_SceneTable3[];

#endif
