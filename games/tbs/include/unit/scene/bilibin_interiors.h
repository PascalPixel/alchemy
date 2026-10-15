#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_INTERIORS_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
s32 BilibinInteriors_PreparePlacements(void);
u8 *SceneData_GetTertiaryTable(void);
s32 FieldScene_SetupActor27OnEntry(void);
extern struct SceneEntrance BilibinInteriors_PrimaryTable[];
extern u32 BilibinInteriors_SecondaryTable[];
extern struct ScenePlacement gBilibinInteriorsPlacements[];
extern struct SceneEvent BilibinInteriors_TertiaryTable[];

#endif
