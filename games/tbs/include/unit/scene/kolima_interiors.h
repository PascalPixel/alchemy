#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_INTERIORS_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
u8 *SceneData_GetPreparedRecords(void);
u8 *SceneData_GetFourthTable(void);
s32 FieldScene_SetupEntryLayoutsBySelector(void);
extern struct SceneEntrance gEntryLayout_PrimaryTable[];
extern u32 gEntryLayout_SecondaryTable[];
extern struct ScenePlacement gEntryLayout_Records[];
extern struct SceneEvent gEntryLayout_FourthTable[];

#endif
